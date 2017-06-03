
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

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  SGXServiceFile ();

  /**
   * Empty Destructor
   */
  virtual ~SGXServiceFile ();

  // Static Public attributes
  //  

  // Public attributes
  //  


  // Public attribute accessor methods
  //  


  // Public attribute accessor methods
  //  



  /**
   * // copies a filename inside
   * @return bool
   * @param  filename filename inside of current filesystem
   */
  bool prepare (char* filename)
  {
	  strncpy(full_file_name, filename, 1024);
	  return true;
  }


  /**
   * @return bool
   */
  bool read_and_decrypt ()
  {
	  void * f = nullptr;
	  int retval;
	  sgx_status_t ret = ocall_file_open(&f, full_file_name, "rb");
	  if (ret != SGX_SUCCESS)
	  {
		  return false;
	  }
	  if (decrypted_content != nullptr)
	  {
		  free(decrypted_content);
		  decrypted_content = nullptr;
	  }

	  size_t fsize = -1L;

	  ret = ocall_file_size(&fsize, f);
	  if (ret != SGX_SUCCESS)
	  {
		  return false;
	  }
	  current_data_size = fsize;
	  unsigned char * encrypted_file_content =(unsigned char*) malloc(fsize);

	  ret = ocall_file_read(&retval, f, 0, fsize, encrypted_file_content);
	  if (ret != SGX_SUCCESS || retval != (int)fsize)
	  {
		  current_data_size = -1L;
		  ocall_file_close(&retval, f);
		  free(encrypted_file_content);
		  return false;
	  }

	  if (!SGXIndependentSealing::unseal_data(encrypted_file_content, fsize, &decrypted_content, &this->current_data_size))
	  {
		  current_data_size = -1L;
		  ocall_file_close(&retval, f);
		  free(encrypted_file_content);
		  return false;
	  }
	  free(encrypted_file_content);
	  ret = ocall_file_close(&retval, f);
	  return true;
  }


  /**
   * @return bool
   */
  bool encrypt_and_save ()
  {
	  return false;
  }


  /**
   * @return bool
   */
  bool close ()
  {
	  return false;
  }


  /**
   * @return bool
   * @param  data_length
   * @param  data
   */
  bool set_decrypted_content (size_t data_length, unsigned char* data)
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

  char full_file_name[1024];
  unsigned char * decrypted_content;
  size_t current_data_size;
public:


  // Private attribute accessor methods
  //  

private:

public:


  // Private attribute accessor methods
  //  


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
  const char* getFull_file_name_1024_ ()   {
    return full_file_name;
  }

 
private:


  void initAttributes () ;

};

#endif // SGXSERVICEFILE_H
