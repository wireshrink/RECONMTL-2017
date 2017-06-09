
#ifndef SGXBLOB_H
#define SGXBLOB_H
#include "SGXServiceFile.h"



typedef struct __dvse_blob_header_t
{
	unsigned char token[8];
	size_t  balance;
	size_t  movie_data_count;
	size_t  used_coupon_count;
}dvse_blob_header_t;


typedef struct __dvse_entry_placeholder_t
{
	char used_coupon[32];
}dvse_entry_placeholder_t;


typedef struct __dvse_used_coupon_data_t
{
	char used_coupon[32];
}dvse_used_coupon_data_t;


typedef struct __dvse_blob_movie_data_t
{
	size_t movie_id;
	size_t  is_free_for_view;
	unsigned char last_allowed_date[16];
}dvse_blob_movie_data_t;

typedef struct __dvse_blob_structure_t
{
	dvse_blob_header_t hdr;
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning ( disable:4200 )
#endif
	dvse_entry_placeholder_t entries[];
#ifdef _MSC_VER
#pragma warning(pop)
#endif

}dvse_blob_structure_t;




// offset functions:
inline dvse_blob_header_t *const dvse_blob_header(unsigned char* const data)
{
	return reinterpret_cast<dvse_blob_header_t *const>(data);
}

inline dvse_blob_structure_t * const blob_as_struct(unsigned char* const data)
{
	return reinterpret_cast<dvse_blob_structure_t *const>(data);
}

inline dvse_entry_placeholder_t *const getplaceholder(unsigned char * const data, int index)
{
	dvse_blob_structure_t * const blobdata = blob_as_struct(data);
	if (index < 0 || index >= (blobdata->hdr.movie_data_count + blobdata->hdr.used_coupon_count))
	{
		return nullptr;
	}
	return &blobdata->entries[index];
}

class SGXBlob : public SGXServiceFile
{
public:

  SGXBlob ();

  virtual ~SGXBlob ();

  

  /**
   * @return bool
   * @param  movie_id
   */
  bool isMoviePlayAllowed(size_t movie_id);

  bool isOvertime(unsigned char thetime[16]);
  bool purchaseMovie(size_t movie_id);

  bool isCouponAlreadyUsed(char *coupon);
  bool setCouponAlreadyUsed(char *coupon);


  bool create_default_blob()
  {
	  return download();
  }

  virtual bool download()
  {
	  return downloadById("blob");
  }

  unsigned int getMovieCount();
  unsigned int getUsedCouponCount();
  dvse_blob_movie_data_t *const  getMovie(unsigned int index);
  dvse_used_coupon_data_t *const getUsedCoupon(unsigned int index);

  /**
   * Set the value of balance
   * @param new_var the new value of balance
   */
  bool setBalance(int new_var);

  /**
   * Get the value of balance
   * @return the value of balance
   */
  int getBalance();
private:


  void initAttributes () ;

};

#endif // SGXBLOB_H
