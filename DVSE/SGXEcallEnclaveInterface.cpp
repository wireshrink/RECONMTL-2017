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
	char blobname[1024];
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

inline int SGXEcallEnclaveInterface::getBalance()
{
	return m_blob.getBalance();
}

/**
* @return bool
* @param  coupon
*/

inline bool SGXEcallEnclaveInterface::applyCoupon(char * coupon)
{
	return m_coupons.applyCoupon(coupon);
}

/**
* gets DECRYPTED movie size
* @return size_t
* @param  movie_id
*/

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

//  
// Methods
//  


// Accessor methods
//  


// Other methods
//  


