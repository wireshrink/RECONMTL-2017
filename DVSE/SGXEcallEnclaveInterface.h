
#ifndef SGXECALLENCLAVEINTERFACE_H
#define SGXECALLENCLAVEINTERFACE_H

#include <string.h>
#include <stdio.h>
#include "DVSE_t.h"
#include "SGXIndependentSealing.h"
#include "sgx_trts.h"

#include "SGXBlob.h"
#include "SGXEpg.h"
#include "SGXCoupons.h"
#include "SGXNetworkReader.h"
#include "SGXFileWriter.h"
#include "SGXFileReader.h"
/**
  * class SGXEcallEnclaveInterface
  * 
  */

class SGXEcallEnclaveInterface
{
	SGXEcallEnclaveInterface();
	virtual ~SGXEcallEnclaveInterface();


public:
	static SGXEcallEnclaveInterface * m_pInstance;
	static SGXEcallEnclaveInterface * getInstance();
  

  /**
   * // writes a file into log
   * @param  data
   * @param  datasize
   */
  bool write_log(unsigned char* data, size_t datasize);

  bool createFilename(char * fname, char * basefolder, const char * name);


  /**
   * refreshes and saves epg and coupon files
   * @return bool
   */
  bool refresh_and_save_service_files();


  /**
  *  initializes enclave data base folder and calls service file refreshing
  * @return bool
  * @param  full_folder_name_1024_
  */

  bool init_enclave(char *full_folder_name);


  /**
   * @return int
   */
  int getBalance();


  /**
   * @return bool
   * @param  coupon
   */
  bool applyCoupon(char* coupon);


  /**
   * downloads the movie
   * @return bool
   * @param  movie_id
   */
  bool prepare_movie(size_t movie_id);


  /**
   * // sets up  movie secure channel, verifies that movie is downloaded and/or
   * downloads it
   * @return bool
   * @param  movie_id
   */
  bool start_movie_play(size_t movie_id);


  /**
   * // reads movie chunk and gets it through local secure channel
   * // returns number of read bytes
   * @return int
   * @param  offset offset in not encrypted file
   * @param  max_size max size of a chunk
   * @param  data Buffer to place a chunk
   */
  int read_movie_chunk(size_t offset, size_t max_size, unsigned char* data);


  /**
   * releases all the currently playing movie resources
   * @return bool
   */
  bool release_movie();


  /**
   * gets DECRYPTED movie size
   * @return size_tW
   * @param  movie_id
   */
  size_t get_movie_size(size_t movie_id);


  /**
  size is 1024, 0 padding
   * @return bool
   * @param  data_1024_
   * @param  pageNum
   */
  bool getEPGPage(unsigned char *data, int pageNum);


  /**
   * @return bool
   * @param  name
   * @param  port
   */
  bool setConnAddr(char* name, int port);



  /**
   * @return bool
   * @param  key_16_
   */
  bool initSecureChannel(unsigned char key[16]);


  bool isFreeToPlay(size_t movie_id) { return m_epg.isFreeToPlay(movie_id); }

private:
	char base_folder[1024];


	unsigned char sec_channel_key[16];

	int m_connport;
	char m_conaddr[1024];
	SGXBlob m_blob;
	SGXEpg  m_epg;
	SGXCoupons m_coupons;

	//void* playing_movie_handle;

	SGXFileReader		m_largeFileReader;
	SGXFileWriter		m_largeFileWriter;
	SGXNetworkReader	m_largeFileNetReader;


	bool downloadMovie(size_t movie_id);
	 
};

#endif // SGXECALLENCLAVEINTERFACE_H
