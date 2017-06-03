
// incapsulating "independent of rdrand" sealing 
// the goal - replace all usages of rdrand inside of sealing APIs of SDK with another RNG, using rdrand as seed. 
// additional goal - generate additional entropy in IV for AES-GCM (instead of 0 as in AES-GCM implementation on Linux).

#pragma once
#include "sgx_tseal.h"
#include "sgx_utils.h"
#include "sgx_trts.h"
#include "sgx_attributes.h"
#include <stdlib.h>
#include <string.h>

// data types, copied with some rework from the SDK

typedef struct _independent_key_request_t
{
	uint16_t                        key_name;        /* Identifies the key required */
	uint16_t                        key_policy;      /* Identifies which inputs should be used in the key derivation */
	sgx_isv_svn_t                   isv_svn;         /* Security Version of the Enclave */
	uint16_t                        reserved1;       /* Must be 0 */
	sgx_cpu_svn_t                   cpu_svn;         /* Security Version of the CPU */
	sgx_attributes_t                attribute_mask;  /* Mask which ATTRIBUTES Seal keys should be bound to */
	sgx_key_id_t                    key_id;          /* Value for key wear-out protection */
	sgx_misc_select_t               misc_mask;       /* Mask what MISCSELECT Seal keys bound to */
	//uint8_t                         reserved2[SGX_KEY_REQUEST_RESERVED2_BYTES];  /* Struct size is 512 bytes - removing, I dont need it in every movie chunk */
} independent_key_request_t;

typedef struct _independent_gcm_data_t
{
	uint32_t  payload_size;                   /*  0: Size of the payload which includes both the encrypted data and the optional additional MAC text */
	uint8_t   iv[SGX_SEAL_IV_SIZE];						  /*  4: was Reserved bits , became IV for the chunk */
	uint8_t   payload_tag[SGX_SEAL_TAG_SIZE]; /* 16: AES-GMAC of the plain text, payload, and the sizes */
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning ( disable:4200 )
#endif
	uint8_t   payload[];                      /* 32: The payload data which includes the encrypted data followed by the optional additional MAC text */
#ifdef _MSC_VER
#pragma warning(pop)
#endif
} independent_aes_gcm_data_t;

typedef struct _independent_sealed_data_t
{
	independent_key_request_t  key_request;       /* 00: The key request used to obtain the sealing key */
	uint32_t           plain_text_offset; /* 64: Offset within aes_data.playload to the start of the optional additional MAC text */
	uint8_t            reserved[12];      /* 68: Reserved bits */
	independent_aes_gcm_data_t aes_data;          /* 80: Data structure holding the AES/GCM related data */
} independent_sealed_data_t;



class SGXIndependentSealing
{
public:
	static bool generate_random_data(unsigned char* data, size_t size);

	SGXIndependentSealing();
	~SGXIndependentSealing();
	
	static size_t calc_sealed_data_size(size_t data_size);
	static size_t calc_unsealed_data_size(size_t data_size);
	static bool   seal_data(unsigned char* in, size_t in_size, unsigned char**out, size_t *poutsize); // memomry allocated inside
	static bool   unseal_data(unsigned char* in, size_t in_size, unsigned char**out, size_t *poutsize); // memomry allocated inside
	static bool   destroy_allocated_data(unsigned char* data);

	typedef independent_sealed_data_t ind_sealing_header_t;

	typedef enum __chunk_size_consts
	{
		SEALING_HEADER_SIZE = sizeof(ind_sealing_header_t),
		UNSEALED_DATA_CHUNK_SIZE = 1024,
		SEALED_DATA_CHUNK_SIZE = UNSEALED_DATA_CHUNK_SIZE + SEALING_HEADER_SIZE,

	} chunk_size_consts_t;
};

