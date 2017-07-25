#include "common_enclave_actions.h"


#include "sgxware.h"
#include <string>
#include <string.h>

#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifdef _MSC_VER
# include <Shlobj.h>
# include <Pathcch.h>

#else
# include <unistd.h>
# include <pwd.h>
# define MAX_PATH FILENAME_MAX
#endif

#include "sgx_urts.h"
#include "sgx_uae_service.h"
#include "DVSE_u.h"

# define TOKEN_FILENAME   "Enclave.token"
# define ENCLAVE_FILENAME "\\DVSE.signed.dll"

/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;


#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")



bool extract_semi_allowed_file(size_t movie_id, char * store_to)
{
	// assuming that the movie is already prepared
	FILE *f = fopen(store_to, "wb");
	if (!f)
	{
		printf("\n Can not open output file  %s...", store_to);
		return false;
	}
	size_t file_size;
	if (!get_file_size(movie_id, &file_size))
	{
		printf("\n Can not get file size for id %zd", movie_id);
		return false;
	}
	unsigned char movie_chunk[1024];
	size_t movie_offset;
	for (movie_offset = 0; movie_offset < file_size; movie_offset += 1024)
	{
		size_t real_chink_size = read_movie_chunk(movie_offset, 1024, movie_chunk);
		fwrite(movie_chunk, 1, real_chink_size, f);
	}
	fclose(f);
	return false;
}


char* GetThisPath(char* dest, size_t destSize)
{
	if (!dest) return NULL;
	DWORD length = GetModuleFileNameA(NULL, dest, (DWORD)destSize);
	if (MAX_PATH > destSize || length > destSize) return NULL;
	PathRemoveFileSpecA(dest);
	return dest;
}
char* enclave_full_name(char* placeholder, size_t size) // the enclave should reside at the same folder as an executable
{
	GetThisPath(placeholder, size);
	strncat_s(placeholder, 1024, ENCLAVE_FILENAME, 1024L);
	return placeholder;
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

#ifdef _CONSOLE
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
#else
#include <QMessageBox>
void print_error_message(sgx_status_t ret)
{
	QMessageBox box;
	char msg[1024];
	size_t idx = 0;
	size_t ttl = sizeof sgx_errlist / sizeof sgx_errlist[0];

	for (idx = 0; idx < ttl; idx++) {
		if (ret == sgx_errlist[idx].err) {

			snprintf(msg, 1024, "Error: %s\n", sgx_errlist[idx].msg);
			box.setText(msg);
			box.exec();
			break;
		}
	}

	if (idx == ttl)
	{
		QMessageBox box;
		box.setText("Error: Unexpected error occurred.\n");
		box.exec();
	}
}

#endif

bool load_enclave()
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
	char encfullname[1024] = { 0 };

	//ret = sgx_create_enclavea(enclave_full_name(encfullname, 1024), SGX_DEBUG_FLAG, &token, &updated, &global_eid, NULL);
	// from now on working with non-debuggable enclaves
	ret = sgx_create_enclavea(enclave_full_name(encfullname, 1024), SGX_DEBUG_FLAG, &token, &updated, &global_eid, NULL);

	if (ret != SGX_SUCCESS) {
		print_error_message(ret);
#ifdef _MSC_VER
		if (token_handler != INVALID_HANDLE_VALUE)
			CloseHandle(token_handler);
#else
		if (fp != NULL) fclose(fp);
#endif
		return false;
	}

	/* Step 3: save the launch token if it is updated */
#ifdef _MSC_VER
	if (updated == FALSE || token_handler == INVALID_HANDLE_VALUE) {
		/* if the token is not updated, or file handler is invalid, do not perform saving */
		if (token_handler != INVALID_HANDLE_VALUE)
			CloseHandle(token_handler);
		return true;
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
	return true;

}
bool unload_enclave()
{
	return sgx_destroy_enclave(global_eid) == SGX_SUCCESS; // and destroy the enclave
}
bool	apply_coupon(char* coupon)
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

bool prepare_file(size_t movie_id)
{
	int retval;
	sgx_status_t ret = ecall_prepare_movie(global_eid, &retval, movie_id);
	if (ret != SGX_SUCCESS)
	{
		print_error_message(ret);
		return false;
	}
	return retval != 0;
}
bool write_app_log(unsigned char* data, size_t length)
{
	sgx_status_t ret;
	int retval;

	ret = ecall_write_log(global_eid, &retval, length, (char*)data);
	if (ret != SGX_SUCCESS)
	{
		print_error_message(ret);
		return false;
	}
	return retval != 0;
}
bool init_enclave(char* address, char* folder, unsigned int port)
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


size_t	read_movie_chunk(size_t movie_offset, size_t chunk_size, unsigned char* dest)
{
	sgx_status_t ret;
	int retval;


	ret = ecall_get_movie_chunk(global_eid, &retval, movie_offset, chunk_size, dest);

	if (ret != SGX_SUCCESS)
	{
		print_error_message(ret);
		return 0;
	}
	return (size_t)retval;

}

bool get_file_size(size_t movie_id, size_t * fsize)
{
	int res;
	sgx_status_t ret = ecall_get_movie_file_size(global_eid, &res, movie_id, fsize);

	if (ret != SGX_SUCCESS)
	{
		print_error_message(ret);
		return false;
	}
	return res != 0;
}

bool get_epg_info(size_t ** movie_ids, bool ** is_payed, unsigned int * num_of_movies)
{
	int res;
	char current_epg_page[1024];
	// getting only 1st page of epg, it is enough for current setup.
	sgx_status_t ret = ecall_get_epg_page(global_eid, &res, 0, 1024, current_epg_page);

	if (ret != SGX_SUCCESS)
	{
		print_error_message(ret);
		return false;
	}
	// count num of movies, according to "line per movie" criteria
	size_t length = strnlen(current_epg_page, 1024);
	size_t i, counter = 0;

	for (i = 0; i < length; i++)
	{
		if (current_epg_page[i] == '\n') counter++;
	}
	*num_of_movies = (unsigned int)counter;

	*movie_ids = (size_t*) malloc(sizeof(size_t) * counter);
	*is_payed = (bool *)malloc(sizeof(bool) * counter);

	counter = 0;
	char* start = current_epg_page;
	char* end = current_epg_page + length;
	char* ptr = start;
	do
	{
		char *name = ptr;
		char *chid = ( char*)memchr((const char*)name, ',', end - name) + 1; if (chid == ( char *)1 || chid >= end) break;
		char *tm = ( char*)memchr((const char*)chid, ',', end - name) + 1; if (tm == ( char *)1 || tm >= end) break;
		char *stat = ( char*)memchr((const char*)tm, ',', end - name) + 1; if (stat == ( char *)1 || stat >= end) break;

		if (strncmp((char*)stat, "free view", strlen("free view")) == 0)
		{
			// getting id
			(*is_payed)[counter] = false;		}
		else
		{
			(*is_payed)[counter] = true;
		}

		(*movie_ids)[counter] = atoll((const char*)chid);

		ptr = (char*)strchr((const char*)ptr, '\n') + 1;
		counter++;

	} 	while (ptr < end);


	return false;
}