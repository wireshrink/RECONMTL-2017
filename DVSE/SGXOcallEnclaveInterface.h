
#ifndef SGXOCALLENCLAVEINTERFACE_H
#define SGXOCALLENCLAVEINTERFACE_H


/**
  * class SGXOcallEnclaveInterface
  * 
  */

class SGXOcallEnclaveInterface
{
public:

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  SGXOcallEnclaveInterface ();

  /**
   * Empty Destructor
   */
  virtual ~SGXOcallEnclaveInterface ();

  // Static Public attributes
  //  

  // Public attributes
  //  


  // Public attribute accessor methods
  //  


  // Public attribute accessor methods
  //  



  /**
   * @return void*
   * @param  filename
   * @param  param
   */
  void* open_file (char* filename, char* param)
  {
  }


  /**
   * @return bool
   * @param  handle
   */
  bool close_file (void* handle)
  {
  }


  /**
   * @return size_t
   * @param  handle
   * @param  buffer
   * @param  max_size
   */
  size_t read_file (void* handle, unsigned char* buffer, size_t max_size)
  {
  }


  /**
   * @return size_t
   * @param  handle
   * @param  buffer
   * @param  size
   */
  size_t write_file (void* handle, unsigned char* buffer, size_t size)
  {
  }


  /**
   * @return void*
   * @param  address
   * @param  port
   */
  void* connect (char* address, int port)
  {
  }


  /**
   * @return bool
   * @param  handle
   */
  bool close_connection (void* handle)
  {
  }


  /**
   * @return size_t
   * @param  handle
   * @param  buffer
   * @param  buffer_size
   */
  size_t read_conn (void* handle, unsigned char* buffer, size_t buffer_size)
  {
  }


  /**
   * @return size_t
   * @param  handle
   * @param  buffer
   * @param  size
   */
  size_t write_conn (void* handle, unsigned char* buffer, size_t size)
  {
  }


  /**
   */
  void get_current_time ()
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

public:


  // Private attribute accessor methods
  //  

private:

public:


  // Private attribute accessor methods
  //  

private:



};

#endif // SGXOCALLENCLAVEINTERFACE_H
