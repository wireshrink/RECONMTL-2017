#include "encryptedfile.h"

EncryptedFile::EncryptedFile(unsigned int movieId):QIODevice()
{
	this->movie_id = movieId;
}
