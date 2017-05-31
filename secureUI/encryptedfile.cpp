#include "encryptedfile.h"

EncryptedFile::EncryptedFile(unsigned int movieId):QIODevice()
{
	this->movie_id = movieId;
	this->m_fsize = -1;
	m_testFile = nullptr;

}


// only for test of qiodevice
bool EncryptedFile::setFileName(char*fname)
{
	strcpy(m_testfname, fname);
	m_testFile = fopen(m_testfname, "rb");
	size_t fpos = ftell(m_testFile);
	fseek(m_testFile, 0, SEEK_END);
	size_t fsize = ftell(m_testFile);
	fseek(m_testFile, fpos, SEEK_SET);
	m_fsize = fsize;
	return true;
}
