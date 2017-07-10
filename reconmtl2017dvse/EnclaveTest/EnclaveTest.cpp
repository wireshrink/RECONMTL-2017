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

void Usage(char* appname)
{
	printf("\nUsage: %s library_folder server_ip port\n", appname);
}

extern sgx_enclave_id_t global_eid;

int main(int argc, char ** argv)
{
	if (argc < 4)
	{
		Usage(argv[0]);
		exit(1);
	}
	char* library_folder = argv[1];
	char* server_ip = argv[2];
	char* strport = argv[3];
	int iport = atoi(strport);

	// initializing sockets 
	WSADATA wsaData = { 0 };
	int iResult = 0;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		wprintf(L"WSAStartup failed: %d\n", iResult);
		return 1;
	}
	
	// exploit the bug of missing boundary checks in reading EPG
	// this allows to get the coupons for viewing cartoons where payment is required
	e0_exfiltrate_data	(server_ip, iport, library_folder);
	// exploit using strcmp for coupon comparison
	// this allows to get the coupons for viewing cartoons where payment is required
	e1_timing_attack	(server_ip, iport, library_folder);
	// exploit trusting fopen too much
	// this allows to extract any payed cartoon
	// when encklave thinks that it works with free one
	e2_file_substitution(server_ip, iport, library_folder);
	// exploit missing replay protection on the blob file
	// by claiming already known coupons, storing the blob aside, 
	// getting the movie, and returning the coupon back 
	e3_blob_reuse		(server_ip, iport, library_folder);
	// exploit trusting OS time too much
	// this allows to extract any already payed cartoon by any time
	// by returning the time close to the payment from the OS
	e4_time_substitution(server_ip, iport, library_folder);
	// exploit key material exaustion, substituted as 
	// mistake in random number generation type miscast,
	// encrypted log left by very unfortunate typo, and 
	// downloading the files before it is determined if 
	// user already payed for this.
	// This allows to decrypt any movie
	e5_crypto_extraction(server_ip, iport, library_folder);

    return 0;
}

