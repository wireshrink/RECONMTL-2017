
#ifndef SGXCOUPONS_H
#define SGXCOUPONS_H
#include "SGXServiceFile.h"


/**
  * class SGXCoupons
  * 
  */

class SGXCoupons : public SGXServiceFile
{
public:
  SGXCoupons ();

  virtual ~SGXCoupons ();

  bool applyCoupon(char *coupon);

  typedef enum __coup_consts_t
  {
	  COUPON_SIZE=32

  }coup_consts_t;

  virtual bool download()
  {
	  return downloadById("coupons");
  }
};

#endif // SGXCOUPONS_H
