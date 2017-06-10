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

#ifndef SGXLARGEFILE_H
#define SGXLARGEFILE_H


class SGXLargeFile
{
public:

  
  SGXLargeFile ();

  /**
   * Empty Destructor
   */
  virtual ~SGXLargeFile ();

  
  /**
   * @return bool
   * @param  pos
   */
  virtual bool seek(size_t pos) = 0;
 

  /**
   * @return bool
   * @param  filename_1024_
   */
  virtual bool open(char filename[1024]) = 0;
 

  /**
   * @return size_t
   * @param  buffer
   * @param  buffer_size
   */
  virtual size_t read(unsigned char* buffer, size_t buffer_size) = 0;
 

  /**
   * @return size_t
   * @param  buffer
   * @param  buffer_size
   */
  virtual size_t write(unsigned char* buffer, size_t buffer_size) = 0;
  /**
  * @return bool
  */
  virtual bool close() = 0;

  virtual bool openMovie(char* folder, size_t movie_id) = 0;

  bool initialized()
  {
	  return m_binitialized;
  }


protected:
	void* handle;
	bool m_binitialized;


  /**
   * Set the value of handle
   * @param new_var the new value of handle
   */
  void setHandle (void* new_var)   {
      handle = new_var;
  }

  /**
   * Get the value of handle
   * @return the value of handle
   */
  void* getHandle ()   {
    return handle;
  }

  
  static bool createMovieFileName(size_t movie_id, char* base_folder, char* buf, size_t buf_size);
private:


  void initAttributes () ;


};

#endif // SGXLARGEFILE_H
