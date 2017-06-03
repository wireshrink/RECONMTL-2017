#include "SGXCoupons.h"

// Constructors/Destructors
//  

SGXCoupons::SGXCoupons () {
}

SGXCoupons::~SGXCoupons () { }

/**
* @return bool
* @param  coupon
*/
bool SGXCoupons::applyCoupon(char * coupon)
{
	size_t sz = this->get_data_size();

	for (size_t i = 0; i < sz; i += COUPON_SIZE)
	{
		if (strncmp((char*)(this->getContent() + i), coupon, COUPON_SIZE) == 0) return true;
	}
	return false;
}

