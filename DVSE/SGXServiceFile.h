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

#ifndef SGXSERVICEFILE_H
#define SGXSERVICEFILE_H

#include <string.h>
#include "DVSE_t.h"
#include "SGXIndependentSealing.h"
#include "sgx_trts.h"
#include <sgx_utils.h>
#include <stdlib.h>
/**
  * class SGXServiceFile
  * 
  */

class SGXServiceFile
{
public:

  SGXServiceFile ();

  virtual ~SGXServiceFile ();

  
  bool prepare(char* filename);


  bool read_and_decrypt();


  bool encrypt_and_save();

  virtual bool download() = 0;
  

  bool set_decrypted_content(size_t data_length, unsigned char* data);

  size_t get_data_size() { return current_data_size; }



private:


  char full_file_name[1024];
  unsigned char * decrypted_content;
  size_t current_data_size;

public:

  /**
   * Set the value of full_file_name
   * @param new_var the new value of full_file_name
   */
  void setFull_file_name (char *new_var)   {
      strncpy (full_file_name, new_var, 1024);
  }

  /**
   * Get the value of full_file_name_1024_
   * @return the value of full_file_name_1024_
   */
  const char* getFull_file_name ()   {
    return full_file_name;
  }

  unsigned char* const getContent() { return decrypted_content; }

  bool downloadById(char *id);
 
private:


  void initAttributes () ;

};

#endif // SGXSERVICEFILE_H
