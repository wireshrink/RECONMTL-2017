#pragma once


// incapsulating independent sealing 
// the goal - replace rdrand with another RNG, using rdrand as seed
// additional goal - generate additional IV for AES-GCM (instead of 0 as in AES-GCM implementation on Linux).
class SGXIndependentSealing
{
	static bool generate_secure_random_iv(unsigned char iv[16]);
public:
	SGXIndependentSealing();
	~SGXIndependentSealing();
	
	static size_t calc_sealed_data_size(size_t data_size);
	static bool   seal_data(unsigned char* in, size_t in_size, unsigned char**out, size_t *poutsize); // memomry allocated inside
	static bool   unseal_data(unsigned char* in, size_t in_size, unsigned char**out, size_t *poutsize); // memomry allocated inside
	static bool   destroy_allocated_data(unsigned char* data);

	typedef struct __ind_sealing_header
	{
	}ind_sealing_header_t;

	typedef enum __chunk_size_consts
	{
		SEALING_HEADER_SIZE = sizeof(ind_sealing_header_t),
		UNSEALED_DATA_CHUNK_SIZE = 1024,
		SEALED_DATA_CHUNK_SIZE = UNSEALED_DATA_CHUNK_SIZE + SEALING_HEADER_SIZE,

	} chunk_size_consts_t;
};

