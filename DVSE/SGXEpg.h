
#ifndef SGXEPG_H
#define SGXEPG_H
#include "SGXServiceFile.h"



/**
  * class SGXEpg
  * 
  */

class SGXEpg : public SGXServiceFile
{
public:

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  SGXEpg ();

  /**
   * Empty Destructor
   */
  virtual ~SGXEpg ();

  // Static Public attributes
  //  

  // Public attributes
  //  


  // Public attribute accessor methods
  //  


  // Public attribute accessor methods
  //  



  /**
   * @param  movie_id
   */
  void getMovieLastTime (size_t movie_id)
  {
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
	  return false;
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

#endif // SGXEPG_H
