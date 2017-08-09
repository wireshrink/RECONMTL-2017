#pragma once


bool load_enclave();
bool unload_enclave();
bool init_enclave(char* netaddr, char* libaddr, unsigned int port);
bool get_epg_info( size_t ** movie_ids, bool **is_payed, unsigned int *num_of_movies);
bool prepare_file(size_t id);
bool extract_semi_allowed_file(size_t movie_id, char* store_to);
bool apply_coupon(char * coupon);
bool get_file_size(size_t movie_id, size_t * fsize);
size_t	read_movie_chunk(size_t movie_offset, size_t chunk_size, unsigned char* dest);
bool write_app_log(unsigned char* data, size_t length);




