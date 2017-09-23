/************************************************************************************************************
*	This application is a TRAINING TARGET for exercises in HACKING Intel® SGX ENCLAVES.                     *
*	This application made vulnerable DELIBERATELY - its main purpose is to demonstrate, shame and blame     *
*   common mistakes usually made with SGX enclave programming.                                              *
*   ONCE AGAIN, IT CONTAINS MISTAKES.                                                                       *
*   Please DO NOT USE it's source for any healthy development/production activity.                          *
*	It is intended to use for training of hacking SGX enclaves ONLY.                                        *
*	It is written ugly(deliberately), designed badly(intentionally) and running very slow.		        	*
*	You can freely use this application for training purposes.												*
*	I'd be glad to hear about your progress.    															*
*																											*
*	This application requires QT5.8 (which uses LGPL v3 license), Intel SGX SDK and							*
*   the Intel® Software Guard Extensions SSL (Intel® SGX SSL) to be compiled.								*
*	This application is written by Michael Atlas (wireshrink@gmail.com) during 2017.						*
*	Happy hacking.																							*
*************************************************************************************************************/


// EnclaveTest.cpp : Enclave exploiot template project
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

#include "exploits.h"

typedef enum __enclave_action__
{
	EA_MEMORY_EXFILTRATION,
	EA_TIMING, 
	EA_FILE_SUBSTITUTION,
	EA_BLOB_REUSE, 
	EA_TIME_SUBSTITUTIUON,
	EA_CRYPTO_PREGENERATE_LOGS,
	EA_CRYPTO_PREGENERATE_COUPONS, 
	EA_CRYPTO_DECRYT_COUPONS,
	EA_LAST
}enclave_action_t;

typedef struct __enclave_action_def__
{
	enclave_action_t act;
	char* clname;
	char* descr;
}enclave_action_def_t;

enclave_action_def_t actions[] = {
	{	EA_MEMORY_EXFILTRATION ,
		"exfiltrate",
		"exfiltrate memory from the enclave by bounds checking mitake "},
#ifndef _MSC_VER
	{	EA_TIMING ,
		"timing",
		"exploit strcmp in coupons comparison (Linux only)" },
#endif
	{	EA_FILE_SUBSTITUTION ,
		"filesubst",
		"read media by making enclave think that it reads free to view movie" },
	{	EA_BLOB_REUSE ,
		"blobreuse",
		"read the media by reusing coupons" },
	{	EA_TIME_SUBSTITUTIUON ,
		"timesubst",
		"read media by replacing the time to allowed one" },
	{	EA_CRYPTO_PREGENERATE_COUPONS ,
		"gencoupons",
		"generate a lot of coupon files [file to append to]" },
	{	EA_CRYPTO_PREGENERATE_LOGS ,
		"genlogs",
		"generate known plaintext [file to create, size in chunks]" },
	{	EA_CRYPTO_DECRYT_COUPONS ,
		"decryptcoupons",
		"(may run in parallel, doesn't use actual anclave capabilities) compare the generated coupons against gerenated logs and possibly decrypt coupons [coupon_file pregen_file ]" },
	{	EA_LAST, 
		"",
		""
	}

};
enclave_action_t str2act(char* str)
{
	int i = 0;
	while (actions[i].act != EA_LAST)
	{
		if (strcmp(actions[i].clname, str) == 0)
		{
			return actions[i].act;
		}
		i++;
	}
	return EA_LAST;
}
void Usage(int argc, char **argv)
{
	int i = 0;

	printf("\n command line arguments: \n");
	for (i = 0; i < argc; i++)
	{
		printf("\t%s\n", argv[i]);
	}
	printf("\nUsage: %s library_folder server_ip port [action [action params]]", argv[0]);
	i = 0;
	while (actions[i].act != EA_LAST)
	{
		printf("\n\t%s - ", actions[i].clname);
		printf("\n\t\t%s - ", actions[i].descr);
		i++;
	}
}

size_t arg2sizet(char*param)
{
	size_t s = 0xffffffffffffffff;
	sscanf(param, "%zd", &s);
	return s;
}
extern sgx_enclave_id_t global_eid;

int main(int argc, char ** argv)
{
	if (argc < 5)
	{
		Usage(argc, argv);
		exit(1);
	}
	char* library_folder = argv[1];
	char* server_ip = argv[2];
	char* strport = argv[3];
	int iport = atoi(strport);
	enclave_action_t act = str2act(argv[4]);
	if (act == EA_LAST)
	{
		Usage(argc, argv);
		exit(1);
	}

	// initializing sockets 
	WSADATA wsaData = { 0 };
	int iResult = 0;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		wprintf(L"WSAStartup failed: %d\n", iResult);
		return 1;
	}

	switch (act)
	{
	case EA_MEMORY_EXFILTRATION:
		// exploit the bug of missing boundary checks in reading EPG
		// this allows to get the coupons for viewing cartoons where payment is required
		e0_exfiltrate_data(server_ip, iport, library_folder);
		break;
	case EA_TIMING:
#ifndef _MSC_VER
		// exploit using strcmp for coupon comparison
		// this allows to get the coupons for viewing cartoons where payment is required
		// NOt practical on Windows because of optimized SSE2 based strncmp 
		// implementation. Linux, however, should work
		// current implementation is nit tested on Linux yet
		e1_timing_attack(server_ip, iport, library_folder);
#endif
		break;
	case EA_FILE_SUBSTITUTION:
		// exploit trusting fopen too much
		// this allows to extract any payed cartoon
		// when enclave thinks that it works with free one
		e2_file_substitution(server_ip, iport, library_folder);
		break;
	case EA_BLOB_REUSE:
		// exploit missing replay protection on the blob file
		// by claiming already known coupons, storing the blob aside, 
		// getting the movie, and returning the coupon back 
		e3_blob_reuse(server_ip, iport, library_folder);
		break;
	case EA_TIME_SUBSTITUTIUON:
		// exploit trusting OS time too much
		// this allows to extract any already payed cartoon by any time
		// by returning the time close to the payment from the OS
		e4_time_substitution(server_ip, iport, library_folder);		
		break;
	case EA_CRYPTO_PREGENERATE_LOGS:
		e5_pregenerate_logs(server_ip, iport, library_folder, argv[5], arg2sizet(argv[6]));
		break;
	case EA_CRYPTO_PREGENERATE_COUPONS:
		e5_pregenerate_coupons(server_ip, iport, library_folder, argv[5]);
		break;
	case EA_CRYPTO_DECRYT_COUPONS:
		// exploit key material exaustion, substituted as 
		// mistake in random number generation type miscast,
		// encrypted log left by very unfortunate typo, and 
		// downloading the files before it is determined if 
		// user already payed for this.
		// This allows to decrypt any movie
		e5_crypto_extraction(server_ip, iport, library_folder, argv[5], argv[6]);
		break;
	default:
		break;
	}
	//printf("\nPress any key to continue ...");
	//getchar();

    return 0;
}

