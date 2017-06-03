
#ifndef SGXBLOB_H
#define SGXBLOB_H
#include "SGXServiceFile.h"



/**
  * class SGXBlob
  * 
  */

class SGXBlob : public SGXServiceFile
{
public:

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  SGXBlob ();

  /**
   * Empty Destructor
   */
  virtual ~SGXBlob ();

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
   * @param  movie_id
   */
  bool isMoviePlayAllowed (size_t movie_id)
  {
  }


  /**
   * @return bool
   * @param  movie_id
   */
  bool purchaseMovie (size_t movie_id)
  {
  }


  /**
   * @return bool
   * @param  coupon_32_
   */
  bool isCouponAlreadyUsed (char coupon_32_)
  {
  }


  bool create_default_blob()
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

  int balance;
public:


  // Private attribute accessor methods
  //  

private:

public:


  // Private attribute accessor methods
  //  


  /**
   * Set the value of balance
   * @param new_var the new value of balance
   */
  void setBalance (int new_var)   {
      balance = new_var;
  }

  /**
   * Get the value of balance
   * @return the value of balance
   */
  int getBalance ()   {
    return balance;
  }
private:


  void initAttributes () ;

};

#endif // SGXBLOB_H
