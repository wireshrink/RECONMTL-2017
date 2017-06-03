#include "SGXEcallEnclaveInterface.h"

// Constructors/Destructors
//  

SGXEcallEnclaveInterface::SGXEcallEnclaveInterface () {
}

SGXEcallEnclaveInterface::~SGXEcallEnclaveInterface () { }

/**
* // writes a file into log
* @param  data
* @param  datasize
*/

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
 // assuming slash issue is alreadu solved during init_enclave
 bool SGXEcallEnclaveInterface::createFilename(char* fname, char * basefolder, const char* name)
 {
	 strncpy(fname, basefolder, 1024);
	 strncat(fname, name, 1024);
	 return true;
 }
/**
* refreshes and saves epg and coupon files
* @return bool
*/

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
	res = res && m_epg.read_and_decrypt();

	res = res && m_coupons.download();
	res = res && m_coupons.read_and_decrypt();

	return res;
}

/**
*  initializes enclave data base folder and calls service file refreshing 
* @return bool
* @param  full_folder_name_1024_
*/

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

/**
* @return int
*/

int SGXEcallEnclaveInterface::getBalance()
{
	return m_blob.getBalance();
}

/**
* @return bool
* @param  coupon
*/

bool SGXEcallEnclaveInterface::applyCoupon(char * coupon)
{
	if (m_coupons.applyCoupon(coupon) &&
		!m_blob.isCouponAlreadyUsed(coupon))
	{
		bool res = true;
		res = res && m_blob.setCouponAlreadyUsed(coupon);
		m_blob.setBalance(m_blob.getBalance() + 10);
		res = res && m_blob.encrypt_and_save();
		return res;
	}
	return false;
}

/**
* // downloads the movie
* @return bool
* @param  movie_id
*/

bool SGXEcallEnclaveInterface::prepare_movie(size_t movie_id)
{
	return false;
}

/**
* gets DECRYPTED movie size
* @return size_t
* @param  movie_id
*/


/**
* releases all the currently playing movie resources
* @return bool
*/


/**
* // reads movie chunk and gets it through local secure channel
* // returns number of read bytes
* @return int
* @param  offset offset in not encrypted file
* @param  max_size max size of a chunk
* @param  data Buffer to place a chunk
*/


/**
* // sets up  movie secure channel, verifies that movie is downloaded and/or
* downloads it
* @return bool
* @param  movie_id
*/

bool SGXEcallEnclaveInterface::start_movie_play(size_t movie_id)
{
	return false;
}

int SGXEcallEnclaveInterface::read_movie_chunk(size_t offset, size_t max_size, unsigned char * data)
{
	return false;
}

bool SGXEcallEnclaveInterface::release_movie()
{
	return false;
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

	outsize = ((outsize / SGXIndependentSealing::SEALED_DATA_CHUNK_SIZE) * SGXIndependentSealing::UNSEALED_DATA_CHUNK_SIZE) +
		((outsize % SGXIndependentSealing::SEALED_DATA_CHUNK_SIZE) - SGXIndependentSealing::SEALING_HEADER_SIZE);
	return outsize;

}

/**
* @return bool
* @param  key_16_
*/


/**
* @return bool
* @param  name
* @param  port
*/


/**
size is 1024, 0 padding
* @return bool
* @param  data_1024_
* @param  pageNum
*/

bool SGXEcallEnclaveInterface::getEPGPage(unsigned char * data, int pageNum)
{
	return m_epg.getEPGPage(data, pageNum);
}

bool SGXEcallEnclaveInterface::setConnAddr(char * name, int port)
{
	return false;
}

bool SGXEcallEnclaveInterface::initSecureChannel(unsigned char key[16])
{
	return sgx_read_rand(key, 16) ==  SGX_SUCCESS;
}
