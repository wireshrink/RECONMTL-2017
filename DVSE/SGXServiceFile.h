
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

  
  /**
   * // copies a filename inside
   * @return bool
   * @param  filename filename inside of current filesystem
   */
  bool prepare(char* filename);


  /**
   * @return bool
   */
  bool read_and_decrypt();


  /**
   * @return bool
   */
  bool encrypt_and_save();


  

  /**
   * @return bool
   * @param  data_length
   * @param  data
   */
  bool set_decrypted_content(size_t data_length, unsigned char* data);

  size_t get_data_size() { return current_data_size; }

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
  const char* getFull_file_name ()   {
    return full_file_name;
  }

  const unsigned char* getContent() { return decrypted_content; }

 
private:


  void initAttributes () ;

};

#endif // SGXSERVICEFILE_H
