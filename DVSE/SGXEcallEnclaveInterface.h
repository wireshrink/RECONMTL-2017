
#ifndef SGXECALLENCLAVEINTERFACE_H
#define SGXECALLENCLAVEINTERFACE_H




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
	  return false;
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
   * @return bool
   * @param  pos handle seek ops
   */
  bool set_movie_pos (size_t pos)
  {
	  return false;
  }


  /**
   * gets unencrypted position in move
   * @return size_t
   */
  size_t get_movie_pos ()
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

public:


  // Private attribute accessor methods
  //  

private:



};

#endif // SGXECALLENCLAVEINTERFACE_H
