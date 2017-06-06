#include "SGXServiceFile.h"
#include "SGXSslWare.h"

// Constructors/Destructors
//  

SGXServiceFile::SGXServiceFile () {
initAttributes();
}

SGXServiceFile::~SGXServiceFile () { }

/**
* // copies a filename inside
* @return bool
* @param  filename filename inside of current filesystem
*/

bool SGXServiceFile::prepare(char * filename)
{
	strncpy(full_file_name, filename, 1024);
	return true;
}

/**
* @return bool
*/

bool SGXServiceFile::read_and_decrypt()
{
	void * f = nullptr;
	int retval;
	sgx_status_t ret = ocall_file_open(&f, full_file_name, "rb");
	if (ret != SGX_SUCCESS)
	{
		return false;
	}
	if (decrypted_content != nullptr)
	{
		free(decrypted_content);
		decrypted_content = nullptr;
	}

	if (f == nullptr)
	{
		return false;
	}
	size_t fsize = -1L;



	ret = ocall_file_size(&fsize, f);
	if (ret != SGX_SUCCESS)
	{
		return false;
	}
	current_data_size = fsize;
	unsigned char * encrypted_file_content = (unsigned char*)malloc(fsize);

	ret = ocall_file_read(&retval, f, 0, fsize, encrypted_file_content);
	if (ret != SGX_SUCCESS || retval != (int)fsize)
	{
		current_data_size = -1L;
		ocall_file_close(&retval, f);
		free(encrypted_file_content);
		return false;
	}

	if (!SGXIndependentSealing::unseal_data(encrypted_file_content, fsize, &decrypted_content, &this->current_data_size))
	{
		current_data_size = -1L;
		ocall_file_close(&retval, f);
		free(encrypted_file_content);
		return false;
	}
	free(encrypted_file_content);
	ret = ocall_file_close(&retval, f);
	return true;
}

/**
* @return bool
*/

bool SGXServiceFile::encrypt_and_save()
{
	void * f = nullptr;
	int retval;
	sgx_status_t ret = ocall_file_open(&f, full_file_name, "wb");
	if (ret != SGX_SUCCESS)
	{
		return false;
	}

	size_t encrypted_size;
	unsigned char * encrypted_file_content;

	if (!SGXIndependentSealing::seal_data(this->decrypted_content, this->current_data_size, &encrypted_file_content, &encrypted_size))
	{
		return false;
	}


	ret = ocall_file_write(&retval, f, encrypted_size, encrypted_file_content);

	if (ret != SGX_SUCCESS || retval != (int)encrypted_size)
	{
		current_data_size = -1L;
		ocall_file_close(&retval, f);
		free(encrypted_file_content);
		return false;
	}


	free(encrypted_file_content);
	ret = ocall_file_close(&retval, f);
	return true;
}

/**
* @return bool
* @param  data_length
* @param  data
*/

bool SGXServiceFile::set_decrypted_content(size_t data_length, unsigned char * data)
{
	unsigned char *ldec_cont = nullptr;
	ldec_cont = (unsigned char*)malloc(data_length);
	if (!ldec_cont)
	{
		return false;
	}
	if (decrypted_content)
	{
		free(decrypted_content);
	}
	memcpy(decrypted_content, data, data_length);
	current_data_size = data_length;
	return true;
}



bool SGXServiceFile::downloadById(char * id)
{
	SGXSslWare *psslw = SGXSslWare::getInstance();
	size_t datalen = strlen(id);
	unsigned char* temp;
	size_t temp_size;
	if (!psslw->reconnect())
	{
		return false;
	}
	if (!psslw->send((unsigned char*)&datalen, sizeof(datalen)))
	{
		psslw->shutdown();
		return false;
	}
	if (!psslw->send((unsigned char*)id, datalen))
	{
		psslw->shutdown();
		return false;
	}

	if (!psslw->receive((unsigned char*)&datalen, sizeof(datalen), &temp_size))
	{
		return false;
	}

	if (temp_size != sizeof(size_t))
	{
		return false;
	}

	temp = (unsigned char*)malloc(datalen);
	if (!temp)
	{
		return false;
	}

	if (!psslw->receive(temp, datalen, &temp_size))
	{
		free(temp);
		return false;
	}
	if (temp_size != datalen)
	{
		free(temp);
		return false;
	}

	if (this->decrypted_content != nullptr)
	{
		free(this->decrypted_content);
	}

	decrypted_content = temp;
	current_data_size = datalen;

	psslw->shutdown();
	return true;
}

void SGXServiceFile::initAttributes () {
  decrypted_content = nullptr;
  current_data_size = 0;
}

