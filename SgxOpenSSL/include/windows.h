
#ifndef __WINDOWS_H__
#define __WINDOWS_H__

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef _FILE_DEFINED
struct _iobuf {
	char *_ptr;
	int   _cnt;
	char *_base;
	int   _flag;
	int   _file;
	int   _charbuf;
	int   _bufsiz;
	char *_tmpfname;
};
typedef struct _iobuf FILE;
#define _FILE_DEFINED
#endif


FILE* __iob_func();

#ifdef  __cplusplus
}
#endif

#ifndef stdin
#define stdin  (&__iob_func()[0])
#endif
#ifndef stdout
#define stdout (&__iob_func()[1])
#endif
#ifndef stderr
#define stderr (&__iob_func()[2])
#endif

#ifndef UINT_PTR
#ifdef _WIN64
#define UINT_PTR uint64_t
#else
#define UINT_PTR uint32_t
#endif
#endif

#ifndef __int3264
#ifdef _WIN64
#define __int3264 int64_t
#else
#define __int3264 int32_t
#endif
#endif

#ifndef WPARAM
#define WPARAM UINT_PTR
#endif

#ifndef LONG_PTR
#define LONG_PTR __int3264 
#endif

#ifndef LPARAM
#define LPARAM LONG_PTR
#endif

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef UINT
#define UINT	uint32_t
#endif

#ifndef BOOL
#define BOOL	int32_t
#endif

#endif // __WINDOWS_H__
