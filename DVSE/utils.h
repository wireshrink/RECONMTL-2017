#pragma once

bool read_whole_file(char* filename, size_t *pfilesize, unsigned char **data); // allocated inside
bool destroy_read_data(unsigned char *data);