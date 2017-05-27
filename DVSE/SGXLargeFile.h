
#ifndef SGXLARGEFILE_H
#define SGXLARGEFILE_H



/**
  * class SGXLargeFile
  * 
  */

class SGXLargeFile
{
public:

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  SGXLargeFile ();

  /**
   * Empty Destructor
   */
  virtual ~SGXLargeFile ();

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

  unsigned char buffer[1024*10];
  void* handle;
public:


  // Private attribute accessor methods
  //  

private:

public:


  // Private attribute accessor methods
  //  


  
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
private:


  void initAttributes () ;

};

#endif // SGXLARGEFILE_H
