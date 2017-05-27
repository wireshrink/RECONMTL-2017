#ifndef DVSE_T_H__
#define DVSE_T_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include "sgx_edger8r.h" /* for sgx_ocall etc. */


#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif


int ecall_init_enclave(char* storage_folder, char* address, int port);
int ecall_update_epg();
int ecall_get_epg_page(int number, size_t strsize, void* page);
int ecall_load_movie(size_t movie_id);
int ecall_get_movie_chunk(size_t movie_id, size_t chunk_offset, size_t chunk_size, void* chunk);
int ecall_get_movie_file_name(size_t movie_id, size_t buf_size, void* filename);
int ecall_try_coupon(char* coupon);
int ecall_get_balance(int* balance);
int ecall_init_secure_channel(unsigned char key[16]);
int ecall_write_log(size_t logsize, char* logstr);

sgx_status_t SGX_CDECL ocall_file_open(void** retval, char* file_name, char* format);
sgx_status_t SGX_CDECL ocall_file_close(int* retval, void* handle);
sgx_status_t SGX_CDECL ocall_file_read(int* retval, void* handle, size_t offset, size_t datasize, unsigned char* data);
sgx_status_t SGX_CDECL ocall_file_write(int* retval, void* handle, size_t offset, size_t datasize, unsigned char* data);
sgx_status_t SGX_CDECL ocall_file_size(size_t* retval, char* file_name);
sgx_status_t SGX_CDECL ocall_socket_connect(int* retval, char* url, unsigned int port);
sgx_status_t SGX_CDECL ocall_socket_send(int* retval, void* data, size_t data_size);
sgx_status_t SGX_CDECL ocall_socket_receive(int* retval, void* data, size_t data_size);
sgx_status_t SGX_CDECL ocall_socket_shutdown(int* retval);
sgx_status_t SGX_CDECL ocall_get_the_current_time(int* retval, unsigned char thetime[16]);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
