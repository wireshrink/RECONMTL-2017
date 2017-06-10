#include "SGXNetworkReader.h"
#include "SGXSslWare.h"


SGXNetworkReader::SGXNetworkReader () {
	file_size = 0L;
	rdptr = 0L;
}

SGXNetworkReader::~SGXNetworkReader () { }

bool SGXNetworkReader::openMovie(char * base_folder, size_t movie_id)
{
	char filename[1024];
	snprintf(filename, 1024, "%zd", movie_id);
	return open(filename);
}

bool SGXNetworkReader::seek(size_t pos)
{
	return false;
}

bool SGXNetworkReader::open(char filename[1024])
{
	size_t id_len, tmp_size, data_len;
	SGXSslWare * psslw = SGXSslWare::getInstance();
	if (!psslw->reconnect())
		return false;
	id_len = strlen(filename);
	if (!psslw->send((unsigned char*)&id_len, sizeof(size_t)))
	{
		psslw->shutdown();
		return false;
	}
	if (!psslw->send((unsigned char*)filename, id_len))
	{
		psslw->shutdown();
		return false;
	}

	if (!psslw->receive((unsigned char*)&data_len, sizeof(size_t), &tmp_size))
	{
		psslw->shutdown();
		return false;
	}
	if (tmp_size != sizeof(size_t))
	{
		psslw->shutdown();
		return false;
	}
	this->file_size = data_len;
	this->rdptr = 0;
	return true;
}

size_t SGXNetworkReader::read(unsigned char * buffer, size_t buffer_size)
{
	SGXSslWare * psslw = SGXSslWare::getInstance();
	size_t real_size;
	if ((file_size - rdptr) < buffer_size)
		buffer_size = (file_size - rdptr);

	if (!psslw->receive(buffer, buffer_size, &real_size))
	{
		psslw->shutdown();
		return false;
	}
	if (real_size != buffer_size)
	{
		psslw->shutdown();
		return false;
	}
	rdptr += buffer_size;
	return real_size;
}

size_t SGXNetworkReader::write(unsigned char * buffer, size_t buffer_size)
{
	return -1L;
}

bool SGXNetworkReader::close()
{
	SGXSslWare * psslw = SGXSslWare::getInstance();
	psslw->shutdown();
	return true;
}

