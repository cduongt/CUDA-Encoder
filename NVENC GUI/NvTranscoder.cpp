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

void PrintHelp()
{
    printf("Usage : NvTranscoder \n"
        "-i <string>                  Specify input .h264 file\n"
        "-o <string>                  Specify output bitstream file\n"
        "\n### Optional parameters ###\n"
        "-size <int int>              Specify output resolution <width height>\n"
        "-codec <integer>             Specify the codec \n"
        "                                 0: H264\n"
        "                                 1: HEVC\n"
        "-preset <string>             Specify the preset for encoder settings\n"
        "                                 hq : nvenc HQ \n"
        "                                 hp : nvenc HP \n"
        "                                 lowLatencyHP : nvenc low latency HP \n"
        "                                 lowLatencyHQ : nvenc low latency HQ \n"
        "                                 lossless : nvenc Lossless HP \n"
        "-fps <integer>               Specify encoding frame rate\n"
        "-goplength <integer>         Specify gop length\n"
        "-numB <integer>              Specify number of B frames\n"
        "-bitrate <integer>           Specify the encoding average bitrate\n"
        "-vbvMaxBitrate <integer>     Specify the vbv max bitrate\n"
        "-vbvSize <integer>           Specify the encoding vbv/hrd buffer size\n"
        "-rcmode <integer>            Specify the rate control mode\n"
        "                                 0:  Constant QP\n"
        "                                 1:  Single pass VBR\n"
        "                                 2:  Single pass CBR\n"
        "                                 4:  Single pass VBR minQP\n"
        "                                 8:  Two pass frame quality\n"
        "                                 16: Two pass frame size cap\n"
        "                                 32: Two pass VBR\n"
        "-qp <integer>                Specify qp for Constant QP mode\n"
        "-deviceID <integer>          Specify the GPU device on which encoding will take place\n"
        "-help                        Prints Help Information\n\n"
        );
}



