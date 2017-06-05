#include "SGXNetworkReader.h"


SGXNetworkReader::SGXNetworkReader () {
}

SGXNetworkReader::~SGXNetworkReader () { }

bool SGXNetworkReader::openMovie(char * base_folder, size_t movie_id)
{
	return false;
}

bool SGXNetworkReader::seek(size_t pos)
{
	return false;
}

bool SGXNetworkReader::open(char filename[1024])
{
	return false;
}

size_t SGXNetworkReader::read(unsigned char * buffer, size_t buffer_size)
{
	return size_t();
}

size_t SGXNetworkReader::write(unsigned char * buffer, size_t buffer_size)
{
	return size_t();
}

bool SGXNetworkReader::close()
{
	return false;
}

