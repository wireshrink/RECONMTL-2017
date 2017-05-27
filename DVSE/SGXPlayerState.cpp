#include "SGXPlayerState.h"
#include <string.h>



SGXPlayerState::SGXPlayerState()
{
	this->balance = 0;
	memset_s(this->folder,1024,  0, 1024);
	memset_s(this->address, 1024, 0, 1024);
	this->port = 0;
	this->valid = false;
	this->p_decoded_epg = nullptr;
	this->p_decoded_blob = nullptr;
	this->p_decoded_coupons = nullptr;
}


SGXPlayerState::~SGXPlayerState()
{
}

bool SGXPlayerState::couponUsed(int index)
{
	return false;
}

char * SGXPlayerState::getCoupon(int index)
{
	return nullptr;
}

bool SGXPlayerState::setConnectionProps(char * addr, int port)
{
	return false;
}

bool SGXPlayerState::init_new_user(char * epg_file, char * blob_file, char * coupon_file, char * address, int port)
{
	return false;
}

bool SGXPlayerState::setBlob(unsigned char * encryptedBlob, size_t blobSize)
{
	return false;
}

bool SGXPlayerState::setCoupon(unsigned char * encryptedCoupon, size_t blobSize)
{
	return false;
}

bool SGXPlayerState::setEPG(unsigned char * encryptedEPG, size_t blobSize)
{
	return false;
}

bool SGXPlayerState::destroyCoupon()
{
	return false;
}

bool SGXPlayerState::destroyEPG()
{
	return false;
}

bool SGXPlayerState::destroyBlob()
{
	return false;
}

bool SGXPlayerState::checkMovieAllowance(size_t movie_id, time_t date)
{
	return false;
}
