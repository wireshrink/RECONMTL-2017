#include "common_enclave_actions.h"

bool load_enclave()
{
	return false;
}

bool unload_enclave()
{
	return false;
}

bool init_enclave(char * netaddr, char * libaddr, unsigned int port)
{
	return false;
}

bool get_epg_info(char ** movie_names, size_t ** movie_ids, bool ** is_payed, unsigned int * num_of_movies)
{
	return false;
}

bool prepare_file(size_t id)
{
	return false;
}

bool extract_semi_allowed_file(size_t movie_id, char * store_to)
{
	return false;
}
