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


#include "SGXEcallEnclaveInterface.h"
#include "SGXBlob.h"
#include <stdlib.h>

// Constructors/Destructors
//  

SGXBlob::SGXBlob () {
initAttributes();
static_assert(sizeof(dvse_entry_placeholder_t) == sizeof(dvse_blob_movie_data_t), "NEED mode buffer for time repr");
static_assert(sizeof(dvse_entry_placeholder_t) == sizeof(dvse_used_coupon_data_t), "NEED mode buffer for time repr");
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
		dvse_blob_movie_data_t* const pm = getMovie(i);
		if (pm->movie_id == movie_id)
		{
			if (pm->is_free_for_view)
			{
				return true; // if it was once free for view it will remain free for view, we are not that evil
			}
			if (!isOvertime((unsigned char*)pm->last_allowed_date))
			{
				return true;
			}
			return false;
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
	return (unsigned int)blob_as_struct(getContent())->hdr.movie_data_count;
}

unsigned int SGXBlob::getUsedCouponCount()
{
	return (unsigned int)blob_as_struct(getContent())->hdr.used_coupon_count;
}

dvse_blob_movie_data_t * const SGXBlob::getMovie(unsigned int index)
{
	dvse_blob_structure_t *const blobdata = blob_as_struct(getContent());
	if (this->get_data_size() <= sizeof(dvse_blob_header_t))
		return nullptr;
	if (blobdata->hdr.movie_data_count < index)
		return nullptr;

	return reinterpret_cast<dvse_blob_movie_data_t *const>(&blobdata->entries[index]);
}

dvse_used_coupon_data_t * const SGXBlob::getUsedCoupon(unsigned int index)
{
	dvse_blob_structure_t *const blobdata = blob_as_struct(getContent());
	if (this->get_data_size() < sizeof(dvse_blob_header_t))
		return nullptr;
	if (blobdata->hdr.used_coupon_count <= index  )
		return nullptr;

	return reinterpret_cast<dvse_used_coupon_data_t *const>(&blobdata->entries[blobdata->hdr.movie_data_count + index]);
}


/**
* Set the value of balance
* @param new_var the new value of balance
*/

bool SGXBlob::setBalance(int new_var) {
	if (this->get_data_size() < sizeof(dvse_blob_header_t))
		return false;
	dvse_blob_header_t * const pheader = dvse_blob_header(getContent());

	pheader->balance = new_var;
	return encrypt_and_save();
}
// should also save the blob
bool SGXBlob::purchaseMovie(size_t movie_id)
{
	unsigned int i;
	bool freetoplay = SGXEcallEnclaveInterface::getInstance()->isFreeToPlay(movie_id);
	// each movie costs $50
	if (getBalance() < 50 && !freetoplay)
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
	
	int new_balance = getBalance() ;
	if (!freetoplay)
	{
		new_balance -= 50;
	}
	
	for (i = 0; i < getMovieCount(); i++)
	{
		if (getMovie(i)->movie_id == movie_id)
		{
			// no need to rewrite all the blob, only changing date and balance
			dvse_blob_header_t * const pheader = dvse_blob_header(getContent());
			pheader->balance = new_balance;
			//probably incorrectly assuming time is in seconds, adding ...
			//microsoft says https://msdn.microsoft.com/query/dev14.query?appId=Dev14IDEF1&l=EN-US&k=k(TIME%2Ftime);k(time);k(DevLang-C%2B%2B);k(TargetOS-Windows)&rd=true
			//so it should be correct according to MSDN
			time_t* pt = (time_t*)curtime;
			(*pt) += 60 * 60 * 2; // adding 2 hours, we are generous
			memcpy((void*)getMovie(i)->last_allowed_date, curtime, 16);
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

	dvse_blob_header_t * const pheader = dvse_blob_header(getContent());
	dvse_blob_header_t * const pnewheader = dvse_blob_header(new_data);
	pheader->balance = new_balance;
	*pnewheader = *pheader;

	dvse_blob_structure_t *const pstruct = blob_as_struct(getContent());
	dvse_blob_structure_t *const pnewstruct = blob_as_struct(new_data);

	pnewheader->movie_data_count++;

	for (i = 0; i < pheader->movie_data_count; i++)
	{
		pnewstruct->entries[i] = pstruct->entries[i];
	}
	// adding last movie in the middle of the data
	dvse_blob_movie_data_t *pnewmovie = reinterpret_cast<dvse_blob_movie_data_t *const>(getplaceholder(new_data, (unsigned int)pheader->movie_data_count));
	pnewmovie->movie_id = movie_id;
	pnewmovie->is_free_for_view = freetoplay;
	memcpy(pnewmovie->last_allowed_date, curtime, 16);

	for (i = 0; i < pheader->used_coupon_count; i++)
	{
		pnewstruct->entries[i+pnewheader->movie_data_count] = pstruct->entries[i+pheader->movie_data_count];
	}

	if (!set_decrypted_content(new_size, new_data))
	{	
		free(new_data);
		return false;
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

	dvse_blob_header_t * const pheader = dvse_blob_header(getContent());
	dvse_blob_header_t * const pnewheader = dvse_blob_header(new_data);

	*pnewheader = *pheader;

	dvse_blob_structure_t *const pstruct = blob_as_struct(getContent());
	dvse_blob_structure_t *const pnewstruct = blob_as_struct(new_data);

	pnewheader->used_coupon_count++;

	for (i = 0; i < (pheader->used_coupon_count + pheader->movie_data_count); i++)
	{
		pnewstruct->entries[i] = pstruct->entries[i];
	}
	//adding the coupon to the end
	memcpy(getplaceholder(new_data, (unsigned int)(pheader->used_coupon_count + pheader->movie_data_count)), coupon, 32);


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
	dvse_blob_header_t * const pheader = dvse_blob_header(getContent());
	return (int)pheader->balance;
}

void SGXBlob::initAttributes () {
}

