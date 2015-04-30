#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <stdio.h>
#include <string.h>
#include <cuda.h>
#include <cuda_runtime.h>

#include "VideoDecoder.h"
#include "VideoEncoder.h"
#include "inc/nvUtils.h"
#include "nvencgui.h"


#ifdef _WIN32
DWORD WINAPI DecodeProc(LPVOID lpParameter);

#else
void* DecodeProc(void *arg);
#endif

void PrintHelp();