#ifndef ENCRYPTEDFILE_H
#define ENCRYPTEDFILE_H

#include <QObject>
#include <QFile>
#include "sgxware.h"

class EncryptedFile : public QFile
{
public:
	unsigned int movie_id;
    EncryptedFile(unsigned int movie_id);
protected:
	virtual qint64 readData(char*data, qint64 maxSize)
	{
		SGXware *pSGX = SGXware::getInstance();
		qint64 pos = this->pos();
		qint64 readDataSize = pSGX->readMovieChunk(movie_id, pos, maxSize, (unsigned char*)data);
		pSGX->inplaceDecrypt(readDataSize, data);
		QFile::seek(pos+readDataSize);
		return readDataSize;
	}
};

#endif // ENCRYPTEDFILE_H
