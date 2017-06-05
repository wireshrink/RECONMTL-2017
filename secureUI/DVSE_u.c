#include "DVSE_u.h"
#include <errno.h>

typedef struct ms_ecall_init_enclave_t {
	int ms_retval;
	char* ms_storage_folder;
	char* ms_address;
	int ms_port;
} ms_ecall_init_enclave_t;

typedef struct ms_ecall_update_epg_t {
	int ms_retval;
} ms_ecall_update_epg_t;

typedef struct ms_ecall_get_epg_page_t {
	int ms_retval;
	int ms_number;
	size_t ms_strsize;
	void* ms_page;
} ms_ecall_get_epg_page_t;

typedef struct ms_ecall_prepare_movie_t {
	int ms_retval;
	size_t ms_movie_id;
} ms_ecall_prepare_movie_t;

typedef struct ms_ecall_get_movie_chunk_t {
	int ms_retval;
	size_t ms_chunk_offset;
	size_t ms_chunk_size;
	void* ms_chunk;
} ms_ecall_get_movie_chunk_t;

typedef struct ms_ecall_get_movie_file_size_t {
	int ms_retval;
	size_t ms_movie_id;
	size_t* ms_size;
} ms_ecall_get_movie_file_size_t;

typedef struct ms_ecall_try_coupon_t {
	int ms_retval;
	char* ms_coupon;
} ms_ecall_try_coupon_t;

typedef struct ms_ecall_get_balance_t {
	int ms_retval;
	int* ms_balance;
} ms_ecall_get_balance_t;

typedef struct ms_ecall_init_secure_channel_t {
	int ms_retval;
	unsigned char* ms_key;
} ms_ecall_init_secure_channel_t;

typedef struct ms_ecall_write_log_t {
	int ms_retval;
	size_t ms_logsize;
	char* ms_logstr;
} ms_ecall_write_log_t;

typedef struct ms_ocall_file_open_t {
	void* ms_retval;
	char* ms_file_name;
	char* ms_format;
} ms_ocall_file_open_t;

typedef struct ms_ocall_file_close_t {
	int ms_retval;
	void* ms_handle;
} ms_ocall_file_close_t;

typedef struct ms_ocall_file_read_t {
	int ms_retval;
	void* ms_handle;
	size_t ms_offset;
	size_t ms_datasize;
	unsigned char* ms_data;
} ms_ocall_file_read_t;

typedef struct ms_ocall_file_write_t {
	int ms_retval;
	void* ms_handle;
	size_t ms_datasize;
	unsigned char* ms_data;
} ms_ocall_file_write_t;

typedef struct ms_ocall_file_size_t {
	size_t ms_retval;
	void* ms_file_handle;
} ms_ocall_file_size_t;

typedef struct ms_ocall_socket_connect_t {
	int ms_retval;
	char* ms_url;
	unsigned int ms_port;
} ms_ocall_socket_connect_t;

typedef struct ms_ocall_socket_send_t {
	int ms_retval;
	void* ms_data;
	size_t ms_data_size;
} ms_ocall_socket_send_t;

typedef struct ms_ocall_socket_receive_t {
	int ms_retval;
	void* ms_data;
	size_t ms_data_size;
} ms_ocall_socket_receive_t;

typedef struct ms_ocall_socket_shutdown_t {
	int ms_retval;
} ms_ocall_socket_shutdown_t;

typedef struct ms_ocall_get_the_current_time_t {
	int ms_retval;
	unsigned char* ms_thetime;
} ms_ocall_get_the_current_time_t;

typedef struct ms_u_sgxssl_ftime64_t {
	void* ms_timeptr;
	uint32_t ms_timeb64Len;
} ms_u_sgxssl_ftime64_t;

typedef struct ms_u_sgxssl_closesocket_t {
	int ms_retval;
	void* ms_s;
	int* ms_wsaError;
} ms_u_sgxssl_closesocket_t;

typedef struct ms_u_sgxssl_recv_t {
	int ms_retval;
	void* ms_s;
	void* ms_buf;
	int ms_len;
	int ms_flag;
	int* ms_wsaError;
} ms_u_sgxssl_recv_t;

typedef struct ms_u_sgxssl_send_t {
	int ms_retval;
	void* ms_s;
	char* ms_buf;
	int ms_len;
	int ms_flags;
	int* ms_wsaError;
} ms_u_sgxssl_send_t;

typedef struct ms_u_sgxssl_shutdown_t {
	int ms_retval;
	void* ms_s;
	int ms_how;
	int* ms_wsaError;
} ms_u_sgxssl_shutdown_t;

typedef struct ms_sgx_oc_cpuidex_t {
	int* ms_cpuinfo;
	int ms_leaf;
	int ms_subleaf;
} ms_sgx_oc_cpuidex_t;

typedef struct ms_sgx_thread_wait_untrusted_event_ocall_t {
	int ms_retval;
	void* ms_self;
} ms_sgx_thread_wait_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_set_untrusted_event_ocall_t {
	int ms_retval;
	void* ms_waiter;
} ms_sgx_thread_set_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_setwait_untrusted_events_ocall_t {
	int ms_retval;
	void* ms_waiter;
	void* ms_self;
} ms_sgx_thread_setwait_untrusted_events_ocall_t;

typedef struct ms_sgx_thread_set_multiple_untrusted_events_ocall_t {
	int ms_retval;
	void** ms_waiters;
	size_t ms_total;
} ms_sgx_thread_set_multiple_untrusted_events_ocall_t;

static sgx_status_t SGX_CDECL DVSE_ocall_file_open(void* pms)
{
	ms_ocall_file_open_t* ms = SGX_CAST(ms_ocall_file_open_t*, pms);
	ms->ms_retval = ocall_file_open(ms->ms_file_name, ms->ms_format);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DVSE_ocall_file_close(void* pms)
{
	ms_ocall_file_close_t* ms = SGX_CAST(ms_ocall_file_close_t*, pms);
	ms->ms_retval = ocall_file_close(ms->ms_handle);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DVSE_ocall_file_read(void* pms)
{
	ms_ocall_file_read_t* ms = SGX_CAST(ms_ocall_file_read_t*, pms);
	ms->ms_retval = ocall_file_read(ms->ms_handle, ms->ms_offset, ms->ms_datasize, ms->ms_data);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DVSE_ocall_file_write(void* pms)
{
	ms_ocall_file_write_t* ms = SGX_CAST(ms_ocall_file_write_t*, pms);
	ms->ms_retval = ocall_file_write(ms->ms_handle, ms->ms_datasize, ms->ms_data);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DVSE_ocall_file_size(void* pms)
{
	ms_ocall_file_size_t* ms = SGX_CAST(ms_ocall_file_size_t*, pms);
	ms->ms_retval = ocall_file_size(ms->ms_file_handle);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DVSE_ocall_socket_connect(void* pms)
{
	ms_ocall_socket_connect_t* ms = SGX_CAST(ms_ocall_socket_connect_t*, pms);
	ms->ms_retval = ocall_socket_connect(ms->ms_url, ms->ms_port);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DVSE_ocall_socket_send(void* pms)
{
	ms_ocall_socket_send_t* ms = SGX_CAST(ms_ocall_socket_send_t*, pms);
	ms->ms_retval = ocall_socket_send(ms->ms_data, ms->ms_data_size);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DVSE_ocall_socket_receive(void* pms)
{
	ms_ocall_socket_receive_t* ms = SGX_CAST(ms_ocall_socket_receive_t*, pms);
	ms->ms_retval = ocall_socket_receive(ms->ms_data, ms->ms_data_size);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DVSE_ocall_socket_shutdown(void* pms)
{
	ms_ocall_socket_shutdown_t* ms = SGX_CAST(ms_ocall_socket_shutdown_t*, pms);
	ms->ms_retval = ocall_socket_shutdown();

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DVSE_ocall_get_the_current_time(void* pms)
{
	ms_ocall_get_the_current_time_t* ms = SGX_CAST(ms_ocall_get_the_current_time_t*, pms);
	ms->ms_retval = ocall_get_the_current_time(ms->ms_thetime);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DVSE_u_sgxssl_ftime64(void* pms)
{
	ms_u_sgxssl_ftime64_t* ms = SGX_CAST(ms_u_sgxssl_ftime64_t*, pms);
	u_sgxssl_ftime64(ms->ms_timeptr, ms->ms_timeb64Len);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DVSE_u_sgxssl_closesocket(void* pms)
{
	ms_u_sgxssl_closesocket_t* ms = SGX_CAST(ms_u_sgxssl_closesocket_t*, pms);
	ms->ms_retval = u_sgxssl_closesocket(ms->ms_s, ms->ms_wsaError);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DVSE_u_sgxssl_recv(void* pms)
{
	ms_u_sgxssl_recv_t* ms = SGX_CAST(ms_u_sgxssl_recv_t*, pms);
	ms->ms_retval = u_sgxssl_recv(ms->ms_s, ms->ms_buf, ms->ms_len, ms->ms_flag, ms->ms_wsaError);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DVSE_u_sgxssl_send(void* pms)
{
	ms_u_sgxssl_send_t* ms = SGX_CAST(ms_u_sgxssl_send_t*, pms);
	ms->ms_retval = u_sgxssl_send(ms->ms_s, (const char*)ms->ms_buf, ms->ms_len, ms->ms_flags, ms->ms_wsaError);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DVSE_u_sgxssl_shutdown(void* pms)
{
	ms_u_sgxssl_shutdown_t* ms = SGX_CAST(ms_u_sgxssl_shutdown_t*, pms);
	ms->ms_retval = u_sgxssl_shutdown(ms->ms_s, ms->ms_how, ms->ms_wsaError);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DVSE_sgx_oc_cpuidex(void* pms)
{
	ms_sgx_oc_cpuidex_t* ms = SGX_CAST(ms_sgx_oc_cpuidex_t*, pms);
	sgx_oc_cpuidex(ms->ms_cpuinfo, ms->ms_leaf, ms->ms_subleaf);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DVSE_sgx_thread_wait_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_wait_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_wait_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_wait_untrusted_event_ocall((const void*)ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DVSE_sgx_thread_set_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_set_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_untrusted_event_ocall((const void*)ms->ms_waiter);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DVSE_sgx_thread_setwait_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_setwait_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_setwait_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_setwait_untrusted_events_ocall((const void*)ms->ms_waiter, (const void*)ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL DVSE_sgx_thread_set_multiple_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_set_multiple_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_multiple_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_multiple_untrusted_events_ocall((const void**)ms->ms_waiters, ms->ms_total);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * func_addr[20];
} ocall_table_DVSE = {
	20,
	{
		(void*)(uintptr_t)DVSE_ocall_file_open,
		(void*)(uintptr_t)DVSE_ocall_file_close,
		(void*)(uintptr_t)DVSE_ocall_file_read,
		(void*)(uintptr_t)DVSE_ocall_file_write,
		(void*)(uintptr_t)DVSE_ocall_file_size,
		(void*)(uintptr_t)DVSE_ocall_socket_connect,
		(void*)(uintptr_t)DVSE_ocall_socket_send,
		(void*)(uintptr_t)DVSE_ocall_socket_receive,
		(void*)(uintptr_t)DVSE_ocall_socket_shutdown,
		(void*)(uintptr_t)DVSE_ocall_get_the_current_time,
		(void*)(uintptr_t)DVSE_u_sgxssl_ftime64,
		(void*)(uintptr_t)DVSE_u_sgxssl_closesocket,
		(void*)(uintptr_t)DVSE_u_sgxssl_recv,
		(void*)(uintptr_t)DVSE_u_sgxssl_send,
		(void*)(uintptr_t)DVSE_u_sgxssl_shutdown,
		(void*)(uintptr_t)DVSE_sgx_oc_cpuidex,
		(void*)(uintptr_t)DVSE_sgx_thread_wait_untrusted_event_ocall,
		(void*)(uintptr_t)DVSE_sgx_thread_set_untrusted_event_ocall,
		(void*)(uintptr_t)DVSE_sgx_thread_setwait_untrusted_events_ocall,
		(void*)(uintptr_t)DVSE_sgx_thread_set_multiple_untrusted_events_ocall,
	}
};

sgx_status_t ecall_init_enclave(sgx_enclave_id_t eid, int* retval, char* storage_folder, char* address, int port)
{
	sgx_status_t status;
	ms_ecall_init_enclave_t ms;
	ms.ms_storage_folder = storage_folder;
	ms.ms_address = address;
	ms.ms_port = port;
	status = sgx_ecall(eid, 0, &ocall_table_DVSE, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t ecall_update_epg(sgx_enclave_id_t eid, int* retval)
{
	sgx_status_t status;
	ms_ecall_update_epg_t ms;
	status = sgx_ecall(eid, 1, &ocall_table_DVSE, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t ecall_get_epg_page(sgx_enclave_id_t eid, int* retval, int number, size_t strsize, void* page)
{
	sgx_status_t status;
	ms_ecall_get_epg_page_t ms;
	ms.ms_number = number;
	ms.ms_strsize = strsize;
	ms.ms_page = page;
	status = sgx_ecall(eid, 2, &ocall_table_DVSE, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t ecall_prepare_movie(sgx_enclave_id_t eid, int* retval, size_t movie_id)
{
	sgx_status_t status;
	ms_ecall_prepare_movie_t ms;
	ms.ms_movie_id = movie_id;
	status = sgx_ecall(eid, 3, &ocall_table_DVSE, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t ecall_get_movie_chunk(sgx_enclave_id_t eid, int* retval, size_t chunk_offset, size_t chunk_size, void* chunk)
{
	sgx_status_t status;
	ms_ecall_get_movie_chunk_t ms;
	ms.ms_chunk_offset = chunk_offset;
	ms.ms_chunk_size = chunk_size;
	ms.ms_chunk = chunk;
	status = sgx_ecall(eid, 4, &ocall_table_DVSE, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t ecall_get_movie_file_size(sgx_enclave_id_t eid, int* retval, size_t movie_id, size_t* size)
{
	sgx_status_t status;
	ms_ecall_get_movie_file_size_t ms;
	ms.ms_movie_id = movie_id;
	ms.ms_size = size;
	status = sgx_ecall(eid, 5, &ocall_table_DVSE, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t ecall_try_coupon(sgx_enclave_id_t eid, int* retval, char* coupon)
{
	sgx_status_t status;
	ms_ecall_try_coupon_t ms;
	ms.ms_coupon = coupon;
	status = sgx_ecall(eid, 6, &ocall_table_DVSE, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t ecall_get_balance(sgx_enclave_id_t eid, int* retval, int* balance)
{
	sgx_status_t status;
	ms_ecall_get_balance_t ms;
	ms.ms_balance = balance;
	status = sgx_ecall(eid, 7, &ocall_table_DVSE, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t ecall_init_secure_channel(sgx_enclave_id_t eid, int* retval, unsigned char key[16])
{
	sgx_status_t status;
	ms_ecall_init_secure_channel_t ms;
	ms.ms_key = (unsigned char*)key;
	status = sgx_ecall(eid, 8, &ocall_table_DVSE, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t ecall_write_log(sgx_enclave_id_t eid, int* retval, size_t logsize, char* logstr)
{
	sgx_status_t status;
	ms_ecall_write_log_t ms;
	ms.ms_logsize = logsize;
	ms.ms_logstr = logstr;
	status = sgx_ecall(eid, 9, &ocall_table_DVSE, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

