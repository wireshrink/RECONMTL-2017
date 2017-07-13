#pragma once


bool load_enclave();
bool unload_enclave();
bool init_enclave(char* netaddr, char* libaddr, unsigned int port);
bool get_epg_info(char ** movie_names, size_t ** movie_ids, bool **is_payed, unsigned int *num_of_movies);
bool prepare_file(size_t id);
bool extract_semi_allowed_file(size_t movie_id, char* store_to);


