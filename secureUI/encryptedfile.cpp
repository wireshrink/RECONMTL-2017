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


#include "encryptedfile.h"

EncryptedFile::EncryptedFile(unsigned int movieId):QIODevice()
{
	this->movie_id = movieId;
	this->m_fsize = -1;
	m_testFile = nullptr;

}


// only for test of qiodevice, will be removed later
bool EncryptedFile::setFileName(char*fname)
{
	strcpy(m_testfname, fname);
	m_testFile = fopen(m_testfname, "rb");
	long fpos = ftell(m_testFile);
	fseek(m_testFile, 0, SEEK_END);
	long fsize = ftell(m_testFile);
	fseek(m_testFile, fpos, SEEK_SET);
	m_fsize = fsize;
	return true;
}
