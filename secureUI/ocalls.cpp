#include "DVSE_u.h"
#include <stdio.h>
#include <time.h>


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
	size_t pos = ftell((FILE*)handle);
	if (pos != offset)
	{
		fseek((FILE*)handle, offset, SEEK_SET);
	}
	return fread(data, 1, size, (FILE*) handle);
}

int ocall_file_write(/*[user_check]*/void *handle, size_t size, /*[in, out]*/unsigned char *data)
{
	return fwrite(data, 1, size, (FILE*)handle);
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
int ocall_get_the_current_time(unsigned char time_holder[16])
{
	time_t ctime;
	static_assert(sizeof (time_t) < 16, "NEED mode buffer for time repr"); 
	time((time_t*)time_holder);
	return 0;
}
size_t ocall_file_size(void* handle)
{
	size_t pos = ftell((FILE*)handle);
	fseek((FILE*)handle, 0, SEEK_END);
	size_t sz = ftell((FILE*)handle);
	fseek((FILE*)handle, pos, SEEK_SET);
	return sz;
}