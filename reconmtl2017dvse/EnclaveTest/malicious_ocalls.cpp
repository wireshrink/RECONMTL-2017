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
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1


#include "DVSE_u.h"
#include <stdio.h>
#include <time.h>
#include <errno.h>

#ifdef _MSC_VER
	#pragma comment(lib,"Ws2_32.lib")
	#include <winsock2.h>
	#include <ws2tcpip.h>
#else
       #include <sys/types.h>
       #include <sys/socket.h>
	   #include <netinet/in.h>
	   #include <arpa/inet.h>
	   #include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/timeb.h>

#include "exploits.h"

// this fuinction is used to configure current time value substitution in the corresponding OCALL
// if the first parameter is set as true, the corresponding fake value will be 
// returned from ocall instead of the current one
bool g_substitute_time = false;
unsigned char substituted_time[16];
void substitute_time(bool substitute, unsigned char *fake_time)
{
	g_substitute_time = substitute;
	memcpy(substituted_time, fake_time, 16);
}
// this fuinction is used to configure file name value substitution in the corresponding OCALL
// if the first parameter is set as true, the corresponding fake value will be 
// used in ocall instead of passed from enclave
bool g_substitute_file = false;
char substituted_file_name[1024];
void substitute_file_name(bool substitute, unsigned char *fake_file_name)
{
	g_substitute_file = substitute;
	if (substitute) strncpy(substituted_file_name, (const char*)fake_file_name, 1024);
}
bool g_substitute_format = false;
char substituted_format[1024];
void substitute_format(bool substitute, unsigned char *fake_format)
{
	g_substitute_format = substitute;
	if (substitute) strncpy(substituted_format, (const char*)fake_format, 1024);
}

bool g_print_mode = true;
void set_print_mode(bool mode)
{
	g_print_mode = mode;
}

bool g_write_to_mem = false;
FILE * memHandle = nullptr;

void set_write_mode(bool to_mem, char* filename)
{
	if (!to_mem && memHandle)
	{
		fclose(memHandle);
		memHandle = nullptr;
	}
	if (to_mem && !memHandle)
	{
		memHandle = fopen(filename, "wb");
	}
	g_write_to_mem = to_mem;
}

FILE* last_opened_file = nullptr;

void close_last_opened_file()
{
	if (!g_write_to_mem)
	{
		if (last_opened_file)
		{
			fclose(last_opened_file);
		}
		last_opened_file = nullptr;
	}
}

#define printf if(g_print_mode)printf

bool is_service_file(char *fname)
{
	const char* fnames[] = {
		"blob.enc", 
		"coupon.enc", 
		"epg.enc", 
		nullptr
	};
	int i = 0;
	size_t length = strlen(fname);
	while (fnames[i] != nullptr)
	{
		size_t base = length - strlen(fnames[i]);
		if (strncmp(fnames[i], fname + base, 1024) == 0)
		{
			return true;
		}
		i++;
	}
	return false;
}

void* ocall_file_open(/*[in, out, string] */char* file_name,
	/*[in, out, string] */char* format)
{
	FILE *f = nullptr;
	if (g_write_to_mem) 
		return memHandle;
	printf("\nOpening file %s with miodifiers %s ... ", file_name, format);
	if (g_substitute_file && !is_service_file(file_name))
	{
		printf("\nChanging the file name on the fly to %s ", substituted_file_name);
		f = fopen(substituted_file_name, g_substitute_format?substituted_format: format);
	}
	else
	{
		f = fopen(file_name, g_substitute_format ? substituted_format : format);
	}
	printf("returns %p\n", f);
	if (!is_service_file(file_name))
	{
		last_opened_file = f;
	}
	return (void*)f;
}

int ocall_file_close(/*[user_check]*/void* handle)
{
	if (g_write_to_mem) 
		return 0;
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

#ifndef _MSC_VER
typedef int SOCKET;
#define INVALID_SOCKET -1
#define SD_BOTH SHUT_RDWR
#endif
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
		return 0;
	}
	uint32_t timeout = SO_RCVTIMEO * 1000;
	socklen_t tlen = 4;
	getsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (void*)&timeout, &tlen);
	timeout *= 10;
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
	return (void*)(uintptr_t)s;
}
void ocall_socket_shutdown(void * socket)
{

	printf("\nShutting down socket %p from the enclave\n", socket);
	shutdown((uintptr_t )socket, SD_BOTH);
	s = INVALID_SOCKET;
}
int ocall_get_the_current_time(unsigned char time_holder[16])
{
	printf("\nGetting time_t from the enclave ...\n");
	memset(time_holder, 0, 16);
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


#ifndef _MSC_VER
// compatibility 
int WSAGetLastError(void)
{
	return errno;
}
int closesocket(void* fd)
{
	return close((uintptr_t)fd);
}

void u_sgxssl_ftime( void * timeptr, uint32_t timeb64Len)
{
	printf("\nGetting the time by the openssl as __timeb64 ...\n");
	ftime((timeb*)timeptr);
}

#else

void u_sgxssl_ftime64( void * timeptr, uint32_t timeb64Len)
{
	printf("\nGetting the time by the openssl as __timeb64 ...\n");
#ifdef _MSC_VER
	_ftime64((__timeb64*)timeptr);
#else
	ftime((timeb*)timeptr);
#endif
}

#endif

int u_sgxssl_closesocket( void* s,  int* wsaError)
{
	int res = closesocket(s);
	printf("\nClosing socket %p from OpenSSL\n", s);
	*wsaError = WSAGetLastError();
	return res;
}
int u_sgxssl_recv(void* s, void* buf, int len, int flag, int* wsaError)
{
	printf("\nrecv from openSSL: handle %p buf %p len %d flags %d \n", s, buf, len, flag);
	int res = recv((uintptr_t)s, (char*)buf, len, flag);
	*wsaError = WSAGetLastError();
	return res;
}
int u_sgxssl_send(void* s, const char* buf, int len, int flags, int* wsaError)
{
	printf("\nsend from openSSL: handle %p buf %p len %d flags %d \n", s, buf, len, flags);
	int res = send((uintptr_t)s, buf, len, flags);
	*wsaError = WSAGetLastError();
	return res;
}
int u_sgxssl_shutdown(void* s, int how, int* wsaError)
{
	int res = shutdown((uintptr_t)s, how);
	printf("\nshutting down socket from openssl:handle %p\n", s);
	*wsaError = WSAGetLastError();
	return res;

}
