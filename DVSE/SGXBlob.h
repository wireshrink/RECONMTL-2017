
#ifndef SGXBLOB_H
#define SGXBLOB_H
#include "SGXServiceFile.h"



typedef struct __dvse_blob_header_t
{
	unsigned char token[8];
	unsigned int  balance;
}dvse_blob_header_t;

typedef struct __dvse_blob_movie_data_t
{
	size_t movie_id;
	unsigned char last_allowed_date[16];
	unsigned int  is_free_for_view;
}dvse_blob_movie_data_t;
typedef struct __dvse_movie_header_t
{
	unsigned int movie_count;
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning ( disable:4200 )
#endif

	dvse_blob_movie_data_t movies[];
#ifdef _MSC_VER
#pragma warning(pop)
#endif
}dvse_movie_header_t;

typedef struct __dvse_used_coupon_data_t
{
	char used_coupon[32];
}dvse_used_coupon_data_t;

typedef struct __dvse_used_coupon_header_t
{
	unsigned int used_coupon_count;
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning ( disable:4200 )
#endif

	dvse_used_coupon_data_t used_coupons[];
#ifdef _MSC_VER
#pragma warning(pop)
#endif
}dvse_used_coupon_header_t;


// offset functions:
inline dvse_blob_header_t *const dvse_blob_header(unsigned char* const data)
{
	return reinterpret_cast<dvse_blob_header_t *const>(data);
}
inline  dvse_movie_header_t * const dvse_movie_header( unsigned char* const data)
{
	return reinterpret_cast<dvse_movie_header_t * const>(dvse_blob_header(data) + 1); // just after the header
}
inline dvse_used_coupon_header_t * const dvse_coupons_header(unsigned char* const data)
{
	dvse_movie_header_t * const movie_header = dvse_movie_header(data);
	unsigned char * const temp = reinterpret_cast<unsigned char* const>(movie_header + 1);// movies start
	return reinterpret_cast<dvse_used_coupon_header_t* const>(temp + (sizeof(dvse_blob_movie_data_t) * movie_header->movie_count));
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
