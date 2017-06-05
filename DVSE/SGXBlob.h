
#ifndef SGXBLOB_H
#define SGXBLOB_H
#include "SGXServiceFile.h"



class SGXBlob : public SGXServiceFile
{
public:

  SGXBlob ();

  virtual ~SGXBlob ();

  

  /**
   * @return bool
   * @param  movie_id
   */
  bool isMoviePlayAllowed (size_t movie_id)
  {
	  return false;
  }


  bool purchaseMovie (size_t movie_id)
  {
	  return false;
  }

  bool isCouponAlreadyUsed (char *coupon)
  {
	  return true;
  }
  bool setCouponAlreadyUsed(char *coupon)
  {
	  return true;
  }


  bool create_default_blob()
  {
	  return download();
  }

  virtual bool download()
  {
	  return downloadById("blob");
  }



private:

  int balance;

public:




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
