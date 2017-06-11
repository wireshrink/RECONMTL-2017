/************************************************************************************************************
*	This application is a TRAINING TARGET for exercises in HACKING Intel® SGX ENCLAVES.                     *
*	This application made vulnerable DELIBERATELY - its main purpose is to demonstrate, shame and blame     *
*   common mistakes usually made with SGX enclave programming.                                              *
*   ONCE AGAIN, IT CONTAINS MISTAKES.                                                                       *
*   Please DO NOT USE it's source for any healthy development/production activity.                          *
*	It is intended to use for training of hacking SGX enclaves ONLY.                                        *
*	It is written ugly(deliberately), designed badly(intentionally) and running very slow.		        	*
*	You can freely use this application for training purposes.												*
*	I'd be glad to hear about your progress.    															*
*																											*
*	This application requires QT5.8 (which uses LGPL v3 license), Intel SGX SDK and							*
*   the Intel® Software Guard Extensions SSL (Intel® SGX SSL) to be compiled.								*
*	This application is written by Michael Atlas (wireshrink@gmail.com) during 2017.						*
*	Happy hacking.																							*
*************************************************************************************************************/


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
	if (getIf()->isBlobInitialized())
		return 1;
	return 0;
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