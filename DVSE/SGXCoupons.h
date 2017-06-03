
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

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  SGXCoupons ();

  /**
   * Empty Destructor
   */
  virtual ~SGXCoupons ();

  /**
   * @return bool
   * @param  coupon
   */
  bool applyCoupon(char *coupon);

  typedef enum __coup_consts_t
  {
	  COUPON_SIZE=32

  }coup_consts_t;

  virtual bool download()
  {
	  return false;
  }
};

#endif // SGXCOUPONS_H
