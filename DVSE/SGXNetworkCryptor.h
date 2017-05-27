
#ifndef SGXNETWORKCRYPTOR_H
#define SGXNETWORKCRYPTOR_H


/**
  * class SGXNetworkCryptor
  * 
  */

class SGXNetworkCryptor
{
public:

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  SGXNetworkCryptor ();

  /**
   * Empty Destructor
   */
  virtual ~SGXNetworkCryptor ();

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
  bool sendPublicKey ()
  {
  }


  /**
   * @return bool
   */
  bool getServerPublicKey ()
  {
  }


  /**
   * @return bool
   */
  bool getNextMovieChunk ()
  {
  }


  /**
   * @return size_t
   */
  size_t getLoadedChunkSize ()
  {
  }


  /**
   * @return unsigned char*
   */
  unsigned char* getLoadedChunk ()
  {
  }


  /**
   * @return bool
   * @param  movie_id
   */
  bool setMovieId (size_t movie_id)
  {
  }


  /**
   * @return bool
   * @param  address_1024_
   * @param  port
   */
  bool initConnection (char address_1024_, int port)
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

#endif // SGXNETWORKCRYPTOR_H
