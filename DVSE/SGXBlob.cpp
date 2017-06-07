#include "SGXEcallEnclaveInterface.h"
#include "SGXBlob.h"
#include <stdlib.h>

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

bool SGXBlob::setBalance(int new_var) {
	if (this->get_data_size() < sizeof(dvse_blob_header_t))
		return false;
	dvse_blob_header_t * pheader = (dvse_blob_header_t*)getContent();

	pheader->balance = new_var;
	return encrypt_and_save();
}
// should also save the blob
bool SGXBlob::purchaseMovie(size_t movie_id)
{
	unsigned int i;

	// each movie costs $50
	if (getBalance() < 50)
	{
		return false;
	}
	
	unsigned char curtime[16];
	int res;

	sgx_status_t ret = ocall_get_the_current_time(&res, curtime);
	if (ret != SGX_SUCCESS)
	{
		return false;
	}
	
	int new_balance = getBalance() - 50;
	for (i = 0; i < getMovieCount(); i++)
	{
		if (getMovie(i)->movie_id == movie_id)
		{
			// no need to rewrite all the blob, only changing date and balance
			dvse_blob_header_t * pheader = (dvse_blob_header_t*)getContent();
			pheader->balance = new_balance;
			//probably incorrectly assuming time is in seconds, adding ...
			//microsoft says https://msdn.microsoft.com/query/dev14.query?appId=Dev14IDEF1&l=EN-US&k=k(TIME%2Ftime);k(time);k(DevLang-C%2B%2B);k(TargetOS-Windows)&rd=true
			//so it should be correct according to MSDN
			time_t* pt = (time_t*)curtime;
			(*pt) += 60 * 60 * 2; // adding 2 hours, we are generous
			memcpy(getMovie(i)->last_allowed_date, curtime, 16);
			return encrypt_and_save();
		}
	}
	// no movie found, adding new one

	size_t new_size = get_data_size() + sizeof(dvse_blob_movie_data_t);
	unsigned char *new_data = (unsigned char*)malloc(new_size);

	if (!new_data)
	{
		return false;
	}

	dvse_blob_header_t * pheader = (dvse_blob_header_t*)getContent();
	dvse_blob_header_t * pnewheader = (dvse_blob_header_t*)new_data;

	*pnewheader = *pheader;


	dvse_movie_header_t *pmovieheader = (dvse_movie_header_t*)(getContent() + sizeof(dvse_blob_header_t));
	dvse_movie_header_t *pnewmovieheader = (dvse_movie_header_t*)(new_data + sizeof(dvse_blob_header_t));

	*pnewmovieheader = *pmovieheader;
	pnewmovieheader->movie_count++;

	for (i = 0; i < pmovieheader->movie_count; i++)
	{
		pnewmovieheader->movies[i] = pmovieheader->movies[i];
	}
	// at this time i means last one
	pnewmovieheader->movies[i].movie_id = movie_id;
	pnewmovieheader->movies[i].is_free_for_view = SGXEcallEnclaveInterface::getInstance()->isFreeToPlay(movie_id);
	memcpy(pnewmovieheader->movies[i].last_allowed_date, curtime, 16);
	
	dvse_used_coupon_header_t *pusedcouponsheader = (dvse_used_coupon_header_t *)(((unsigned char*)pmovieheader) +
		sizeof(dvse_movie_header_t) +
		(pmovieheader->movie_count * sizeof(dvse_blob_movie_data_t)));

	dvse_used_coupon_header_t *pnewusedcouponsheader = (dvse_used_coupon_header_t *)(((unsigned char*)pnewmovieheader) +
		sizeof(dvse_movie_header_t) +
		(pnewmovieheader->movie_count * sizeof(dvse_blob_movie_data_t)));

	*pnewusedcouponsheader = *pusedcouponsheader;

	for (i = 0; i < pusedcouponsheader->used_coupon_count; i++)
	{
		pnewusedcouponsheader->used_coupons[i] = pusedcouponsheader->used_coupons[i];
	}

	return encrypt_and_save();
}

// should also save the blob
bool SGXBlob::setCouponAlreadyUsed(char * coupon)
{
	unsigned int i;
	if (isCouponAlreadyUsed(coupon))
	{
		return true; // already used, no need to set
	}
	size_t new_size = get_data_size() + sizeof(dvse_used_coupon_data_t);
	unsigned char* new_data = (unsigned char*)malloc(new_size);
	
	if (!new_data) return false;

	dvse_blob_header_t * pheader = (dvse_blob_header_t*)getContent();
	dvse_blob_header_t * pnewheader = (dvse_blob_header_t*)new_data;

	*pnewheader = *pheader;


	dvse_movie_header_t *pmovieheader = (dvse_movie_header_t*)(getContent() + sizeof(dvse_blob_header_t));
	dvse_movie_header_t *pnewmovieheader = (dvse_movie_header_t*)(new_data + sizeof(dvse_blob_header_t));

	*pnewmovieheader = *pmovieheader;

	for (i = 0; i < pmovieheader->movie_count; i++)
	{
		pnewmovieheader->movies[i] = pmovieheader->movies[i];
	}
	
	dvse_used_coupon_header_t *pusedcouponsheader = (dvse_used_coupon_header_t *)(((unsigned char*)pmovieheader) +
		sizeof(dvse_movie_header_t) +
		(pmovieheader->movie_count * sizeof(dvse_blob_movie_data_t)));

	dvse_used_coupon_header_t *pnewusedcouponsheader = (dvse_used_coupon_header_t *)(((unsigned char*)pnewmovieheader) +
		sizeof(dvse_movie_header_t) +
		(pnewmovieheader->movie_count * sizeof(dvse_blob_movie_data_t)));

	*pnewusedcouponsheader = *pusedcouponsheader;

	for (i = 0; i < pusedcouponsheader->used_coupon_count; i++)
	{
		pnewusedcouponsheader->used_coupons[i] = pusedcouponsheader->used_coupons[i];
	}

	pnewusedcouponsheader->used_coupon_count++;

	memcpy(pusedcouponsheader->used_coupons[pnewusedcouponsheader->used_coupon_count - 1].used_coupon, coupon, 32);

	if (!set_decrypted_content(new_size, new_data))
	{
		free(new_data);
		return false;
	}



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

