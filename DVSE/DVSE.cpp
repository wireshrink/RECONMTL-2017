#include "DVSE_t.h"
#include "SGXIndependentSealing.h"
#include "sgx_trts.h"
#include <sgx_utils.h>
#include <sgx_tseal.h>
#include <string.h>
#include <stdio.h>
#include "SGXEcallEnclaveInterface.h"




SGXEcallEnclaveInterface *getIf()
{
	return SGXEcallEnclaveInterface::getInstance();
}

int ecall_init_enclave(/*[in, string]*/ char *storage_folder,
	/*[in, string]*/ char *address,
	/*[in]*/         int port)
{
	
	return getIf()->setConnAddr(address, port) && getIf()->init_enclave(storage_folder) ;

}

int ecall_update_epg()
{
	return getIf()->refresh_and_save_service_files();
}

int ecall_get_epg_page(int number, size_t strsize, void* page)
{
	return getIf()->getEPGPage((unsigned char*)page, number);
}

int ecall_prepare_movie(size_t movie_id)
{
	return getIf()->prepare_movie(movie_id);
}



int ecall_get_movie_chunk(size_t chunk_offset, size_t chunk_size, void* chunk)
{
	return getIf()->read_movie_chunk(chunk_offset, chunk_size,(unsigned char*) chunk);
}


int ecall_try_coupon(char* coupon)
{
	return getIf()->applyCoupon(coupon);
}

int ecall_get_balance( int *balance)
{
	*balance = getIf()->getBalance();
	return 1;
}

int ecall_init_secure_channel(unsigned char key[16])
{
	return getIf()->initSecureChannel(key);
}

int ecall_write_log(size_t logsize, char *logstr)
{
	return getIf()->write_log((unsigned char*)logstr, logsize);
}
int ecall_get_movie_file_size(size_t movie_id, size_t * size)
{
	*size = getIf()->get_movie_size(movie_id);
	return (*size != -1L);
}