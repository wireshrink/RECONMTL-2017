
#include "stdafx.h"
#include "exploits.h"
#include "common_enclave_actions.h"
/*
small note on exploited bug: 
	*(reinterpret_cast<uint8_t*>(&data[i])) = xorshift128plus();
                        ^
						this should be uint64_t, not uint8_t

						This converts key id of 32 bytes to be 4 bytes of random data
						and 12 bytes of IV to be 2 bytes of random data.
						This actually means that we have a good chances to recover the movie 
						after some sequential downloads of it
In order to exploit this problem reliably we need to generate
2^(32+16) different  



*/
// create an enclave
//  do
//   write log 0s of 1024 byte length (movie chunk size)
//   read the file
//   copy it with name including all random points
// until all randomality in key generation is exhausted
// call prepare movie for all the movies
// close the enclave
// for all files in the library folder:
//   for each chunk in the file
//	   find the corresponding log file by the randomization in the chunk
//     xor it with 0s (as required by GCM definition) 
//     write the result to output file

void e5_crypto_extraction(char* server_ip, int iport, char* library_folder)
{
	if (!load_enclave())
	{
		printf("\n can not load enclave");
		return;
	}
	size_t * ids = nullptr;
	bool *is_payed = nullptr;
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


	
}
