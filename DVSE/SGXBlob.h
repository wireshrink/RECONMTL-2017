
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
  dvse_blob_movie_data_t *getMovie(unsigned int index);
  dvse_used_coupon_data_t *getUsedCoupon(unsigned int index);

  /**
   * Set the value of balance
   * @param new_var the new value of balance
   */
  void setBalance(int new_var);

  /**
   * Get the value of balance
   * @return the value of balance
   */
  int getBalance();
private:


  void initAttributes () ;

};

#endif // SGXBLOB_H
