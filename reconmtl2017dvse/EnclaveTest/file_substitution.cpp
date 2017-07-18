
#include "stdafx.h"
#include "exploits.h"
#include "common_enclave_actions.h"


void e2_file_substitution(char* server_ip, int iport, char* library_folder)
{
	// create an enclave
	if (!load_enclave())
	{
		printf("\nCan not load enclave ...");
		return;
	}
	// get an epg
	size_t * ids = nullptr;
	bool *is_payed = nullptr;
	unsigned int num_of_movies = 0;
	if (!init_enclave(server_ip, library_folder, iport))
	{
		printf("\nCan not initialize enclave for getting EPG...");
		return;
	}
	if (!get_epg_info( &ids, &is_payed, &num_of_movies))
	{
		printf("\nCan not get EPG info. Is your server running ?");
		return;
	}
	// close enclave
	if (!unload_enclave())
	{
		printf("\nSomethign wrong, can not unload enclave.");
		return;
	}
	// for all payed movies
	unsigned int i;
	for (i = 0; i < num_of_movies; i++)
	{
		//	create an enclave
		if (!load_enclave())
		{
			printf("\nCan not load enclave for movie %zd ...", ids[i]);
			return;
		}
		if (!init_enclave(server_ip, library_folder, iport))
		{
			printf("\nCan not initialize enclave for movie %zd ...", ids[i]);
			return;
		}
		//  try to play payed movie in order to cause its download
		if (!prepare_file(ids[i]))
		{
			printf("\nCan not download encrypted movie. Is your server running ?");
			return;
		}
		//  configure file name substitution
		char filename[1024];
		snprintf(filename, 1024, "%s\\movie.%zd", library_folder, ids[i]);
		char filename_out[1024];
		snprintf(filename_out, 1024, "%s\\movie.decrypted.%zd", library_folder, ids[i]);
		substitute_file_name(true, (unsigned char*)filename);
		//  read as free played file and store
		extract_semi_allowed_file(ids[i], filename_out);
		//  close enclave
		if (!unload_enclave())
		{
			printf("\nunload enclave. Is your server running ?");
			return;
		}
	}
	substitute_file_name(false, nullptr);
}
