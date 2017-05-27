#include "EncryptedFileReader.h"
#include "SGXIndependentSealing.h"
#include "DVSE_t.h"


EncryptedFileReader::EncryptedFileReader()
{
	f = nullptr;
	this->sliding_window_offset = 0;
}


EncryptedFileReader::~EncryptedFileReader()
{
	if (f != nullptr)
	{
		// close the file here
		int retval;
		ocall_file_close(&retval, f);
	}
}

bool EncryptedFileReader::open(char * filename, char * format)
{
	sgx_status_t ret;
	int res;

	ret = ocall_file_open(&this->f, filename, format);
	if (ret != SGX_SUCCESS)
	{
		return false;
	}
	if (this->f == nullptr)
	{
		return false;
	}
	size_t size;
	ret = ocall_file_size(&this->filesize, filename);
	if (ret != SGX_SUCCESS)
	{
		return false;
	}
	return true;
}

bool EncryptedFileReader::setpos(size_t pos)
{

	return false;
}

bool EncryptedFileReader::read_decrypted_data(size_t pos, unsigned char * buffer, size_t size)
{
	if (this->f == nullptr)
	{
		return false;
	}
	if (!sync_sliding_window(pos))
	{
		return false;
	}
	size_t enc_size_to_read;
	return false;
}

bool EncryptedFileReader::sync_sliding_window(size_t pos)
{
	return false;
}
