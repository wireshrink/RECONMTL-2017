#include "SGXSslWare.h"
#include "DVSE_t.h" 

// SHA1 Fingerprint=8C:8D:5C:F1:86:9C:9B:16:AA:D5:22:FB:5D:B5:09:31:9E:57:39:94
const unsigned char MANUAL_SERVER_SHA1_HASH[] = { 0x8C, 0x8D, 0x5C, 0xF1, 0x86, 0x9C, 0x9B, 0x16, 0xAA, 0xD5, 0x22, 0xFB, 0x5D, 0xB5, 0x09, 0x31, 0x9E, 0x57, 0x39, 0x94 };
// this code is heavily based on the sample from http://fm4dd.com/openssl/sslconnect.htm

SGXSslWare * SGXSslWare::m_pInstance = nullptr;
const char* const PREFERRED_CIPHERS = "HIGH:!aNULL:!kRSA:!SRP:!PSK:!CAMELLIA:!RC4:!MD5:!DSS";

SGXSslWare::SGXSslWare()
{
	server = nullptr;
}


SGXSslWare::~SGXSslWare()
{
}

void init_openssl_library(void)
{
	/* https://www.openssl.org/docs/ssl/SSL_library_init.html */
	(void)SSL_library_init();
	/* Cannot fail (always returns success) ??? */

	/* https://www.openssl.org/docs/crypto/ERR_load_crypto_strings.html */
	SSL_load_error_strings();
	/* Cannot fail ??? */
}

SGXSslWare * SGXSslWare::getInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new SGXSslWare();
		init_openssl_library();
	}

	return m_pInstance;
}

void SGXSslWare::destroy()
{
	SSL_free(ssl);
	SSL_CTX_free(ctx);
	if (m_pInstance) delete m_pInstance;
}

bool SGXSslWare::connect(char * address, int port)
{
	const SSL_METHOD *method;
	X509                *cert = NULL;
	/* ---------------------------------------------------------- *
	* These function calls initialize openssl for correct work.  *
	* ---------------------------------------------------------- */
	OpenSSL_add_all_algorithms();
	ERR_load_BIO_strings();
	ERR_load_crypto_strings();
	SSL_load_error_strings();

	/* ---------------------------------------------------------- *
	* initialize SSL library and register algorithms             *
	* ---------------------------------------------------------- */
	if (SSL_library_init() < 0)
		return false;

	/* ---------------------------------------------------------- *
	* Set SSLv2 client hello, also announce SSLv3 and TLSv1      *
	* ---------------------------------------------------------- */
	method = SSLv23_client_method();

	/* ---------------------------------------------------------- *
	* Try to create a new SSL context                            *
	* ---------------------------------------------------------- */
	if ((ctx = SSL_CTX_new(method)) == NULL)
		return false;

	/* ---------------------------------------------------------- *
	* Disabling SSLv2 will leave v3 and TSLv1 for negotiation    *
	* ---------------------------------------------------------- */
	SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);

	/* ---------------------------------------------------------- *
	* Create new SSL connection state object                     *
	* ---------------------------------------------------------- */
	ssl = SSL_new(ctx);

	/* ---------------------------------------------------------- *
	* Make the underlying TCP socket connection                  *
	* ---------------------------------------------------------- */
	sgx_status_t ret = ocall_socket_connect(&server, address, port);
	if (ret != SGX_SUCCESS)
	{
		return false;
	}
	if (server == 0)
		return false;

	/* ---------------------------------------------------------- *
	* Attach the SSL session to the socket descriptor            *
	* ---------------------------------------------------------- */
	SSL_set_fd(ssl, (int)server);

	/* ---------------------------------------------------------- *
	* Try to SSL-connect here, returns 1 for success             *
	* ---------------------------------------------------------- */
	if (SSL_connect(ssl) != 1)
		return false;

	/* ---------------------------------------------------------- *
	* Get the remote certificate into the X509 structure         *
	* ---------------------------------------------------------- */
	cert = SSL_get_peer_certificate(ssl);
	if (cert == NULL)
		return false;
	/* ---------------------------------------------------------- *
	* This is demo buggy app, no revocation handling reqauired    *
	* ---------------------------------------------------------- */

	if (memcmp(cert->sha1_hash, MANUAL_SERVER_SHA1_HASH, SHA_DIGEST_LENGTH))
	{
		if (cert) { X509_free(cert); }
		return false;
	}
	if (cert) { X509_free(cert); }

	strncpy(m_addr, address, 1024);
	m_port = port;
	return true;


}

bool SGXSslWare::reconnect()
{
	return connect(m_addr, m_port);
}

bool SGXSslWare::send(unsigned char * data, size_t size)
{
	return SSL_write(ssl, data, size) == size;
}

bool SGXSslWare::receive(unsigned char * data, size_t max_size, size_t * real_size)
{
	*real_size = SSL_read(ssl, data, max_size);
	return (*real_size != -1L);
}

bool SGXSslWare::shutdown()
{
	//ocall_socket_shutdown((int)server);
	ocall_socket_shutdown(server);
	return true;
}
