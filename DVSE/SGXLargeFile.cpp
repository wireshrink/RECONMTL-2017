#include "SGXLargeFile.h"
#include <stdio.h>

// Constructors/Destructors
//  

SGXLargeFile::SGXLargeFile () {
initAttributes();
}

SGXLargeFile::~SGXLargeFile () { }


bool SGXLargeFile::createMovieFileName(size_t movie_id, char * base_folder, char * buf, size_t buf_size)
{
	snprintf(buf, buf_size, "%s\\movie.%zx", base_folder, movie_id);
	return true;
}

void SGXLargeFile::initAttributes () {
	m_binitialized = false;
	handle = nullptr;
}

