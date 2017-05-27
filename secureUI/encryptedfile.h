#ifndef ENCRYPTEDFILE_H
#define ENCRYPTEDFILE_H

#include <QObject>
#include <QIODevice>
#include "sgxware.h"

class EncryptedFile : public QIODevice
{
public:
	unsigned int movie_id;
    EncryptedFile(unsigned int movie_id);
	virtual bool open(QIODevice::OpenMode mode)
	{
		return false;
	}
	virtual void close()
	{
		return;
	}
	virtual qint64 pos()
	{
		return 0;
	}
	virtual qint64 size()
	{
		return 0;
	}
	virtual bool seek(qint64 pos)
	{
		return false;
	}
	virtual bool atEnd()
	{
		return pos() == size();
	}

	virtual bool isSequential()
	{
		return false;
	}
	virtual bool reset()
	{
		return seek(0);
	}
	virtual qint64 bytesAvailable()
	{
		return size() - pos();
	};

	virtual qint64 bytesToWrite()
	{
		return 0;
	}

	virtual bool canReadLine()
	{
		return false;
	};
	virtual bool waitForReadyRead(int msecs)
	{
		// always ready
		return true;
	}
	virtual bool waitForBytesWritten(int msecs)
	{
		return false; // no write no cry
	}

protected:
	virtual qint64 readData(char*data, qint64 maxSize)
	{
		SGXware *pSGX = SGXware::getInstance();
		qint64 pos = this->pos();
		qint64 readDataSize = pSGX->readMovieChunk(movie_id, pos, maxSize, (unsigned char*)data);
		pSGX->inplaceDecrypt(readDataSize, data);
		QIODevice::seek(pos+readDataSize);
		return readDataSize;
	}
	virtual qint64 readLineData(char *data, qint64 maxlen)
	{
		return 0;
	}

	virtual qint64 writeData(const char *data, qint64 len)
	{
		return 0; // no write at alll, reading only
	}



};

#endif // ENCRYPTEDFILE_H
