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

#ifndef _VIDEO_DECODER
#define _VIDEO_DECODER

#include <nvcuvid.h>
#include <cuda.h>
#include "FrameQueue.h"

class CudaDecoder
{
public:
    CudaDecoder();
    virtual ~CudaDecoder(void);

    bool IsFinished()            { return m_bFinish; }
    virtual void InitVideoDecoder(const char* videoPath, CUvideoctxlock ctxLock, FrameQueue* pFrameQueue,
            int targetWidth = 0, int targetHeight = 0);
    virtual void Start();
    virtual void GetCodecParam(int* width, int* height, int* frame_rate_num, int* frame_rate_den);
    virtual void* GetDecoder()   { return m_videoDecoder; }

public:
    CUvideosource  m_videoSource;
    CUvideoparser  m_videoParser;
    CUvideodecoder m_videoDecoder;
    CUvideoctxlock m_ctxLock;
    CUVIDDECODECREATEINFO m_oVideoDecodeCreateInfo;

    FrameQueue*    m_pFrameQueue;
    int            m_decodedFrames;

protected:
    bool m_bFinish;
};

#endif
