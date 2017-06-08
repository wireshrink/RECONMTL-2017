#ifndef SGXWARE_H
#define SGXWARE_H
#include <time.h>
#include <QWidget>

typedef enum __movie_status
{
    M_STT_VOID,
    M_STT_DOWNLOADED,
    M_STT_LAST,
}movie_status_t;

typedef enum __movie_state
{
    M_STS_FREE,
    M_STS_PAY,
    M_STS_LAST
} movie_state_t;


typedef struct __movie
{
    unsigned int   movie_id;
    char           movie_name[1024];
    time_t         movie_expiration;
    movie_status_t movie_status;
    movie_state_t  movie_state;

} movie_t;

typedef enum __DRM_status
{
    DRM_BAD_DATE,
    DRM_BAD_ENCRYPTION,
    DRM_DOWNLOAD_IN_PROGRESS,
    DRM_NEED_MORE_MONEY,
    DRM_LAST
} DRM_status_t;

class SGXware
{
    SGXware();
    ~SGXware();
    static SGXware *m_pInstance;
	size_t epg_rdPtr;
	unsigned int epg_page_index;
	unsigned char secure_channel_key[16];
	char          current_epg_page[1024];
	char          base_folder[1024];
	bool createMovie(movie_t *pMovie);
	bool readUntil(char* buf, char delimiter);

public:
    static SGXware * getInstance();
    static void      destroyInstance();

    bool         initUser (char* address, int port, char* folder);
    //bool         getUserProps(char address[1024], int *port, char folder[1024] );
    bool         initSecureChannel(void);
    bool         getBalance(int *balance);
    bool         tryCoupon(char* coupon);

    bool         firstMovie(movie_t *pFirstMovie);
    bool         nextMovie(movie_t *pNextMovie);

    bool         prepareMovie(unsigned int movie_id);
    bool         refreshEPG(void);
    quint64 readMovieChunk(size_t movie_offset, size_t chunk_size, unsigned char* dest);
    bool         writeAppLog (unsigned char* data, size_t length);
    bool        getFileName(size_t id, size_t movie_name_size, char *movie_name);
	bool        inplaceDecrypt(qint64 size, void* data);
	bool        getFileSize(size_t movie_id, size_t* fsize);

};

const char* strMovieStatus(movie_status_t s);
const char* strMovieState(movie_state_t s);
const char* strExpiry(time_t time);

#endif // SGXWARE_H
