#include "DVSE_t.h"

#include "sgx_trts.h" /* for sgx_ocalloc, sgx_is_outside_enclave */

#include <errno.h>
#include <string.h> /* for memcpy etc */
#include <stdlib.h> /* for malloc/free etc */

#define CHECK_REF_POINTER(ptr, siz) do {	\
	if (!(ptr) || ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_UNIQUE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)


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
	size_t ms_buf_size;
	size_t* ms_filename;
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
	size_t ms_offset;
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

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4127)
#pragma warning(disable: 4200)
#endif

static sgx_status_t SGX_CDECL sgx_ecall_init_enclave(void* pms)
{
	ms_ecall_init_enclave_t* ms = SGX_CAST(ms_ecall_init_enclave_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_storage_folder = ms->ms_storage_folder;
	size_t _len_storage_folder = _tmp_storage_folder ? strlen(_tmp_storage_folder) + 1 : 0;
	char* _in_storage_folder = NULL;
	char* _tmp_address = ms->ms_address;
	size_t _len_address = _tmp_address ? strlen(_tmp_address) + 1 : 0;
	char* _in_address = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_ecall_init_enclave_t));
	CHECK_UNIQUE_POINTER(_tmp_storage_folder, _len_storage_folder);
	CHECK_UNIQUE_POINTER(_tmp_address, _len_address);

	if (_tmp_storage_folder != NULL) {
		_in_storage_folder = (char*)malloc(_len_storage_folder);
		if (_in_storage_folder == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_storage_folder, _tmp_storage_folder, _len_storage_folder);
		_in_storage_folder[_len_storage_folder - 1] = '\0';
	}
	if (_tmp_address != NULL) {
		_in_address = (char*)malloc(_len_address);
		if (_in_address == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_address, _tmp_address, _len_address);
		_in_address[_len_address - 1] = '\0';
	}
	ms->ms_retval = ecall_init_enclave(_in_storage_folder, _in_address, ms->ms_port);
err:
	if (_in_storage_folder) free(_in_storage_folder);
	if (_in_address) free(_in_address);

	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_update_epg(void* pms)
{
	ms_ecall_update_epg_t* ms = SGX_CAST(ms_ecall_update_epg_t*, pms);
	sgx_status_t status = SGX_SUCCESS;

	CHECK_REF_POINTER(pms, sizeof(ms_ecall_update_epg_t));

	ms->ms_retval = ecall_update_epg();


	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_get_epg_page(void* pms)
{
	ms_ecall_get_epg_page_t* ms = SGX_CAST(ms_ecall_get_epg_page_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	void* _tmp_page = ms->ms_page;
	size_t _tmp_strsize = ms->ms_strsize;
	size_t _len_page = _tmp_strsize;
	void* _in_page = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_ecall_get_epg_page_t));
	CHECK_UNIQUE_POINTER(_tmp_page, _len_page);

	if (_tmp_page != NULL) {
		if ((_in_page = (void*)malloc(_len_page)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_page, 0, _len_page);
	}
	ms->ms_retval = ecall_get_epg_page(ms->ms_number, _tmp_strsize, _in_page);
err:
	if (_in_page) {
		memcpy(_tmp_page, _in_page, _len_page);
		free(_in_page);
	}

	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_prepare_movie(void* pms)
{
	ms_ecall_prepare_movie_t* ms = SGX_CAST(ms_ecall_prepare_movie_t*, pms);
	sgx_status_t status = SGX_SUCCESS;

	CHECK_REF_POINTER(pms, sizeof(ms_ecall_prepare_movie_t));

	ms->ms_retval = ecall_prepare_movie(ms->ms_movie_id);


	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_get_movie_chunk(void* pms)
{
	ms_ecall_get_movie_chunk_t* ms = SGX_CAST(ms_ecall_get_movie_chunk_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	void* _tmp_chunk = ms->ms_chunk;
	size_t _tmp_chunk_size = ms->ms_chunk_size;
	size_t _len_chunk = _tmp_chunk_size;
	void* _in_chunk = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_ecall_get_movie_chunk_t));
	CHECK_UNIQUE_POINTER(_tmp_chunk, _len_chunk);

	if (_tmp_chunk != NULL) {
		if ((_in_chunk = (void*)malloc(_len_chunk)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_chunk, 0, _len_chunk);
	}
	ms->ms_retval = ecall_get_movie_chunk(ms->ms_chunk_offset, _tmp_chunk_size, _in_chunk);
err:
	if (_in_chunk) {
		memcpy(_tmp_chunk, _in_chunk, _len_chunk);
		free(_in_chunk);
	}

	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_get_movie_file_size(void* pms)
{
	ms_ecall_get_movie_file_size_t* ms = SGX_CAST(ms_ecall_get_movie_file_size_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	size_t* _tmp_filename = ms->ms_filename;
	size_t _len_filename = sizeof(*_tmp_filename);
	size_t* _in_filename = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_ecall_get_movie_file_size_t));
	CHECK_UNIQUE_POINTER(_tmp_filename, _len_filename);

	if (_tmp_filename != NULL) {
		if ((_in_filename = (size_t*)malloc(_len_filename)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_filename, 0, _len_filename);
	}
	ms->ms_retval = ecall_get_movie_file_size(ms->ms_movie_id, ms->ms_buf_size, _in_filename);
err:
	if (_in_filename) {
		memcpy(_tmp_filename, _in_filename, _len_filename);
		free(_in_filename);
	}

	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_try_coupon(void* pms)
{
	ms_ecall_try_coupon_t* ms = SGX_CAST(ms_ecall_try_coupon_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_coupon = ms->ms_coupon;
	size_t _len_coupon = _tmp_coupon ? strlen(_tmp_coupon) + 1 : 0;
	char* _in_coupon = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_ecall_try_coupon_t));
	CHECK_UNIQUE_POINTER(_tmp_coupon, _len_coupon);

	if (_tmp_coupon != NULL) {
		_in_coupon = (char*)malloc(_len_coupon);
		if (_in_coupon == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_coupon, _tmp_coupon, _len_coupon);
		_in_coupon[_len_coupon - 1] = '\0';
	}
	ms->ms_retval = ecall_try_coupon(_in_coupon);
err:
	if (_in_coupon) free(_in_coupon);

	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_get_balance(void* pms)
{
	ms_ecall_get_balance_t* ms = SGX_CAST(ms_ecall_get_balance_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	int* _tmp_balance = ms->ms_balance;
	size_t _len_balance = sizeof(*_tmp_balance);
	int* _in_balance = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_ecall_get_balance_t));
	CHECK_UNIQUE_POINTER(_tmp_balance, _len_balance);

	if (_tmp_balance != NULL) {
		if ((_in_balance = (int*)malloc(_len_balance)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_balance, 0, _len_balance);
	}
	ms->ms_retval = ecall_get_balance(_in_balance);
err:
	if (_in_balance) {
		memcpy(_tmp_balance, _in_balance, _len_balance);
		free(_in_balance);
	}

	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_init_secure_channel(void* pms)
{
	ms_ecall_init_secure_channel_t* ms = SGX_CAST(ms_ecall_init_secure_channel_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	unsigned char* _tmp_key = ms->ms_key;
	size_t _len_key = 16 * sizeof(*_tmp_key);
	unsigned char* _in_key = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_ecall_init_secure_channel_t));
	CHECK_UNIQUE_POINTER(_tmp_key, _len_key);

	if (_tmp_key != NULL) {
		if ((_in_key = (unsigned char*)malloc(_len_key)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_key, 0, _len_key);
	}
	ms->ms_retval = ecall_init_secure_channel(_in_key);
err:
	if (_in_key) {
		memcpy(_tmp_key, _in_key, _len_key);
		free(_in_key);
	}

	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_write_log(void* pms)
{
	ms_ecall_write_log_t* ms = SGX_CAST(ms_ecall_write_log_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_logstr = ms->ms_logstr;
	size_t _tmp_logsize = ms->ms_logsize;
	size_t _len_logstr = _tmp_logsize;
	char* _in_logstr = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_ecall_write_log_t));
	CHECK_UNIQUE_POINTER(_tmp_logstr, _len_logstr);

	if (_tmp_logstr != NULL) {
		_in_logstr = (char*)malloc(_len_logstr);
		if (_in_logstr == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_logstr, _tmp_logstr, _len_logstr);
	}
	ms->ms_retval = ecall_write_log(_tmp_logsize, _in_logstr);
err:
	if (_in_logstr) free(_in_logstr);

	return status;
}

SGX_EXTERNC const struct {
	size_t nr_ecall;
	struct {void* call_addr; uint8_t is_priv;} ecall_table[10];
} g_ecall_table = {
	10,
	{
		{(void*)(uintptr_t)sgx_ecall_init_enclave, 0},
		{(void*)(uintptr_t)sgx_ecall_update_epg, 0},
		{(void*)(uintptr_t)sgx_ecall_get_epg_page, 0},
		{(void*)(uintptr_t)sgx_ecall_prepare_movie, 0},
		{(void*)(uintptr_t)sgx_ecall_get_movie_chunk, 0},
		{(void*)(uintptr_t)sgx_ecall_get_movie_file_size, 0},
		{(void*)(uintptr_t)sgx_ecall_try_coupon, 0},
		{(void*)(uintptr_t)sgx_ecall_get_balance, 0},
		{(void*)(uintptr_t)sgx_ecall_init_secure_channel, 0},
		{(void*)(uintptr_t)sgx_ecall_write_log, 0},
	}
};

SGX_EXTERNC const struct {
	size_t nr_ocall;
	uint8_t entry_table[10][10];
} g_dyn_entry_table = {
	10,
	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
	}
};


sgx_status_t SGX_CDECL ocall_file_open(void** retval, char* file_name, char* format)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_file_name = file_name ? strlen(file_name) + 1 : 0;
	size_t _len_format = format ? strlen(format) + 1 : 0;

	ms_ocall_file_open_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_file_open_t);
	void *__tmp = NULL;

	ocalloc_size += (file_name != NULL && sgx_is_within_enclave(file_name, _len_file_name)) ? _len_file_name : 0;
	ocalloc_size += (format != NULL && sgx_is_within_enclave(format, _len_format)) ? _len_format : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_file_open_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_file_open_t));

	if (file_name != NULL && sgx_is_within_enclave(file_name, _len_file_name)) {
		ms->ms_file_name = (char*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_file_name);
		memcpy(ms->ms_file_name, file_name, _len_file_name);
	} else if (file_name == NULL) {
		ms->ms_file_name = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	if (format != NULL && sgx_is_within_enclave(format, _len_format)) {
		ms->ms_format = (char*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_format);
		memcpy(ms->ms_format, format, _len_format);
	} else if (format == NULL) {
		ms->ms_format = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	status = sgx_ocall(0, ms);

	if (retval) *retval = ms->ms_retval;

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_file_close(int* retval, void* handle)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_file_close_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_file_close_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_file_close_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_file_close_t));

	ms->ms_handle = SGX_CAST(void*, handle);
	status = sgx_ocall(1, ms);

	if (retval) *retval = ms->ms_retval;

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_file_read(int* retval, void* handle, size_t offset, size_t datasize, unsigned char* data)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_data = datasize;

	ms_ocall_file_read_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_file_read_t);
	void *__tmp = NULL;

	ocalloc_size += (data != NULL && sgx_is_within_enclave(data, _len_data)) ? _len_data : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_file_read_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_file_read_t));

	ms->ms_handle = SGX_CAST(void*, handle);
	ms->ms_offset = offset;
	ms->ms_datasize = datasize;
	if (data != NULL && sgx_is_within_enclave(data, _len_data)) {
		ms->ms_data = (unsigned char*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_data);
		memcpy(ms->ms_data, data, _len_data);
	} else if (data == NULL) {
		ms->ms_data = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	status = sgx_ocall(2, ms);

	if (retval) *retval = ms->ms_retval;

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_file_write(int* retval, void* handle, size_t offset, size_t datasize, unsigned char* data)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_data = datasize;

	ms_ocall_file_write_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_file_write_t);
	void *__tmp = NULL;

	ocalloc_size += (data != NULL && sgx_is_within_enclave(data, _len_data)) ? _len_data : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_file_write_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_file_write_t));

	ms->ms_handle = SGX_CAST(void*, handle);
	ms->ms_offset = offset;
	ms->ms_datasize = datasize;
	if (data != NULL && sgx_is_within_enclave(data, _len_data)) {
		ms->ms_data = (unsigned char*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_data);
		memset(ms->ms_data, 0, _len_data);
	} else if (data == NULL) {
		ms->ms_data = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	status = sgx_ocall(3, ms);

	if (retval) *retval = ms->ms_retval;
	if (data) memcpy((void*)data, ms->ms_data, _len_data);

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_file_size(size_t* retval, void* file_handle)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_file_size_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_file_size_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_file_size_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_file_size_t));

	ms->ms_file_handle = SGX_CAST(void*, file_handle);
	status = sgx_ocall(4, ms);

	if (retval) *retval = ms->ms_retval;

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_socket_connect(int* retval, char* url, unsigned int port)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_url = url ? strlen(url) + 1 : 0;

	ms_ocall_socket_connect_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_socket_connect_t);
	void *__tmp = NULL;

	ocalloc_size += (url != NULL && sgx_is_within_enclave(url, _len_url)) ? _len_url : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_socket_connect_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_socket_connect_t));

	if (url != NULL && sgx_is_within_enclave(url, _len_url)) {
		ms->ms_url = (char*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_url);
		memcpy(ms->ms_url, url, _len_url);
	} else if (url == NULL) {
		ms->ms_url = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	ms->ms_port = port;
	status = sgx_ocall(5, ms);

	if (retval) *retval = ms->ms_retval;

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_socket_send(int* retval, void* data, size_t data_size)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_data = data_size;

	ms_ocall_socket_send_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_socket_send_t);
	void *__tmp = NULL;

	ocalloc_size += (data != NULL && sgx_is_within_enclave(data, _len_data)) ? _len_data : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_socket_send_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_socket_send_t));

	if (data != NULL && sgx_is_within_enclave(data, _len_data)) {
		ms->ms_data = (void*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_data);
		memcpy(ms->ms_data, data, _len_data);
	} else if (data == NULL) {
		ms->ms_data = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	ms->ms_data_size = data_size;
	status = sgx_ocall(6, ms);

	if (retval) *retval = ms->ms_retval;
	if (data) memcpy((void*)data, ms->ms_data, _len_data);

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_socket_receive(int* retval, void* data, size_t data_size)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_data = data_size;

	ms_ocall_socket_receive_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_socket_receive_t);
	void *__tmp = NULL;

	ocalloc_size += (data != NULL && sgx_is_within_enclave(data, _len_data)) ? _len_data : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_socket_receive_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_socket_receive_t));

	if (data != NULL && sgx_is_within_enclave(data, _len_data)) {
		ms->ms_data = (void*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_data);
		memcpy(ms->ms_data, data, _len_data);
	} else if (data == NULL) {
		ms->ms_data = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	ms->ms_data_size = data_size;
	status = sgx_ocall(7, ms);

	if (retval) *retval = ms->ms_retval;
	if (data) memcpy((void*)data, ms->ms_data, _len_data);

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_socket_shutdown(int* retval)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_socket_shutdown_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_socket_shutdown_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_socket_shutdown_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_socket_shutdown_t));

	status = sgx_ocall(8, ms);

	if (retval) *retval = ms->ms_retval;

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_get_the_current_time(int* retval, unsigned char thetime[16])
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_thetime = 16 * sizeof(*thetime);

	ms_ocall_get_the_current_time_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_get_the_current_time_t);
	void *__tmp = NULL;

	ocalloc_size += (thetime != NULL && sgx_is_within_enclave(thetime, _len_thetime)) ? _len_thetime : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_get_the_current_time_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_get_the_current_time_t));

	if (thetime != NULL && sgx_is_within_enclave(thetime, _len_thetime)) {
		ms->ms_thetime = (unsigned char*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_thetime);
		memcpy(ms->ms_thetime, thetime, _len_thetime);
	} else if (thetime == NULL) {
		ms->ms_thetime = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	status = sgx_ocall(9, ms);

	if (retval) *retval = ms->ms_retval;

	sgx_ocfree();
	return status;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
