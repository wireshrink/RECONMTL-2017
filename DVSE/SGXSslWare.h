#pragma once
// SSL connection singleton

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>

#include <windows.h>
#include <sys/types.h>


#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/x509.h>
#include <openssl/buffer.h>
#include <openssl/x509v3.h>
#include <openssl/opensslconf.h>

class SGXSslWare
{
	static SGXSslWare * m_pInstance;

	SSL_CTX* ctx = NULL;
	//BIO *web = NULL, *out = NULL;
	SSL *ssl = NULL;
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

