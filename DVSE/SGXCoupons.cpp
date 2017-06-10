/************************************************************************************************************
*	This application is a TRAINING TARGET for exercises in HACKING Intel® SGX ENCLAVES.                     *
*	This application made vulnerable DELIBERATELY - its main purpose is to demonstrate, shame and blame     *
*   common mistakes usually made with SGX enclave programming.                                              *
*   ONCE AGAIN, IT CONTAINS MISTAKES.                                                                       *
*   Please DO NOT USE it's source for any healthy development/production activity.                          *
*	It is intended to use for training of hacking SGX enclaves ONLY.                                        *
*	It is written ugly(deliberately), designed badly(intentionally) and running very slow.		        	*
*	You can freely use this application for training purposes.												*
*	I'd be glad to hear about your progress.    															*
*																											*
*	This application requires QT5.8 (which uses LGPL v3 license), Intel SGX SDK and							*
*   the Intel® Software Guard Extensions SSL (Intel® SGX SSL) to be compiled.								*
*	This application is written by Michael Atlas (wireshrink@gmail.com) during 2017.						*
*	Happy hacking.																							*
*************************************************************************************************************/


#include "SGXCoupons.h"

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

