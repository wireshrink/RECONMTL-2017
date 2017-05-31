#ifndef ENCRYPTEDFILE_H
#define ENCRYPTEDFILE_H

#include <QObject>
#include <QIODevice>
#include <QFile>
#include "sgxware.h"

class EncryptedFile : public QIODevice
{
public:
	unsigned int movie_id;
	size_t m_fsize;
    EncryptedFile(unsigned int movie_id);
	virtual bool open(QIODevice::OpenMode mode)
	{
		bool res = SGXware::getInstance()->prepareMovie(this->movie_id);
			
		return res && QIODevice::open(mode);
	}
	virtual void close()
	{
		if (m_testFile)
		{
			fclose(m_testFile);
			this->m_testFile = nullptr;
			m_fsize = -1;
		}
		return QIODevice::close();
	}
	
	virtual qint64 size() const
	{
	
		return m_fsize;
	}
	virtual bool seek(qint64 pos)
	{
		bool bq = QIODevice::seek(pos);
		bool res =  bq && fseek(m_testFile, pos, SEEK_SET) == 0;
		return res;
	}

	virtual bool isSequential() const
	{
		return false;
	}
	virtual bool reset()
	{
		return seek(0);
	}
	virtual qint64 bytesAvailable() const
	{
		qint64 const bA = QIODevice::bytesAvailable();
		return bA;
	};


protected:
	virtual qint64 readData(char*data, qint64 maxSize)
	{
		/*SGXware *pSGX = SGXware::getInstance();
		qint64 pos = this->pos();
		qint64 readDataSize = pSGX->readMovieChunk(movie_id, pos, maxSize, (unsigned char*)data);
		pSGX->inplaceDecrypt(readDataSize, data);
		QIODevice::seek(pos+readDataSize);
		return readDataSize;*/
		qint64 r = fread(data, 1, maxSize, this->m_testFile);//QIODevice::readData(data, maxSize);
		if (maxSize > 0)
			return r;
		return r;
	}

	virtual qint64 writeData(const char *data, qint64 len)
	{
		//return QIODevice::writeData(data, len); // no write at alll, reading only
		return 0;
	}



public:
	// only for test of qiodevice;
	bool setFileName(char* fname);
	FILE * m_testFile;
	char m_testfname[1024];
};

#endif // ENCRYPTEDFILE_H
