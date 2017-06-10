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

#include "SGXEcallEnclaveInterface.h"
#include "SGXNetworkReader.h"
#include "SGXSslWare.h"


SGXEcallEnclaveInterface::SGXEcallEnclaveInterface () {
	
}

SGXEcallEnclaveInterface::~SGXEcallEnclaveInterface () { }

SGXEcallEnclaveInterface * SGXEcallEnclaveInterface::m_pInstance = nullptr;

SGXEcallEnclaveInterface * SGXEcallEnclaveInterface::getInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new SGXEcallEnclaveInterface();
	}
	return m_pInstance;
}

 bool SGXEcallEnclaveInterface::write_log(unsigned char * data, size_t datasize)
{
	void *f = nullptr;
	int retval;
	size_t outsize;
	unsigned char *out = nullptr;
	char logfile[1024];
	if (base_folder[0] == '\0')
	{
		return false;
	}
	strncpy(logfile, base_folder, 1024);
	strncat(logfile, "applog.txt", 1024);
	sgx_status_t ret = ocall_file_open(&f, logfile, "awb");
	if (ret != SGX_SUCCESS)
	{
		return false;
	}
	if (f == nullptr)
	{
		return false;
	}
	if (!SGXIndependentSealing::seal_data((unsigned char*)data, datasize, &out, &outsize))
	{
		return false;
	}
	ret = ocall_file_write(&retval, f, outsize, out);
	if (ret != SGX_SUCCESS)
	{
		return false;
	}
	if (!SGXIndependentSealing::destroy_allocated_data(out)) return false;
	return true;
}
 // assuming slash issue is already solved during init_enclave
 bool SGXEcallEnclaveInterface::createFilename(char* fname, char * basefolder, const char* name)
 {
	 strncpy(fname, basefolder, 1024);
	 strncat(fname, name, 1024);
	 return true;
 }

 bool SGXEcallEnclaveInterface::refresh_and_save_service_files()
{
	bool res = true;
	char blobname[1024];
	char epgname[1024];
	char coupname[1024];


	res = res && createFilename(blobname, base_folder, "blob.enc");
	res = res && createFilename(epgname, base_folder,  "epg.enc");
	res = res && createFilename(coupname, base_folder, "coupon.enc");

	res = res && m_blob.prepare(blobname);
	res = res && m_epg.prepare(epgname);
	res = res && m_coupons.prepare(coupname);


	if (res && !m_blob.read_and_decrypt())
	{
		res  = res && m_blob.create_default_blob(); 
		res = res && m_blob.encrypt_and_save();
	}
	
	res = res && m_epg.download();
	res = res && m_epg.encrypt_and_save();

	res = res && m_coupons.download();
	res = res && m_coupons.encrypt_and_save();

	return res;
}

bool SGXEcallEnclaveInterface::init_enclave(char * full_folder_name)
{
	size_t l = strlen(full_folder_name);
	
	if (l >= 1023)
	{
		return false;
	}

	strncpy(this->base_folder, full_folder_name, 1024);
	if (base_folder[l - 1] != '\\')
	{
		strncat(base_folder, "\\", 1024);
	}

	return refresh_and_save_service_files();
}

int SGXEcallEnclaveInterface::getBalance()
{
	return m_blob.getBalance();
}

bool SGXEcallEnclaveInterface::applyCoupon(char * coupon)
{
	if (m_coupons.applyCoupon(coupon) &&
		!m_blob.isCouponAlreadyUsed(coupon))
	{
		bool res = true;
		res = res && m_blob.setCouponAlreadyUsed(coupon);
		res  = res && m_blob.setBalance(m_blob.getBalance() + 25); // saves automatically
		return res;
	}
	return false;
}


bool SGXEcallEnclaveInterface::prepare_movie(size_t movie_id)
{
	if (!m_blob.isMoviePlayAllowed(movie_id))
	{
		if (!m_blob.purchaseMovie(movie_id))
			return false;
	}
	if (m_blob.isMoviePlayAllowed(movie_id))
	{
		if (get_movie_size(movie_id) == -1L)
		{
			// initiate movie download
			if (!downloadMovie(movie_id))
			{
				return false;
			}
		}
		return start_movie_play(movie_id);
	}
	return false;
}


bool SGXEcallEnclaveInterface::start_movie_play(size_t movie_id)
{
	if (m_largeFileReader.initialized())
	{
		if (!release_movie())
			return false;
	}
	if (!m_largeFileReader.openMovie(this->base_folder, movie_id))
	{
		return false;
	}
	return m_largeFileReader.initialized();
}

int SGXEcallEnclaveInterface::read_movie_chunk(size_t offset, size_t max_size, unsigned char * data)
{
	if (!m_largeFileReader.seek(offset))
		return 0;
	size_t res = m_largeFileReader.read(data, max_size);
	return (int)res;
}

bool SGXEcallEnclaveInterface::release_movie()
{
	return m_largeFileReader.close();
}

size_t SGXEcallEnclaveInterface::get_movie_size(size_t movie_id)
{
	void *f = nullptr;
	int retval;
	size_t outsize;
	unsigned char *out = nullptr;
	char movie_file_name[1024];
	if (base_folder[0] == '\0')
	{
		return -1L;
	}
	snprintf(movie_file_name, 1024, "%s\\movie.%zx", this->base_folder, movie_id);
	sgx_status_t ret = ocall_file_open(&f, movie_file_name, "rb");
	
	if (ret != SGX_SUCCESS)
	{
		return -1L;
	}
	if (f == nullptr)
	{
		return -1L;
	}
	ret = ocall_file_size(&outsize, f);
	if (ret != SGX_SUCCESS)
	{
		return -1L;
	}
	ret = ocall_file_close(&retval, f);
	if (ret != SGX_SUCCESS)
	{
		return -1L;
	}

	outsize = SGXIndependentSealing::calc_unsealed_data_size(outsize);
	return outsize;

}

bool SGXEcallEnclaveInterface::getEPGPage(unsigned char * data, int pageNum)
{
	return m_epg.getEPGPage(data, pageNum);
}

bool SGXEcallEnclaveInterface::setConnAddr(char * name, int port)
{
	m_connport = port;
	strncpy(m_conaddr, name, 1024);
	bool res = SGXSslWare::getInstance()->connect(m_conaddr, m_connport);
	if (res) SGXSslWare::getInstance()->shutdown();
	return res;
}

bool SGXEcallEnclaveInterface::initSecureChannel(unsigned char key[16])
{
	bool ret = sgx_read_rand(key, 16) ==  SGX_SUCCESS;
	memcpy(sec_channel_key, key, 16);
	return ret;
}

bool SGXEcallEnclaveInterface::downloadMovie(size_t movie_id)
{
	size_t read_size = 0, write_size = 0;
	unsigned char buffer[1024];
	if (!m_largeFileNetReader.openMovie(this->base_folder, movie_id))
		return false;
	if (!m_largeFileWriter.openMovie(this->base_folder, movie_id))
		return false;
	do
	{
		read_size = m_largeFileNetReader.read(buffer, 1024);
		if (read_size == -1L)
			return false;
		if (read_size > 0)
		{
			write_size = m_largeFileWriter.write(buffer, read_size);
			if (write_size == -1L) return false;
		}

	} while (read_size != 0);
	m_largeFileWriter.close();
	return true;
}
