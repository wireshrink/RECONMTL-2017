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


#include "SGXFileWriter.h"
#include "DVSE_t.h" 
#include "SGXIndependentSealing.h"


// Constructors/Destructors
//  

SGXFileWriter::SGXFileWriter () {
}

SGXFileWriter::~SGXFileWriter () { }


bool SGXFileWriter::openMovie(char* base_folder, size_t movie_id)
{
	char filename[1024];
	this->createMovieFileName(movie_id, base_folder, filename, 1024);
	return open(filename);
}

bool SGXFileWriter::seek(size_t pos)
{
	return false;
}

bool SGXFileWriter::open(char filename[1024])
{
	void* handle = nullptr;
	sgx_status_t ret = ocall_file_open(&handle, filename, (char*)"wb");
	if (ret != SGX_SUCCESS)
		return false;
	if (handle == nullptr)
		return false;
	setHandle(handle);
	return true;
}
#define _MIN(a, b) (a)<(b)? (a):(b);
size_t SGXFileWriter::read(unsigned char * buffer, size_t buffer_size)
{
	return -1L;
}

size_t SGXFileWriter::write(unsigned char * buffer, size_t buffer_size)
{
	size_t b_pos = 0;
	int written = 0;
	// seling by chunks
	for (b_pos = 0; b_pos < buffer_size; b_pos += SGXIndependentSealing::UNSEALED_DATA_CHUNK_SIZE)
	{
		unsigned char *sealed_chunk = nullptr;
		int chunk_written = 0;
		size_t chunk_size = _MIN(buffer_size-b_pos, SGXIndependentSealing::UNSEALED_DATA_CHUNK_SIZE);
		size_t sealed_size;
		sgx_status_t ret;
		if (!SGXIndependentSealing::seal_data(buffer + b_pos, chunk_size , &sealed_chunk, &sealed_size))
		{
			return -1L;
		}
		ret = ocall_file_write(&chunk_written, getHandle(), sealed_size, sealed_chunk);
		SGXIndependentSealing::destroy_allocated_data(sealed_chunk);
		if (ret != SGX_SUCCESS)
			return -1L;
		if (chunk_written == -1)
			return -1L;
		written += chunk_written;

	}
	return written;
}

bool SGXFileWriter::close()
{
	sgx_status_t ret;
	int res;
	ret = ocall_file_close(&res, getHandle());
	setHandle(nullptr);
	return true;
}
