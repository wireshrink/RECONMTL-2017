#ifndef DVSE_U_H__
#define DVSE_U_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include <string.h>
#include "sgx_edger8r.h" /* for sgx_status_t etc. */


#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

void* SGX_UBRIDGE(SGX_NOCONVENTION, ocall_file_open, (char* file_name, char* format));
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_file_close, (void* handle));
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_file_read, (void* handle, size_t offset, size_t datasize, unsigned char* data));
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_file_write, (void* handle, size_t datasize, unsigned char* data));
size_t SGX_UBRIDGE(SGX_NOCONVENTION, ocall_file_size, (void* file_handle));
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_socket_connect, (char* url, unsigned int port));
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_socket_send, (void* data, size_t data_size));
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_socket_receive, (void* data, size_t data_size));
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_socket_shutdown, ());
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_get_the_current_time, (unsigned char thetime[16]));

sgx_status_t ecall_init_enclave(sgx_enclave_id_t eid, int* retval, char* storage_folder, char* address, int port);
sgx_status_t ecall_update_epg(sgx_enclave_id_t eid, int* retval);
sgx_status_t ecall_get_epg_page(sgx_enclave_id_t eid, int* retval, int number, size_t strsize, void* page);
sgx_status_t ecall_prepare_movie(sgx_enclave_id_t eid, int* retval, size_t movie_id);
sgx_status_t ecall_get_movie_chunk(sgx_enclave_id_t eid, int* retval, size_t chunk_offset, size_t chunk_size, void* chunk);
sgx_status_t ecall_get_movie_file_size(sgx_enclave_id_t eid, int* retval, size_t movie_id, size_t* size);
sgx_status_t ecall_try_coupon(sgx_enclave_id_t eid, int* retval, char* coupon);
sgx_status_t ecall_get_balance(sgx_enclave_id_t eid, int* retval, int* balance);
sgx_status_t ecall_init_secure_channel(sgx_enclave_id_t eid, int* retval, unsigned char key[16]);
sgx_status_t ecall_write_log(sgx_enclave_id_t eid, int* retval, size_t logsize, char* logstr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
