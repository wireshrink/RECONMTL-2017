
#ifndef SGXENCRYPTIONSERVICE_H
#define SGXENCRYPTIONSERVICE_H

/**
  * class SGXEncryptionservice
  * 
  */

class SGXEncryptionservice
{
public:

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  SGXEncryptionservice ();

  /**
   * Empty Destructor
   */
  virtual ~SGXEncryptionservice ();

  // Static Public attributes
  //  

  // Public attributes
  //  


  // Public attribute accessor methods
  //  


  // Public attribute accessor methods
  //  



  /**
   * @return bool
   */
  bool generateSecureIV ()
  {
  }


  /**
   * @return size_t
   * @param  orig_data_size
   */
  size_t getSealedDataSize (size_t orig_data_size)
  {
  }


  /**
   * @return bool
   * @param  data
   * @param  data_size
   * @param  sealed_data
   * @param  sealed_data_buffer_size
   */
  bool seal (unsigned char* data, size_t data_size, unsigned char* sealed_data, size_t sealed_data_buffer_size)
  {
  }


  /**
   * @return bool
   * @param  data
   * @param  data_size
   * @param  sealed_data
   * @param  sealed_data_size
   */
  bool unseal (unsigned char* data, size_t data_size, unsigned char* sealed_data, size_t sealed_data_size)
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

#endif // SGXENCRYPTIONSERVICE_H
