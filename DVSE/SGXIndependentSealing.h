#pragma once
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

};

