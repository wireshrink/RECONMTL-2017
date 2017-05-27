#include "utils.h"
#include "DVSE_t.h"
#include "SGXIndependentSealing.h"
#include "sgx_trts.h"
#include <string.h>
#include "SGXPlayerState.h"


bool read_whole_file(char * filename, size_t * pfilesize, unsigned char ** data)
{
	void *f = nullptr;
	sgx_status_t ret = ocall_file_open(&f, filename, "rb");


	if (ret != SGX_SUCCESS)
	{
		return false;
	}
	if (f == nullptr)
	{
		return false;
	}


	size_t file_data_size;
	ret = ocall_file_size(&file_data_size, filename);
	if (ret != SGX_SUCCESS)
	{
		return false;
	}


	return false;
}

bool destroy_read_data(unsigned char * data)
{
	return false;
}
