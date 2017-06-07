#include "SGXBlob.h"

// Constructors/Destructors
//  

SGXBlob::SGXBlob () {
initAttributes();
}

SGXBlob::~SGXBlob () { }

/**
* @return bool
* @param  movie_id
*/

bool SGXBlob::isMoviePlayAllowed(size_t movie_id)
{
	return false;
}

bool SGXBlob::purchaseMovie(size_t movie_id)
{
	return false;
}

bool SGXBlob::isCouponAlreadyUsed(char * coupon)
{
	return true;
}

bool SGXBlob::setCouponAlreadyUsed(char * coupon)
{
	return true;
}

void SGXBlob::initAttributes () {
}

