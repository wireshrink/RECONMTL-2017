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

#ifndef SGXFILEWRITER_H
#define SGXFILEWRITER_H
#include "SGXLargeFile.h"


/**
  * class SGXFileWriter
  * all inherited functions should be implemented
  */

class SGXFileWriter : public SGXLargeFile
{
public:

  
  SGXFileWriter ();

  virtual ~SGXFileWriter ();

  virtual bool openMovie(char* base_folder, size_t movie_id);

  /**
  * @return bool
  * @param  pos
  */
  virtual bool seek(size_t pos);


  /**
  * @return bool
  * @param  filename_1024_
  */
  virtual bool open(char filename[1024]);


  /**
  * @return size_t
  * @param  buffer
  * @param  buffer_size
  */
  virtual size_t read(unsigned char* buffer, size_t buffer_size);


  /**
  * @return size_t
  * @param  buffer
  * @param  buffer_size
  */
  virtual size_t write(unsigned char* buffer, size_t buffer_size);
  /**
  * @return bool
  */
  virtual bool close();


protected:


private:



};

#endif // SGXFILEWRITER_H
