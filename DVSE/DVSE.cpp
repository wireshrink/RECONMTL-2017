#include "DVSE_t.h"
#include "SGXIndependentSealing.h"
#include "sgx_trts.h"
#include <sgx_utils.h>
#include <sgx_tseal.h>
#include <string.h>
#include <stdio.h>
#include "SGXPlayerState.h"
#include "SGXVideoPlayerState.h"
#include "utils.h"
#include "SGXEcallEnclaveInterface.h"

unsigned char	g_secure_channel_key[16];
char			base_folder[1024] = {'\0'};
SGXPlayerState	playerState;
SGXVideoPlayerState state;
SGXEcallEnclaveInterface inf;

int ecall_init_enclave(/*[in, string]*/ char *storage_folder,
	/*[in, string]*/ char *address,
	/*[in]*/         int port)
{
	/*char epg_file[1024];
	char coupon_file[1024];
	char blob_file[1024];

	size_t folder_len = strlen(storage_folder);
	if (folder_len > 1000 || folder_len == 0)
	{
		return 0;
	}
	
	strncpy(epg_file, storage_folder, 1024);
	strncpy(coupon_file, storage_folder, 1024);
	strncpy(blob_file, storage_folder, 1024);

	if (!playerState.setConnectionProps(address, port))
	{
		return 0;
	}


	if (storage_folder[folder_len - 1] != '\\')
	{
		strncat(epg_file, "\\", 1024);
		strncat(coupon_file, "\\", 1024);
		strncat(coupon_file, "\\", 1024);
	}

	strncpy(base_folder, epg_file, 1024);

	strncat(epg_file, "epg.enc", 1024);
	strncat(blob_file, "blob.enc", 1024);
	strncat(coupon_file, "coupons.enc", 1024);
	
	size_t file_data_size;
	sgx_status_t ret;
	ret = ocall_file_size(&file_data_size, blob_file);
	if (file_data_size == -1)
	{
		if (!playerState.init_new_user(epg_file, blob_file, coupon_file, address, port)) 
		{
			return 0;
		}
	}
	if (ret != SGX_SUCCESS)
	{
		return false;
	}


	unsigned char* file_data = nullptr;
	size_t         file_size = 0;

	if (!read_whole_file(blob_file, &file_data_size, &file_data))
	{
		
		return 0;
	}

	if (!playerState.setBlob(file_data, file_data_size))
	{
		destroy_read_data(file_data);
	}
	destroy_read_data(file_data);
	if (!read_whole_file(coupon_file, &file_data_size, &file_data))
	{
		destroy_read_data(file_data);
		return 0;
	}

	if (!playerState.setCoupon(file_data, file_data_size))
	{
		destroy_read_data(file_data);
		playerState.destroyBlob();
		return 0;
	}
	destroy_read_data(file_data);
	if (!read_whole_file(epg_file, &file_data_size, &file_data))
	{
		
		destroy_read_data(file_data);
		playerState.destroyBlob();
		playerState.destroyCoupon();
		return 0;

	}
	

	if (!playerState.setEPG(file_data, file_data_size))
	{
		destroy_read_data(file_data);
		playerState.destroyBlob();
		playerState.destroyCoupon();
		return 0;
	
	}
	destroy_read_data(file_data);

	playerState.setValid(true);
	return 1;
	*/
	/*if (!state.setBase_folder(storage_folder)) return 0;
	if (!state.initPlayerState(storage_folder)) return 0;
	if (!state.RefreshAndLoadServiceFiles()) return 0;
	if (!state.)
	state.setValid(1);
	return 1;*/
	
	return inf.init_enclave(storage_folder) && inf.setConnAddr(address, port);

}
/*
int ecall_decode_blob([out]char storage_folder[1024],
	[out]char address[1024],
	[out]int *port)
{
	if (!playerState.isValid())
	{
		return 0;
	}
	strncpy(storage_folder, playerState.getFolder(), 1024);
	strncpy(address, playerState.getHostAddress(), 1024);
	*port = playerState.getPort();
	return 1;
	
}
*/

int ecall_update_epg()
{
	/*
	char * epgdata = nullptr;
	size_t epgsize = 0;
	char  epgfilename[1024];

	strncpy(epgfilename, base_folder, 1024);
	strncat(epgfilename, "epg.enc", 1024);
	if (!download_file("epg.epg", &epgsize, &epgdata))
	{
		return 0;
	}
	if (!seal_file(epgfilename, epgdata, epgsize))
	{
		destroy_read_data((unsigned char*)epgdata);
		return 0;
	}

	destroy_read_data((unsigned char*)epgdata);

	if (!read_whole_file(epgfilename, &epgsize, (unsigned char**)&epgdata))
	{

		destroy_read_data((unsigned char*)epgdata);
		return 0;

	}


	if (!playerState.setEPG((unsigned char*)epgdata, epgsize))
	{
		destroy_read_data((unsigned char*)epgdata);
		return 0;

	}
	destroy_read_data((unsigned char*)epgdata);

	return 1;
	*/
	return inf.refresh_and_save_service_files();
}

int ecall_get_epg_page(int number, size_t strsize, void* page)
{
	return inf.getEPGPage((unsigned char*)page, number);
}

int ecall_prepare_movie(size_t movie_id)
{
	/*char movie_name[1024];
	char movie_filename[1024];
	snprintf(movie_name, 1024, "movie.%16x", movie_id);
	strncpy(movie_filename, base_folder, 1024);
	strncat(movie_filename, movie_name, 1024);
	
	if (!download_and_seal(movie_name, movie_filename))
	{
		return 0;
	}
	return 1;*/
	return inf.prepare_movie(movie_id);
}

// chunk offset and size are in normal file offset system, headers of seaked data are not encounted
// file is written in chukns of 1024

int ecall_get_movie_chunk(size_t chunk_offset, size_t chunk_size, void* chunk)
{
	/*char movie_name[1024];
	char movie_filename[1024];
	unsigned char encrypted_chunks[2048];
	unsigned char decrypted_data[1024];
	snprintf(movie_name, 1024, "movie.%ld", movie_id);
	strncpy(movie_filename, base_folder, 1024);
	strncat(movie_filename, movie_name, 1024);

	sgx_status_t ret;
	int res;
	time_t curtime;

	// checking balance

	ret = ocall_get_the_current_time(&res, (unsigned char*)&curtime);
	if (ret != SGX_SUCCESS)
		return 0;

	// should decrease the balance if applicable
	if (!playerState.checkMovieAllowance(movie_id, curtime))
	{
		return 0;
	}
	if (playerState.changed())
	{
		if (!playerState.saveEPG() || !playerState.saveBlob())
		{
			return 0;
		}
	}
	void * f = nullptr;

	ret = ocall_file_open(&f, movie_filename, "rb");

	if (ret != SGX_SUCCESS)
		return 0;

	if (f == nullptr)
	{
		return 0;
	}

	size_t rdptr = 0;
	size_t toread = chunk_size;
	size_t first_chunk_pos = chunk_offset / 1024;

	ret = ocall_file_read(&res, f, first_chunk_pos * SGXIndependentSealing::calc_sealed_data_size(1024), 
						  SGXIndependentSealing::calc_sealed_data_size(1024), encrypted_chunks); // reading the first chunk

	if (ret != SGX_SUCCESS)
		return 0;
	unsigned char* unsealed_data = nullptr;
	size_t outsize = 0;
	if (!SGXIndependentSealing::unseal_data(encrypted_chunks, res, &unsealed_data, &outsize))
	{
		ret = ocall_file_close(&res, f);
		return 0;
	}

	memcpy(chunk, unsealed_data + chunk_offset % 1024, 1024 - (chunk_offset % 1024));
	rdptr = 1024 - (chunk_offset % 1024);


	
	


	return 0; */
	return inf.read_movie_chunk(chunk_offset, chunk_size,(unsigned char*) chunk);
}


int ecall_get_movie_file_size(size_t movie_id, size_t buf_size, /*[out, size = buf_size]*/size_t* sz)
{
	/*if (base_folder[0] == '\0')
		return 0;
	snprintf((char*)filename, buf_size, "%smovie.%16llx", base_folder, movie_id); */
	return 0;
}

int ecall_try_coupon(char* coupon)
{
	// 
	/*for (int i = 0; i < playerState.couponCount(); i++)
	{
		if (!playerState.couponUsed(i) && strcmp(coupon, playerState.getCoupon(i)) == 0)
			return 1;
	}
	return 0;*/
	return inf.applyCoupon(coupon);
}

int ecall_get_balance( int *balance)
{
	*balance = state.getBalance();
	return 1;
}

int ecall_init_secure_channel(unsigned char key[16])
{
	/*if (sgx_read_rand(key, 16) != SGX_SUCCESS)
	{
		return 0;
	}
	memcpy(g_secure_channel_key, key, 16);
	return 1;
	*/
	return inf.initSecureChannel(key);
}
int ecall_write_log(size_t logsize, char *logstr)
{
	/*void *f = nullptr;
	int retval;
	size_t outsize;
	unsigned char *out = nullptr;
	char logfile[1024];
	if (base_folder[0] == '\0')
	{
		return 0;
	}
	strncpy(logfile, base_folder, 1024);
	strncat(logfile, "applog.txt", 1024);
	sgx_status_t ret = ocall_file_open(&f, logfile, "awb");
	if (ret != SGX_SUCCESS)
	{
		return 0;
	}
	if (f == nullptr)
	{
		return 0;
	}
	if (!SGXIndependentSealing::seal_data((unsigned char*)logstr, logsize, &out, &outsize))
	{
		return 0;
	}
	ret = ocall_file_write(&retval, f, (size_t)-1,outsize, out );
	if (ret != SGX_SUCCESS)
	{
		return 0;
	}
	if (!SGXIndependentSealing::destroy_allocated_data(out))
		return 0;
	
	return 1;
	*/
	return inf.write_log((unsigned char*)logstr, logsize);
}