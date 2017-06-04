#include "SGXFileReader.h"
#include "DVSE_t.h" 

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
	sgx_status_t ret = ocall_file_open(&handle, filename, "rb");
	if (ret != SGX_SUCCESS)
		return false;
	if (handle == nullptr)
		return false;
	setHandle(handle);
	m_pos = 0;
	return true;
}

size_t SGXFileReader::read(unsigned char * buffer, size_t buffer_size)
{
	return size_t();
}

size_t SGXFileReader::write(unsigned char * buffer, size_t buffer_size)
{
	return -1L;
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

//  
// Methods
//  


// Accessor methods
//  


// Other methods
//  


