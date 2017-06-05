#ifndef SGXSSL_TEXCEPTION_H__
#define SGXSSL_TEXCEPTION_H__

/*
this linker comment makes sure that the linker will not do any optimization on this section and throw it out
this function is called when the enclave is loaded for the first time
in this function we initialize the cpuid fields and other things needed for the correct operation of openssl
*/
#ifdef _WIN64
#pragma comment(linker, "/include:init_exception_handler")
#else
#pragma comment(linker, "/include:_init_exception_handler")
#endif

#endif