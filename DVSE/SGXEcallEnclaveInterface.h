
#ifndef SGXECALLENCLAVEINTERFACE_H
#define SGXECALLENCLAVEINTERFACE_H

#include <string.h>
#include <stdio.h>
#include "DVSE_t.h"
#include "SGXIndependentSealing.h"
#include "sgx_trts.h"


/**
  * class SGXEcallEnclaveInterface
  * 
  */

class SGXEcallEnclaveInterface
{
public:

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  SGXEcallEnclaveInterface ();

  /**
   * Empty Destructor
   */
  virtual ~SGXEcallEnclaveInterface ();

  // Static Public attributes
  //  

  // Public attributes
  //  


  // Public attribute accessor methods
  //  


  // Public attribute accessor methods
  //  



  /**
   * // writes a file into log
   * @param  data
   * @param  datasize
   */
  bool write_log (unsigned char* data, size_t datasize)
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
	  ret = ocall_file_write(&retval, f ,outsize, out );
	  if (ret != SGX_SUCCESS)
	  {
		return false;
	  }
	  if (!SGXIndependentSealing::destroy_allocated_data(out)) return false;
	  return true;
  }


  /**
   * refreshes and saves epg and coupon files
   * @return bool
   */
  bool refresh_and_save_service_files ()
  {
	  return false;
  }


  /**
   *  initializes enclave data, reads the blob inside if exists or creates it,
   * downloads coupons and epg if needed
   * @return bool
   * @param  full_folder_name_1024_
   */
  bool init_enclave (char *full_folder_name)
  {
	  return false;
  }


  /**
   * @return int
   */
  int getBalance ()
  {
	  return false;
  }


  /**
   * @return bool
   * @param  coupon
   */
  bool applyCoupon (char* coupon)
  {
	  return false;
  }


  /**
   * // downloads the movie
   * @return bool
   * @param  movie_id
   */
  bool prepare_movie (size_t movie_id)
  {
	  return false;
  }


  /**
   * // sets up  movie secure channel, verifies that movie is downloaded and/or
   * downloads it
   * @return bool
   * @param  movie_id
   */
  bool start_movie_play (size_t movie_id)
  {
	  return false;
  }


  /**
   * // reads movie chunk and gets it through local secure channel
   * // returns number of read bytes
   * @return int
   * @param  offset offset in not encrypted file
   * @param  max_size max size of a chunk
   * @param  data Buffer to place a chunk
   */
  int read_movie_chunk (size_t offset, size_t max_size, unsigned char* data)
  {
	  return false;
  }


  /**
   * releases all the currently playing movie resources
   * @return bool
   */
  bool release_movie ()
  {
  }


  /**
   * gets DECRYPTED movie size
   * @return size_t
   * @param  movie_id
   */
  size_t get_movie_size (size_t movie_id)
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
  size is 1024, 0 paddinf
   * @return bool
   * @param  data_1024_
   * @param  pageNum
   */
  bool getEPGPage (unsigned char *data, int pageNum) 
  {
	  return false;
  }


  /**
   * @return bool
   * @param  name
   * @param  port
   */
  bool setConnAddr (char* name, int port)
  {
	  return false;
  }



  /**
   * @return bool
   * @param  key_16_
   */
  bool initSecureChannel (unsigned char key[16])
  {
	  return false;
  }

protected:

  // Static Protected attributes
  //  

  // Protected attributes
  //  

public:


  // Protected attribute accessor methods
  //  

protected:

public:


  // Protected attribute accessor methods
  //  

protected:


private:

  // Static Private attributes
  //  

  // Private attributes
  //  

public:


  // Private attribute accessor methods
  //  

private:
	char base_folder[1024];

public:


  // Private attribute accessor methods
  //  

private:



};

#endif // SGXECALLENCLAVEINTERFACE_H
