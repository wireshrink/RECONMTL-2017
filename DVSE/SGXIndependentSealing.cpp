#include "SGXIndependentSealing.h"


bool SGXIndependentSealing::generate_secure_random_iv(unsigned char iv[16])
{
	return false;
}

SGXIndependentSealing::SGXIndependentSealing()
{
}


SGXIndependentSealing::~SGXIndependentSealing()
{
}

size_t SGXIndependentSealing::calc_sealed_data_size(size_t data_size)
{
	return 0;
}

bool SGXIndependentSealing::seal_data(unsigned char * in, size_t in_size, unsigned char ** out, size_t * poutsize)
{
	return false;
}

bool SGXIndependentSealing::unseal_data(unsigned char * in, size_t in_size, unsigned char ** out, size_t * poutsize)
{
	return false;
}

bool SGXIndependentSealing::destroy_allocated_data(unsigned char * data)
{
	
	return false;
}
