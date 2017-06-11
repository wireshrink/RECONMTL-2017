/************************************************************************************************************
*	This application is a TRAINING TARGET for exercises in HACKING Intel® SGX ENCLAVES.                     *
*	This application made vulnerable DELIBERATELY - its main purpose is to demonstrate, shame and blame     *
*   common mistakes usually made with SGX enclave programming.                                              *
*   ONCE AGAIN, IT CONTAINS MISTAKES.                                                                       *
*   Please DO NOT USE it's source for any healthy development/production activity.                          *
*	It is intended to use for training of hacking SGX enclaves ONLY.                                        *
*	It is written ugly(deliberately), designed badly(intentionally) and running very slow.		        	*
*	You can freely use this application for training purposes.												*
*	I'd be glad to hear about your progress.    															*
*																											*
*	This application requires QT5.8 (which uses LGPL v3 license), Intel SGX SDK and							*
*   the Intel® Software Guard Extensions SSL (Intel® SGX SSL) to be compiled.								*
*	This application is written by Michael Atlas (wireshrink@gmail.com) during 2017.						*
*	Happy hacking.																							*
*************************************************************************************************************/

#include "SGXEpg.h"


SGXEpg::SGXEpg () {
}

SGXEpg::~SGXEpg () { }

/**
* @param  movie_id
*/

bool SGXEpg::isFreeToPlay(size_t movie_id)
{
	// parsing the data inside the enclave, 
	// The first time in the arena, a deadly stunt, hurry to see !
	unsigned char *ptr = (unsigned char*)getContent();
	unsigned char *end = ptr + get_data_size() + 1; // including trailing zero
	do
	{
		unsigned char *name = ptr;
		unsigned char *chid = (unsigned char*)memchr((const char*)name, ',', end - name) + 1; if (chid == (unsigned char *)1 || chid >= end) return false;
		unsigned char *tm = (unsigned char*)memchr((const char*)chid, ',', end - name) + 1; if (tm == (unsigned char *)1 || tm >= end) return false;
		unsigned char *stat = (unsigned char*)memchr((const char*)tm, ',', end - name) + 1; if (stat == (unsigned char *)1 || stat >= end) return false;

		if (strncmp((char*)stat, "free view", strlen("free view")) == 0)
		{
			// getting id
			size_t id = atoll((const char*)chid);
			if (id == movie_id) return true; // if its state is tree view, return true.
		}
		ptr = (unsigned char*)strchr((const char*)ptr, '\n') + 1;
	} while (ptr < end);

	return false;
}

/**
* @return bool
* @param  data_1024_
* @param  pageNum
*/

bool SGXEpg::getEPGPage(unsigned char data[1024], int pageNum)
{
	memcpy(data, this->getContent() + pageNum * 1024, 1024);
	return true;
}

bool SGXEpg::download()
{
	return downloadById("epg");
}

