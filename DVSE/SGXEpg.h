
#ifndef SGXEPG_H
#define SGXEPG_H
#include "SGXServiceFile.h"
#include <time.h>


/**
  * class SGXEpg
  * 
  */

class SGXEpg : public SGXServiceFile
{
public:

 
  SGXEpg ();

  virtual ~SGXEpg ();

  /**
   * @param  movie_id
   */
  bool isFreeToPlay (size_t movie_id)
  {
	  return false;
  }


  /**
   * @return bool
   * @param  data_1024_
   * @param  pageNum
   */
  bool getEPGPage (unsigned char data[1024], int pageNum)
  {
	  memcpy(data, this->getContent() + pageNum * 1024, 1024);
	  return true;
  }

  virtual bool download()
  {
	  return downloadById("epg");
  }

};

#endif // SGXEPG_H
