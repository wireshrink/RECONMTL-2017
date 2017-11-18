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


#include "SGXFileReader.h"
#include "DVSE_t.h" 
#include "SGXIndependentSealing.h"

// Constructors/Destructors
//  

SGXFileReader::SGXFileReader () {
}

SGXFileReader::~SGXFileReader () { }

bool SGXFileReader::openMovie(char* base_folder, size_t movie_id)
{
	char filename[1024];
	this->createMovieFileName(movie_id, base_folder, filename, 1024);
	return open(filename);
}

bool SGXFileReader::seek(size_t pos)
{
	m_pos = pos;
	return true;
}

bool SGXFileReader::open(char filename[1024])
{
	void* handle = nullptr;
	sgx_status_t ret = ocall_file_open(&handle, filename, (char*)"rb");
	if (ret != SGX_SUCCESS)
		return false;
	if (handle == nullptr)
		return false;
	setHandle(handle);
	m_pos = 0;
	m_binitialized = true;
	return true;
}
#define _MIN(a, b) (a)<(b)? (a):(b);
size_t SGXFileReader::read(unsigned char * buffer, size_t buffer_size)
{
	// calculating real position in encrypted file
	size_t enc_pos = SGXIndependentSealing::calc_sealed_data_size(m_pos);
	size_t current_chunk_pos = (enc_pos / SGXIndependentSealing::SEALED_DATA_CHUNK_SIZE) * SGXIndependentSealing::SEALED_DATA_CHUNK_SIZE;
	size_t read_data = 0;
	size_t chunk_read_data = 0;
	unsigned char sealed_chunk[SGXIndependentSealing::SEALED_DATA_CHUNK_SIZE];
	unsigned char *unsealed = nullptr;
	size_t        unsealed_size = 0;
	int retval;
	do
	{
		//reading the current chunk
		unsealed_size = 0;
		memset(sealed_chunk, 0, SGXIndependentSealing::SEALED_DATA_CHUNK_SIZE);
		sgx_status_t ret = ocall_file_read(&retval, getHandle(), current_chunk_pos, SGXIndependentSealing::SEALED_DATA_CHUNK_SIZE, sealed_chunk);
		if (ret != SGX_SUCCESS)
			return -1L;
		if (retval <= 0)
		{
			return -1L;
		}
		if (!SGXIndependentSealing::unseal_data(sealed_chunk, retval, &unsealed, &unsealed_size))
			return -1L;
		size_t off_in_chunk = (m_pos % SGXIndependentSealing::UNSEALED_DATA_CHUNK_SIZE);
		size_t size_in_chunk = SGXIndependentSealing::UNSEALED_DATA_CHUNK_SIZE - off_in_chunk;
		if (size_in_chunk > (buffer_size - read_data))
		{
			size_in_chunk = buffer_size - read_data; // avoid ovwewriting the tail of the buffer
		}
		memcpy(buffer + read_data, unsealed + off_in_chunk, size_in_chunk);
		read_data += size_in_chunk;
		m_pos += size_in_chunk;
		
		enc_pos = SGXIndependentSealing::calc_sealed_data_size(m_pos);
		current_chunk_pos = (enc_pos / SGXIndependentSealing::SEALED_DATA_CHUNK_SIZE) * SGXIndependentSealing::SEALED_DATA_CHUNK_SIZE;
		memset(unsealed, 0, SGXIndependentSealing::UNSEALED_DATA_CHUNK_SIZE);
		SGXIndependentSealing::destroy_allocated_data(unsealed);
	} while (retval != 0 && read_data < buffer_size);
	return read_data;
}

size_t SGXFileReader::write(unsigned char * buffer, size_t buffer_size)
{
	return -1L; // no write no cry
}

bool SGXFileReader::close()
{
	sgx_status_t ret;
	int res;
	ret = ocall_file_close(&res, getHandle());
	m_pos = 0;
	setHandle(nullptr);
	return true;
}
