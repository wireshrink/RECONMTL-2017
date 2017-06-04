
#ifndef SGXFILEWRITER_H
#define SGXFILEWRITER_H
#include "SGXLargeFile.h"


/**
  * class SGXFileWriter
  * all inherited functions should be implemented
  */

class SGXFileWriter : public SGXLargeFile
{
public:

  
  SGXFileWriter ();

  virtual ~SGXFileWriter ();

  virtual bool openMovie(char* base_folder, size_t movie_id);

  /**
  * @return bool
  * @param  pos
  */
  virtual bool seek(size_t pos);


  /**
  * @return bool
  * @param  filename_1024_
  */
  virtual bool open(char filename[1024]);


  /**
  * @return size_t
  * @param  buffer
  * @param  buffer_size
  */
  virtual size_t read(unsigned char* buffer, size_t buffer_size);


  /**
  * @return size_t
  * @param  buffer
  * @param  buffer_size
  */
  virtual size_t write(unsigned char* buffer, size_t buffer_size);
  /**
  * @return bool
  */
  virtual bool close();


protected:


private:



};

#endif // SGXFILEWRITER_H
