/************************************************************************************************************
*	This application is a TRAINING TARGET for exercises in HACKING Intel® SGX ENCLAVES.                     *
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
*   the Intel® Software Guard Extensions SSL (Intel® SGX SSL) to be compiled.								*
*	This application is written by Michael Atlas (wireshrink@gmail.com) during 2017.						*
*	Happy hacking.																							*
*************************************************************************************************************/


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
		res = res && SGXware::getInstance()->getFileSize(this->movie_id, &this->m_fsize);
		res = res && QIODevice::open(mode);
		if (!res)
		{
			return false;
		}
		return res;
	}
	virtual void close()
	{
		if (m_testFile)
		{
			fclose(m_testFile);
			this->m_testFile = nullptr;
			m_fsize = -1;
		}
		QIODevice::close();
	}
	
	virtual qint64 size() const
	{
	
		return m_fsize;
	}
	virtual bool seek(qint64 pos)
	{
		bool bq = QIODevice::seek(pos);
		//bool res =  bq && fseek(m_testFile, pos, SEEK_SET) == 0;
		return bq; // nothing to do here, only read counts
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
		SGXware *pSGX = SGXware::getInstance();
		qint64 pos = this->pos();

		qint64 readDataSize = pSGX->readMovieChunk( pos, maxSize, (unsigned char*)data);
		return readDataSize;
	}

	virtual qint64 writeData(const char *data, qint64 len)
	{
		Q_UNUSED(data);
		Q_UNUSED(len);
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
