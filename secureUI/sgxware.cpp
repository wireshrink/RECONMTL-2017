#include "sgxware.h"
#include <string>
#include <string.h>

#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifdef _MSC_VER
# include <Shlobj.h>
#else
# include <unistd.h>
# include <pwd.h>
# define MAX_PATH FILENAME_MAX
#endif

#include "sgx_urts.h"
#include "sgx_uae_service.h"
#include "DVSE_u.h"

# define TOKEN_FILENAME   "Enclave.token"
# define ENCLAVE_FILENAME "DVSE.signed.dll"

/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;


SGXware *SGXware::m_pInstance = nullptr;

const char* strMovieState(movie_state_t s)
{
    switch (s)
    {
    case M_STS_FREE: return "free view";
    case M_STS_PAY: return "payment required";
    }
    return "state unknown";

}
const char* strMovieStatus(movie_status_t s)
{
    switch(s)
    {
    case M_STT_DOWNLOADED: return "already downloaded";
    case M_STT_VOID:       return "need to download";
    }
    return "status unknown";
}
movie_state_t str2state(char * state)
{
	if (strcmp(state, "free view") == 0) return M_STS_FREE;
	if (strcmp(state, "payment required") == 0) return M_STS_PAY;
	return M_STS_LAST;
}
movie_status_t str2status(char* status)
{
	if (strcmp(status, "already downloaded") == 0) return M_STT_DOWNLOADED;
	if (strcmp(status, "need to download") == 0) return M_STT_VOID;
	return M_STT_LAST;
}

const char* strExpiry(time_t time)
{
    return std::to_string(time).c_str();
	
}
typedef struct _sgx_errlist_t {
	sgx_status_t err;
	const char *msg;
	const char *sug; /* Suggestion */
} sgx_errlist_t;

/* Error code returned by sgx_create_enclave */
static sgx_errlist_t sgx_errlist[] = {
	{
		SGX_ERROR_UNEXPECTED,
		"Unexpected error occurred.",
		NULL
	},
	{
		SGX_ERROR_INVALID_PARAMETER,
		"Invalid parameter.",
		NULL
	},
	{
		SGX_ERROR_OUT_OF_MEMORY,
		"Out of memory.",
		NULL
	},
	{
		SGX_ERROR_ENCLAVE_LOST,
		"Power transition occurred.",
		"Please refer to the sample \"PowerTransition\" for details."
	},
	{
		SGX_ERROR_INVALID_ENCLAVE,
		"Invalid enclave image.",
		NULL
	},
	{
		SGX_ERROR_INVALID_ENCLAVE_ID,
		"Invalid enclave identification.",
		NULL
	},
	{
		SGX_ERROR_INVALID_SIGNATURE,
		"Invalid enclave signature.",
		NULL
	},
	{
		SGX_ERROR_OUT_OF_EPC,
		"Out of EPC memory.",
		NULL
	},
	{
		SGX_ERROR_NO_DEVICE,
		"Invalid SGX device.",
		"Please make sure SGX module is enabled in the BIOS, and install SGX driver afterwards."
	},
	{
		SGX_ERROR_MEMORY_MAP_CONFLICT,
		"Memory map conflicted.",
		NULL
	},
	{
		SGX_ERROR_INVALID_METADATA,
		"Invalid enclave metadata.",
		NULL
	},
	{
		SGX_ERROR_DEVICE_BUSY,
		"SGX device was busy.",
		NULL
	},
	{
		SGX_ERROR_INVALID_VERSION,
		"Enclave version was invalid.",
		NULL
	},
	{
		SGX_ERROR_INVALID_ATTRIBUTE,
		"Enclave was not authorized.",
		NULL
	},
	{
		SGX_ERROR_ENCLAVE_FILE_ACCESS,
		"Can't open enclave file.",
		NULL
	},
	{
		SGX_ERROR_NDEBUG_ENCLAVE,
		"The enclave is signed as product enclave, and can not be created as debuggable enclave.",
		NULL
	},
};

/* Check error conditions for loading enclave */
void print_error_message(sgx_status_t ret)
{
	size_t idx = 0;
	size_t ttl = sizeof sgx_errlist / sizeof sgx_errlist[0];

	for (idx = 0; idx < ttl; idx++) {
		if (ret == sgx_errlist[idx].err) {
			if (NULL != sgx_errlist[idx].sug)
				printf("Info: %s\n", sgx_errlist[idx].sug);
			printf("Error: %s\n", sgx_errlist[idx].msg);
			break;
		}
	}

	if (idx == ttl)
		printf("Error: Unexpected error occurred.\n");
}


SGXware::SGXware()
{
	// creating an enclave
	char token_path[MAX_PATH] = { '\0' };
	sgx_launch_token_t token = { 0 };
	sgx_status_t ret = SGX_ERROR_UNEXPECTED;
	int updated = 0;

	/* Step 1: try to retrieve the launch token saved by last transaction
	*         if there is no token, then create a new one.
	*/
#ifdef _MSC_VER
	/* try to get the token saved in CSIDL_LOCAL_APPDATA */
	if (S_OK != SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, token_path)) {
		strncpy_s(token_path, _countof(token_path), TOKEN_FILENAME, sizeof(TOKEN_FILENAME));
	}
	else {
		strncat_s(token_path, _countof(token_path), "\\" TOKEN_FILENAME, sizeof(TOKEN_FILENAME) + 2);
	}

	/* open the token file */
	HANDLE token_handler = CreateFileA(token_path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
	if (token_handler == INVALID_HANDLE_VALUE) {
		printf("Warning: Failed to create/open the launch token file \"%s\".\n", token_path);
	}
	else {
		/* read the token from saved file */
		DWORD read_num = 0;
		ReadFile(token_handler, token, sizeof(sgx_launch_token_t), &read_num, NULL);
		if (read_num != 0 && read_num != sizeof(sgx_launch_token_t)) {
			/* if token is invalid, clear the buffer */
			memset(&token, 0x0, sizeof(sgx_launch_token_t));
			printf("Warning: Invalid launch token read from \"%s\".\n", token_path);
		}
	}
#else /* __GNUC__ */
	/* try to get the token saved in $HOME */
	const char *home_dir = getpwuid(getuid())->pw_dir;

	if (home_dir != NULL &&
		(strlen(home_dir) + strlen("/") + sizeof(TOKEN_FILENAME) + 1) <= MAX_PATH) {
		/* compose the token path */
		strncpy(token_path, home_dir, strlen(home_dir));
		strncat(token_path, "/", strlen("/"));
		strncat(token_path, TOKEN_FILENAME, sizeof(TOKEN_FILENAME) + 1);
	}
	else {
		/* if token path is too long or $HOME is NULL */
		strncpy(token_path, TOKEN_FILENAME, sizeof(TOKEN_FILENAME));
	}

	FILE *fp = fopen(token_path, "rb");
	if (fp == NULL && (fp = fopen(token_path, "wb")) == NULL) {
		printf("Warning: Failed to create/open the launch token file \"%s\".\n", token_path);
	}

	if (fp != NULL) {
		/* read the token from saved file */
		size_t read_num = fread(token, 1, sizeof(sgx_launch_token_t), fp);
		if (read_num != 0 && read_num != sizeof(sgx_launch_token_t)) {
			/* if token is invalid, clear the buffer */
			memset(&token, 0x0, sizeof(sgx_launch_token_t));
			printf("Warning: Invalid launch token read from \"%s\".\n", token_path);
		}
	}
#endif
	/* Step 2: call sgx_create_enclave to initialize an enclave instance */
	/* Debug Support: set 2nd parameter to 1 */
	ret = sgx_create_enclave(ENCLAVE_FILENAME, SGX_DEBUG_FLAG, &token, &updated, &global_eid, NULL);
	if (ret != SGX_SUCCESS) {
		print_error_message(ret);
#ifdef _MSC_VER
		if (token_handler != INVALID_HANDLE_VALUE)
			CloseHandle(token_handler);
#else
		if (fp != NULL) fclose(fp);
#endif
		return ;
	}

	/* Step 3: save the launch token if it is updated */
#ifdef _MSC_VER
	if (updated == FALSE || token_handler == INVALID_HANDLE_VALUE) {
		/* if the token is not updated, or file handler is invalid, do not perform saving */
		if (token_handler != INVALID_HANDLE_VALUE)
			CloseHandle(token_handler);
		return;
	}

	/* flush the file cache */
	FlushFileBuffers(token_handler);
	/* set access offset to the begin of the file */
	SetFilePointer(token_handler, 0, NULL, FILE_BEGIN);

	/* write back the token */
	DWORD write_num = 0;
	WriteFile(token_handler, token, sizeof(sgx_launch_token_t), &write_num, NULL);
	if (write_num != sizeof(sgx_launch_token_t))
		printf("Warning: Failed to save launch token to \"%s\".\n", token_path);
	CloseHandle(token_handler);
#else /* __GNUC__ */
	if (updated == FALSE || fp == NULL) {
		/* if the token is not updated, or file handler is invalid, do not perform saving */
		if (fp != NULL) fclose(fp);
		return 0;
	}

	/* reopen the file with write capablity */
	fp = freopen(token_path, "wb", fp);
	if (fp == NULL) return 0;
	size_t write_num = fwrite(token, 1, sizeof(sgx_launch_token_t), fp);
	if (write_num != sizeof(sgx_launch_token_t))
		printf("Warning: Failed to save launch token to \"%s\".\n", token_path);
	fclose(fp);
#endif
	
}
SGXware::~SGXware()
{

}
SGXware *		SGXware::getInstance()
{
    m_pInstance = new SGXware;
    return m_pInstance;
}
void			SGXware::destroyInstance()
{

	if (m_pInstance != nullptr)
	{
		sgx_destroy_enclave(global_eid);
		delete m_pInstance;
	}
}
bool			SGXware::initSecureChannel(void)
{
	sgx_status_t ret;
	int retval;
	ret = ecall_init_secure_channel(global_eid, &retval, secure_channel_key);
	if (ret != SGX_SUCCESS)
	{
		print_error_message(ret);
		return false;
	}
	return retval != 0;
}
unsigned int	SGXware::getBalance()
{
	
	int retval;
	int balance = 0;
	sgx_status_t ret = ecall_get_balance(global_eid, &retval, &balance);
	if (ret != SGX_SUCCESS)
	{
		print_error_message(ret);
		return false;
	}
	return retval != 0;
}
bool			SGXware::tryCoupon(char* coupon)
{
	int retval;
	sgx_status_t ret = ecall_try_coupon(global_eid, &retval, coupon);
	if (ret != SGX_SUCCESS)
	{
		print_error_message(ret);
		return false;
	}
	return retval != 0;
}
bool			SGXware::nextMovie(movie_t *pMovie)
{
	return createMovie(pMovie);
}
bool			SGXware::firstMovie(movie_t *pFirstMovie)
{
	
	sgx_status_t ret;
	int res;
	// assuming fresh reloaded EPG
	epg_rdPtr = 0;
	epg_page_index = 0;	
	ret = ecall_get_epg_page(global_eid, &res, epg_page_index ,1024,  current_epg_page);

	if (ret != SGX_SUCCESS)
	{
		print_error_message(ret);
		return false;
	}
	return createMovie(pFirstMovie);
}
bool			SGXware::readUntil(char* buf, char delimiter)
{
	sgx_status_t ret;
	int res;

	int index = 0;
	
	do {
		if (epg_rdPtr >= 1024)
		{
			epg_page_index++;
			epg_rdPtr = 0;
			ret = ecall_get_epg_page(global_eid, &res, epg_page_index, 1024, current_epg_page);

			if (ret != SGX_SUCCESS)
			{
				print_error_message(ret);
				return false;
			}
			if (res == 0)
			{
				return false; // end of the EPG
			}
		}
		if (this->current_epg_page[this->epg_rdPtr] == '\0')
			return false; // end of page, misconfiguration
		buf[index] = this->current_epg_page[this->epg_rdPtr];
		index++;
		epg_rdPtr++;

	} while (this->current_epg_page[this->epg_rdPtr] != '\0' &&
		this->current_epg_page[this->epg_rdPtr] != delimiter);
	// skipping delimiter
	epg_rdPtr++; // next will be caught in the next call

	return true;
}
time_t			str2time(char *buf)
{
	time_t t = (time_t)atoll(buf);
	return t;
}
bool			SGXware::createMovie(movie_t *pNextMovie)
{
	bool res = true;
	char buf[1024];
	
	// assuming we stop by the end of the nex movie definition or in the start of the EPG
	res = res && readUntil(pNextMovie->movie_name, ',');
	res = res && readUntil(buf, ','); pNextMovie->movie_id = atoi(buf);
	res = res && readUntil(buf, ','); pNextMovie->movie_expiration = str2time(buf);
	res = res && readUntil(buf, ','); pNextMovie->movie_state = str2state(buf);
	res = res && readUntil(buf, '\n'); pNextMovie->movie_status = str2status(buf);
	return res;
}
bool			SGXware::downloadMovie(unsigned int movie_id)
{
	int retval;
	sgx_status_t ret = ecall_load_movie(global_eid, &retval, movie_id);
	if (ret != SGX_SUCCESS)
	{
		print_error_message(ret);
		return false;
	}
	return retval != 0;
}
bool			SGXware::refreshEPG(void)
{
	int retval;
	sgx_status_t ret = ecall_update_epg(global_eid, &retval);
	if (ret != SGX_SUCCESS)
	{
		print_error_message(ret);
		return false;
	}
	return retval != 0;
}
bool			SGXware::writeAppLog (unsigned char* data, size_t length)
{
	sgx_status_t ret;
	int retval;
	ret = ecall_write_log(global_eid, &retval, length,(char*) data);
	if (ret != SGX_SUCCESS)
	{
		print_error_message(ret);
		return false;
	}
    return retval != 0;
}
bool			SGXware::getFileName(unsigned int id, size_t movie_name_size, char *movie_name)
{
	int retval;
	
	sgx_status_t ret = ecall_update_epg(global_eid, &retval);
	if (ret != SGX_SUCCESS)
	{
		print_error_message(ret);
		return nullptr;
	}
	ret = ecall_get_movie_file_name(global_eid, &retval, id, movie_name_size, movie_name);
	// epg is stored as comma separated values, so let's read it
	
	return nullptr;
}
bool			SGXware::initUser(char* address, int port, char* folder)
{
	sgx_status_t ret;
	int retval;
	ret = ecall_init_enclave(global_eid, &retval, folder, address, port);
	if (ret != SGX_SUCCESS)
	{
		print_error_message(ret);
		return false;
	}
	return retval != 0;
}
bool			SGXware::getUserProps(char address[1024], int *port, char folder[1024])
{
	sgx_status_t ret;
	int retval;

	ret = ecall_decode_blob(global_eid, &retval, folder, address, port);
	if (ret != SGX_SUCCESS)
	{
		print_error_message(ret);
		return false;
	}
	return retval != 0;
}
DRM_status_t	SGXware::readMovieChunk(unsigned int movie_id, size_t movie_offset, size_t chunk_size, unsigned char* dest)
{
	sgx_status_t ret;
	int retval;

	ret = ecall_get_movie_chunk(global_eid, &retval, movie_id, movie_offset, chunk_size, dest);
	if (ret != SGX_SUCCESS)
	{
		print_error_message(ret);
		return DRM_NEED_MORE_MONEY;
	}
	return retval == 0 ? DRM_NEED_MORE_MONEY: DRM_LAST;

}
bool			SGXware::inplaceDecrypt(qint64 size, void* data)
{

	return true;
}