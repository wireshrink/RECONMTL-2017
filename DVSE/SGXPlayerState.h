#pragma once
#include <time.h>

class SGXPlayerState
{
protected:
	int balance;
	char folder[1024];
	char address[1024];
	int port;

	unsigned char *couponMask;
	unsigned int numCoupons;
	bool valid;

	char *p_decoded_epg;
	size_t p_decoded_epg_size;
	char *p_decoded_blob; // decoded blob should contain the data about payment status of each movie
	size_t p_decoded_blob_size;
	char *p_decoded_coupons;
	size_t p_decoded_coupons_size;

public:
	int getBalance() { return balance; }
	void setBalance(int balance_to_set) { balance = balance_to_set; }
	bool couponUsed(int index);
	int couponCount() { return numCoupons; }
	char * getCoupon(int index);

	int getPort() {return port;}
	char * getEPG() { return p_decoded_epg; }
	char * getHostAddress() { return address; }
	char * getFolder() { return folder; }
	bool isValid() { return valid; }
	void setValid(bool validity) { valid = validity; }
	bool setConnectionProps(char * addr, int port);
	bool init_new_user(char* epg_file, char* blob_file, char*coupon_file, char*address, int port);
	bool setBlob(unsigned char* encryptedBlob, size_t blobSize);
	bool setCoupon(unsigned char* encryptedCoupon, size_t blobSize);
	bool setEPG(unsigned char* encryptedEPG, size_t blobSize);
	bool destroyCoupon();
	bool destroyEPG();
	bool destroyBlob();
	// also decreases the balance if applicable
	bool checkMovieAllowance(size_t movie_id, time_t date);
	SGXPlayerState();
	~SGXPlayerState();
};

// assuming that EPG is already loaded
// epg line format:
// 0x%16x(id), [1024] name, [string:30] last date, [bool:5] payment required

#define EPG_ID_LINE_OFFSET		0
#define EPG_ID_LINE_SIZE		18
#define EPG_NAME_LINE_OFFSET	(EPG_ID_LINE_OFFSET + EPG_ID_LINE_SIZE)
#define EPG_NAME_LINE_SIZE		1024
#define EPG_DATE_LINE_OFFSET	(EPG_NAME_LINE_OFFSET + EPG_NAME_LINE_SIZE)
#define EPG_DATE_LINE_SIZE		30
#define EPG_NEEDPAY_LINE_OFFSET (EPG_DATE_LINE_OFFSET + EPG_DATE_LINE_SIZE)
#define EPG_NEEDPAY_LINE_SIZE	5
#define EPG_WHOLE_LINE_SIZE		(EPG_NEEDPAY_LINE_OFFSET + EPG_NEEDPAY_LINE_SIZE)
