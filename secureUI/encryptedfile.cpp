#include "encryptedfile.h"

EncryptedFile::EncryptedFile(unsigned int movieId):QFile()
{
	this->movie_id = movieId;
}
