/**
*   Copyright(C) 2011-2016 Intel Corporation All Rights Reserved.
*
*   The source code, information  and  material ("Material") contained herein is
*   owned  by Intel Corporation or its suppliers or licensors, and title to such
*   Material remains  with Intel Corporation  or its suppliers or licensors. The
*   Material  contains proprietary information  of  Intel or  its  suppliers and
*   licensors. The  Material is protected by worldwide copyright laws and treaty
*   provisions. No  part  of  the  Material  may  be  used,  copied, reproduced,
*   modified, published, uploaded, posted, transmitted, distributed or disclosed
*   in any way  without Intel's  prior  express written  permission. No  license
*   under  any patent, copyright  or  other intellectual property rights  in the
*   Material  is  granted  to  or  conferred  upon  you,  either  expressly,  by
*   implication, inducement,  estoppel or  otherwise.  Any  license  under  such
*   intellectual  property  rights must  be express  and  approved  by  Intel in
*   writing.
*
*   *Third Party trademarks are the property of their respective owners.
*
*   Unless otherwise  agreed  by Intel  in writing, you may not remove  or alter
*   this  notice or  any other notice embedded  in Materials by Intel or Intel's
*   suppliers or licensors in any way.
*/

// test_vrfcert.cpp : Defines the entry point for the console application.
#include "vrfcert_u.h"
#include "sgx_uae_service.h"

#include <errno.h>
#include <stdio.h>
#ifdef _MSC_VER
#include <tchar.h>
#endif

#ifndef _MSC_VER
#define TCHAR           char
#define _TCHAR          char
#define _T(str)         str
#define fprintf_s       fprintf
#define _tmain          main
#endif


#ifdef _MSC_VER
#define ENCLAVE_NAME _T("vrfcert.signed.dll")
#else
#define ENCLAVE_NAME "libvrfcert_enclave.so"
#endif


#define USAGE \
        "Usage: test_vrfcert <Cert File> \n" \
        "   <Cert File>     The provided cert list files.\n\n"\
        "If 'valid_certlist.txt' is provided, success will be returned.\n"\
        "Otherwise, you will get failure."

static sgx_launch_token_t token ={0};//lauch with empty token
//Function to load the enclave and relative file name is used
//It is assumed that the project is started at the $(OutDir) 
//  when the signed enclave dll is generated
static sgx_status_t load_enclave(sgx_enclave_id_t *eid)
{
    int launch_token_update = 0;
    sgx_status_t ret;

    ret = sgx_create_enclave(ENCLAVE_NAME, SGX_DEBUG_FLAG, &token,
         &launch_token_update, eid, NULL);
    return ret;
}

//Function to read all data of the file into a buffer and return pointer to the buffer
// use function free_file_data to free the buffer on success
// The size of the buffer is returned in reference parameter size
uint8_t *read_file_data(TCHAR *filename, long& size)
{
    FILE *file_cert_chain = NULL;
    uint8_t *p = NULL;
    long fsize = 0;
    size = 0;
#ifdef _MSC_VER
    errno = _tfopen_s(&file_cert_chain, filename, _T("rb"));
    if(errno!=0 || file_cert_chain == NULL)
    {
#else
    file_cert_chain = fopen(filename, "rb");
    if(file_cert_chain == NULL)
    {
#endif
        goto ret_point;
    }
    if(fseek(file_cert_chain, 0, SEEK_END)!=0)
        goto ret_point;
    fsize = ftell(file_cert_chain);
    if(fsize <=0 ) goto ret_point;

    size = fsize;
    p = (uint8_t *)malloc(fsize);
    if(p == NULL) goto ret_point;
    if(fseek(file_cert_chain, 0, SEEK_SET)!=0)
    {
        free(p);
        p=NULL;
        goto ret_point;
    }
    if(fread(p, 1, fsize, file_cert_chain)!=fsize)
    {
        free(p);
        p=NULL;
        goto ret_point;
    }
ret_point:
    if(file_cert_chain)fclose(file_cert_chain);
    if(p == NULL)
        fprintf_s(stderr,"Fail to read the cert data.\n");
    return p;
}

void free_file_data(uint8_t *p)
{
    free(p);
}

int _tmain(int argc, _TCHAR* argv[])
{
    sgx_enclave_id_t eid = 0;
    int ret = -1;
    long size = 0;
    uint8_t * p = NULL;
    if(argc != 2)
    {
        fprintf_s(stderr, ("Invalid input command.\n\n%s\n"), USAGE);
        ret = -1;
        goto ret_point;
    }

    p = read_file_data(argv[1], size);
    if(p == NULL)
    {
        ret = -1;
        goto ret_point;
    }

    if(load_enclave(&eid)!=SGX_SUCCESS)
    {
        fprintf_s(stderr,"Fail to load enclave\n");
        ret = -1;
        goto ret_point;
    }

    //Function to call verification code inside enclave
    if(verify_cert_in_perm(eid, &ret, p, size)!=SGX_SUCCESS)
    {
        ret = -1;
    }

    if(ret == 0)
    {
        printf("Verify passed\n");
    }
    else
    {
        printf("Verify failed\n");
    }

    sgx_destroy_enclave(eid);

ret_point:
    if(p) free_file_data(p);

    printf("Enter a character before exit ...\n");
    getchar();
    return ret;
}

