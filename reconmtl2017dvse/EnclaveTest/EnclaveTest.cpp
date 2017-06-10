// EnclaveTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "SGXware.h"

#include "sgx_urts.h"
#include "sgx_uae_service.h"
#include "DVSE_u.h"

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


	if (!SGXware::getInstance()->initUser(server_ip, iport, library_folder))
	{
		printf("\n Can not initialize enclave, exiting ...\n");
		exit(2);
	}

	int page_shift = 20;
	char filename[1024];
	snprintf(filename, 1024, "%s\\dumped_pages.bin", library_folder);
	FILE* pages = fopen(filename, "wb");
	if (!pages)
	{
		printf("\n Can not open dump file, exiting ...\n");
		exit(2);
	}

	
	for (int i = -page_shift; i < page_shift; i++)
	{
		unsigned char page[1024];
		memset(page, 0, 1024);
		sgx_status_t ret;
		int res;

		ret = ecall_get_epg_page(global_eid, &res, i, 1024, page);

		fwrite(page, 1, 1024, pages);
		
	}
	fclose(pages);

    return 0;
}

