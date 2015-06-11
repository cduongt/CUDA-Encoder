#include "NvTranscoder.h"

#ifdef _WIN32
DWORD WINAPI DecodeProc(LPVOID lpParameter)
{
    CudaDecoder* pDecoder = (CudaDecoder*)lpParameter;
    pDecoder->Start();

    return 0;
}

#else
void* DecodeProc(void *arg)
{
    CudaDecoder* pDecoder = (CudaDecoder*)arg;
    pDecoder->Start();

    return NULL;
}

#endif