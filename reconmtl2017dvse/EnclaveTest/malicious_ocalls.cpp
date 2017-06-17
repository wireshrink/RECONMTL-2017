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
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1


#include "DVSE_u.h"
#include <stdio.h>
#include <time.h>

#pragma comment(lib,"Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>

#include <sys/types.h>
#include <sys/timeb.h>

void* ocall_file_open(/*[in, out, string] */char* file_name,
	/*[in, out, string] */char* format)
{
	printf("\nOpening file %s with miodifiers %s ... ", file_name, format);
	FILE* f = fopen(file_name, format);
	printf("returns %p\n", f);
	return (void*)f;
}

int ocall_file_close(/*[user_check]*/void* handle)
{
	printf("\nClosing handle %p\n", handle);
	return fclose((FILE*)handle);
}

int ocall_file_read(void *handle, size_t offset, size_t size, /*[in, out]*/unsigned char *data)
{
	printf("\n Reading from handle %p by offset %zx with size %zx into %p\n", handle, offset, size, data);
	size_t pos = ftell((FILE*)handle);
	if (pos != offset)
	{
		fseek((FILE*)handle, (long)offset, SEEK_SET);
	}
	return (int)fread(data, 1, size, (FILE*)handle);
}

int ocall_file_write(/*[user_check]*/void *handle, size_t size, /*[in, out]*/unsigned char *data)
{
	printf("\n Writing to handle %p with size %zx into %p\n", handle,  size, data);
	return (int)fwrite(data, 1, size, (FILE*)handle);
}

SOCKET s = INVALID_SOCKET;

void* ocall_socket_connect(/*[in, string]*/char *url, unsigned int port)
{
	struct sockaddr_in server;
	printf("\nConnecting to %s on port %d\n", url, port);
	s = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_addr.s_addr = inet_addr(url);
	server.sin_family = AF_INET;
	server.sin_port = htons((unsigned short)port);
	if (connect(s, (struct sockaddr*) &server, sizeof(server)) < 0)
	{
		unsigned int err = GetLastError();
		(void)err; // I just want to see it during debug session, not more
		return 0;
	}
	DWORD timeout = SO_RCVTIMEO * 1000, tlen = 4;
	getsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, (int*)&tlen);
	timeout *= 10;
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
	return (void*)s;
}
void ocall_socket_shutdown(void * socket)
{
	printf("\nShutting down socket %p from the enclave\n", socket);
	shutdown((SOCKET)socket, SD_BOTH);
	s = INVALID_SOCKET;
}
int ocall_get_the_current_time(unsigned char time_holder[16])
{
	printf("\nGetting time_t from the enclave ...\n");
	time((time_t*)time_holder);
	return 0;
}
size_t ocall_file_size(void* handle)
{
	printf("\nGetting a file size for handle %p ", handle);
	if (handle == nullptr)
	{
		printf(" = -1\n");
		return -1L;
	}
	size_t pos = ftell((FILE*)handle);
	fseek((FILE*)handle, 0, SEEK_END);
	size_t sz = ftell((FILE*)handle);
	fseek((FILE*)handle, (long)pos, SEEK_SET); 
	printf("= %zx\n", sz);
	return sz;
}

#pragma warning(disable:4311)
#pragma warning(disable:4302)

void u_sgxssl_ftime64( void * timeptr, uint32_t timeb64Len)
{
	printf("\nGetting the time by the openssl as __timeb64 ...\n");
	_ftime64((__timeb64*)timeptr);
}


int u_sgxssl_closesocket( void* s,  int* wsaError)
{
	int res = closesocket((int)s);
	printf("\nClosing socket %p from OpenSSL\n", s);
	*wsaError = WSAGetLastError();
	return res;
}
int u_sgxssl_recv(void* s, void* buf, int len, int flag, int* wsaError)
{
	printf("\nrecv from openSSL: handle %p buf %p len %d flags %d \n", s, buf, len, flag);
	int res = recv((int)s, (char*)buf, len, flag);
	*wsaError = WSAGetLastError();
	return res;
}
int u_sgxssl_send(void* s, const char* buf, int len, int flags, int* wsaError)
{
	printf("\nsend from openSSL: handle %p buf %p len %d flags %d \n", s, buf, len, flags);
	int res = send((int)s, buf, len, flags);
	*wsaError = WSAGetLastError();
	return res;
}
int u_sgxssl_shutdown(void* s, int how, int* wsaError)
{
	int res = shutdown((int)s, how);
	printf("\nshutting down socket from openssl:handle %p\n", s);
	*wsaError = WSAGetLastError();
	return res;

}
