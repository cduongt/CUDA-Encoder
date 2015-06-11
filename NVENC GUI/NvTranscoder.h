////////////////////////////////////////////////////////////////////////////
//
// Copyright 1993-2014 NVIDIA Corporation.  All rights reserved.
//
// Please refer to the NVIDIA end user license agreement (EULA) associated
// with this source code for terms and conditions that govern your use of
// this software. Any use, reproduction, disclosure, or distribution of
// this software and related documentation outside the terms of the EULA
// is strictly prohibited.
//
////////////////////////////////////////////////////////////////////////////

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