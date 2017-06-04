
#ifndef SGXFILEREADER_H
#define SGXFILEREADER_H
#include "SGXLargeFile.h"


/**
  * class SGXFileReader
  * all inherited functions should be implemented
  */

class SGXFileReader : public SGXLargeFile
{
public:

  
  SGXFileReader ();

  virtual ~SGXFileReader ();

  virtual bool openMovie(size_t movie_id);
 
  /**
  * @return bool
  * @param  pos
  */
  virtual bool seek(size_t pos);


  /**
  * @return bool
  * @param  filename_1024_
  */
  virtual bool open(char filename[1024]) ;


  /**
  * @return size_t
  * @param  buffer
  * @param  buffer_size
  */
  virtual size_t read(unsigned char* buffer, size_t buffer_size) ;


  /**
  * @return size_t
  * @param  buffer
  * @param  buffer_size
  */
  virtual size_t write(unsigned char* buffer, size_t buffer_size) ;
  /**
  * @return bool
  */
  virtual bool close() ;


protected:



private:



};

#endif // SGXFILEREADER_H
