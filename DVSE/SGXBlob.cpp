#include "SGXEcallEnclaveInterface.h"
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
	if (this->get_data_size() < sizeof(dvse_blob_header_t))
		return false;
	unsigned int i;
	for (i = 0; i < getMovieCount(); i++)
	{
		dvse_blob_movie_data_t* pm = getMovie(i);
		if (pm->movie_id == movie_id)
		{
			if (pm->is_free_for_view)
			{
				return true; // if it was once free for view it will remain free for view, we are not that evil
			}
			if (!isOvertime(pm->last_allowed_date))
			{
				return true;
			}
		}

	}
	return false;
}
bool SGXBlob::isOvertime(unsigned char thetime[16])
{
	unsigned char curtime[16];
	int res;
	sgx_status_t ret = ocall_get_the_current_time(&res, curtime);
	if (ret != SGX_SUCCESS)
	{
		return false;
	}
	time_t *ptm0 = (time_t*)thetime;
	time_t *ptm1 = (time_t*)curtime;

	// this should be long long, right ?
	return difftime( (*ptm0) , (*ptm1)) >= 0;
}

bool SGXBlob::isCouponAlreadyUsed(char * coupon)
{
	unsigned int i;
	unsigned int count = getUsedCouponCount();
	for (i = 0; i < count; i++)
	{
		if (!strncmp(coupon, getUsedCoupon(i)->used_coupon, 32))
			return true;
	}
	return false;
}

unsigned int SGXBlob::getMovieCount()
{
	if (this->get_data_size() < sizeof(dvse_blob_header_t))
		return 0;
	dvse_blob_header_t * pheader = (dvse_blob_header_t*)getContent();
	dvse_movie_header_t *pmovieheader = (dvse_movie_header_t*)(getContent() + sizeof(dvse_blob_header_t));
	return pmovieheader->movie_count;
}

unsigned int SGXBlob::getUsedCouponCount()
{
	if (this->get_data_size() < sizeof(dvse_blob_header_t))
		return false;
	dvse_blob_header_t * pheader = (dvse_blob_header_t*)getContent();
	dvse_movie_header_t *pmovieheader = (dvse_movie_header_t*)(getContent() + sizeof(dvse_blob_header_t));
	dvse_used_coupon_header_t *pusedcouponsheader = (dvse_used_coupon_header_t *)(((unsigned char*)pmovieheader) +
		sizeof(dvse_movie_header_t) +
		(pmovieheader->movie_count * sizeof(dvse_blob_movie_data_t)) );
	return pusedcouponsheader->used_coupon_count;
	
}

dvse_blob_movie_data_t * SGXBlob::getMovie(unsigned int index)
{
	if (this->get_data_size() < sizeof(dvse_blob_header_t))
		return nullptr;
	dvse_blob_header_t * pheader = (dvse_blob_header_t*)getContent();
	dvse_movie_header_t *pmovieheader = (dvse_movie_header_t*)(getContent() + sizeof(dvse_blob_header_t));

	return &pmovieheader->movies[index];
}

dvse_used_coupon_data_t * SGXBlob::getUsedCoupon(unsigned int index)
{
	if (this->get_data_size() < sizeof(dvse_blob_header_t))
		return false;
	dvse_blob_header_t * pheader = (dvse_blob_header_t*)getContent();
	dvse_movie_header_t *pmovieheader = (dvse_movie_header_t*)(getContent() + sizeof(dvse_blob_header_t));
	dvse_used_coupon_header_t *pusedcouponsheader = (dvse_used_coupon_header_t *)(((unsigned char*)pmovieheader) +
		sizeof(dvse_movie_header_t) +
		(pmovieheader->movie_count * sizeof(dvse_blob_movie_data_t)));
	return &pusedcouponsheader->used_coupons[index];
}


/**
* Set the value of balance
* @param new_var the new value of balance
*/

void SGXBlob::setBalance(int new_var) {
	if (this->get_data_size() < sizeof(dvse_blob_header_t))
		return;
	dvse_blob_header_t * pheader = (dvse_blob_header_t*)getContent();

	pheader->balance = new_var;
	encrypt_and_save();
}
// should also save the blob
bool SGXBlob::purchaseMovie(size_t movie_id)
{


	return encrypt_and_save();
}

// should also save the blob
bool SGXBlob::setCouponAlreadyUsed(char * coupon)
{
	return encrypt_and_save();
}

/**
* Get the value of balance
* @return the value of balance
*/

int SGXBlob::getBalance() {
	if (this->get_data_size() < sizeof(dvse_blob_header_t))
		return 0;
	dvse_blob_header_t * pheader = (dvse_blob_header_t*)getContent();
	return pheader->balance;
}

void SGXBlob::initAttributes () {
}

