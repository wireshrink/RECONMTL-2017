
#ifndef SGXNETWORKREADER_H
#define SGXNETWORKREADER_H
#include "SGXLargeFile.h"


/**
  * class SGXNetworkReader
  * all inherited functions should be implemented
  */

class SGXNetworkReader : public SGXLargeFile
{
public:

  SGXNetworkReader ();

  virtual ~SGXNetworkReader ();

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

#endif // SGXNETWORKREADER_H
