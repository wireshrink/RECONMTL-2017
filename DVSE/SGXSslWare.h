/************************************************************************************************************
*	This application is a TRAINING TARGET for exercises in HACKING Intel� SGX ENCLAVES.                     *
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
*   the Intel� Software Guard Extensions SSL (Intel� SGX SSL) to be compiled.								*
*	This application is written by Michael Atlas (wireshrink@gmail.com) during 2017.						*
*	Happy hacking.																							*
*************************************************************************************************************/


#pragma once
// SSL connection singleton

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#ifdef _MSC_VER
#include <windows.h>

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/x509.h>
#include <openssl/buffer.h>
#include <openssl/x509v3.h>
#include <openssl/opensslconf.h>
#endif
#include <sys/types.h>



class SGXSslWare
{
	static SGXSslWare * m_pInstance;
#ifdef _MSC_VER
	SSL_CTX* ctx = NULL;
	//BIO *web = NULL, *out = NULL;
	SSL *ssl = NULL;
#endif
	void* server;
	char m_addr[1024];
	unsigned int m_port;

	SGXSslWare();
	virtual ~SGXSslWare();
public:
	static SGXSslWare *getInstance();
	void destroy();
	bool connect(char* address, int port);
	bool reconnect();
	bool send(unsigned char* data, size_t size);
	bool receive(unsigned char* data, size_t max_size, size_t *real_size);
	bool shutdown();
};

