
#include "stdafx.h"
#include "exploits.h"
#include "common_enclave_actions.h"

void e5_crypto_extraction(char* server_ip, int iport, char* library_folder)
{
	// create an enclave
	//  do
	//   write log 0s of 1024 byte lebgth (movie chunk size)
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
}
