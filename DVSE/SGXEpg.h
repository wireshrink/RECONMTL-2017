
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
  bool isFreeToPlay(size_t movie_id);


  /**
   * @return bool
   * @param  data_1024_
   * @param  pageNum
   */
  bool getEPGPage(unsigned char data[1024], int pageNum);

  virtual bool download();

};

#endif // SGXEPG_H
