#pragma once
#include "SGXIndependentSealing.h"
class EncryptedFileReader
{
	unsigned char sliding_window[10 * 1024];
	
	size_t sliding_window_offset;
	size_t filesize;
	void *f;

public:
	EncryptedFileReader();
	~EncryptedFileReader();

	bool open(char* filename, char* format);
	bool setpos(size_t pos);
	bool read_decrypted_data(size_t pos, unsigned char* buffer, size_t size);
	bool sync_sliding_window(size_t pos);
};

