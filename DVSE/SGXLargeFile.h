
#ifndef SGXLARGEFILE_H
#define SGXLARGEFILE_H



/**
  * class SGXLargeFile
  * 
  */

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
	unsigned char buffer[1024 * 10];
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
