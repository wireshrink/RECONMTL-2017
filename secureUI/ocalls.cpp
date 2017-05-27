#include "DVSE_u.h"
#include <stdio.h>

void* ocall_file_open(/*[in, out, string] */char* file_name,
	/*[in, out, string] */char* format)
{

	return (void*)fopen(file_name, format);
}

int ocall_file_close(/*[user_check]*/void* handle)
{
	return fclose((FILE*)handle);
}

int ocall_file_read(void *handle, size_t offset, size_t size, /*[in, out]*/unsigned char *data)
{

	return 0;
}

int ocall_file_write(/*[user_check]*/void *handle, size_t offset, size_t size, /*[in, out]*/unsigned char *data)
{
	return 0;
}

int ocall_socket_connect(/*[in, string]*/char *url, unsigned int port)
{
	return 0;
}
int ocall_socket_send(/*[in, out, size = data_size]*/ void* data, size_t data_size)
{
	return 0;
}
int ocall_socket_receive(/*[in, out, size = data_size]*/ void* data, size_t data_size)
{
	return 0;
}
int ocall_socket_shutdown()
{
	return 0;
}
int ocall_get_the_current_time(unsigned char time[16])
{
	return 0;
}
size_t ocall_file_size(char* filename)
{
	return 0;
}