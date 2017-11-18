/************************************************************************************************************
*	This application is a TRAINING TARGET for exercises in HACKING Intel� SGX ENCLAVES.                     *
*	This application made vulnerable DELIBERATELY - its main purpose is to demonstrate, shame and blame     *
*   common mistakes usually made with SGX enclave programming.                                              *
*   ONCE AGAIN, IT CONTAINS MISTAKES.                                                                       *
*   Please DO NOT USE it's source for any healthy development/production activity.                          *
*	It is intended to use for training of hacking SGX enclaves ONLY.                                        *
*	It is written ugly(deliberately), designed badly(intentionally) and running very slow.		        	*
*	You can freely use this application for training purposes.												*
*	I'd be glad to hear about your progress.    															*
*																											*
*	This application requires QT5.8 (which uses LGPL v3 license), Intel SGX SDK and							*
*   the Intel� Software Guard Extensions SSL (Intel� SGX SSL) to be compiled.								*
*	This application is written by Michael Atlas (wireshrink@gmail.com) during 2017.						*
*	Happy hacking.																							*
*************************************************************************************************************/


#include "SGXServiceFile.h"
#include "SGXSslWare.h"


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
	sgx_status_t ret = ocall_file_open(&f, full_file_name, (char*)"rb");
	if (ret != SGX_SUCCESS)
	{
		return false;
	}

	if (f == nullptr)
	{
		return false;
	}
	size_t fsize = -1L;



	ret = ocall_file_size(&fsize, f);
	if (ret != SGX_SUCCESS || fsize == 0 || fsize == -1L)
	{
		return false;
	}
	if (decrypted_content != nullptr)
	{
		free(decrypted_content);
		decrypted_content = nullptr;
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
	sgx_status_t ret = ocall_file_open(&f, full_file_name, (char*)"wb");
	if (ret != SGX_SUCCESS)
	{
		return false;
	}

	size_t encrypted_size = 0L;
	unsigned char * encrypted_file_content = nullptr;

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


	ret = ocall_file_close(&retval, f);
	free(encrypted_file_content);
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
	if (data == decrypted_content)
	{
		return false; // setting the same memory twice
	}
	ldec_cont = (unsigned char*)malloc(data_length);
	if (!ldec_cont)
	{
		return false;
	}
	if (decrypted_content)
	{
		free(decrypted_content);
	}
	memcpy(ldec_cont, data, data_length);
	this->decrypted_content = ldec_cont;
	current_data_size = data_length;
	return true;
}



bool SGXServiceFile::downloadById(const char * id)
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

