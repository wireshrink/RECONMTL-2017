
#ifndef SGXSERVICEFILE_H
#define SGXSERVICEFILE_H



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
   * // opens the file
   * @return bool
   * @param  filename filename inside of current filesystem
   */
  bool open (char* filename)
  {
  }


  /**
   * @return bool
   */
  bool read_and_decrypt ()
  {
  }


  /**
   * @return bool
   */
  bool encrypt_and_save ()
  {
  }


  /**
   * @return bool
   */
  bool close ()
  {
  }


  /**
   * @return bool
   * @param  data_length
   * @param  data
   */
  bool set_decrypted_content (size_t data_length, unsigned char* data)
  {
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

  char full_file_name_1024_;
  unsigned char * decrypted_content;
public:


  // Private attribute accessor methods
  //  

private:

public:


  // Private attribute accessor methods
  //  


  /**
   * Set the value of full_file_name_1024_
   * @param new_var the new value of full_file_name_1024_
   */
  void setFull_file_name_1024_ (char new_var)   {
      full_file_name_1024_ = new_var;
  }

  /**
   * Get the value of full_file_name_1024_
   * @return the value of full_file_name_1024_
   */
  char getFull_file_name_1024_ ()   {
    return full_file_name_1024_;
  }

  /**
   * Set the value of decrypted_content
   * @param new_var the new value of decrypted_content
   */
  void setDecrypted_content (unsigned char * new_var)   {
      decrypted_content = new_var;
  }

  /**
   * Get the value of decrypted_content
   * @return the value of decrypted_content
   */
  unsigned char * getDecrypted_content ()   {
    return decrypted_content;
  }
private:


  void initAttributes () ;

};

#endif // SGXSERVICEFILE_H
