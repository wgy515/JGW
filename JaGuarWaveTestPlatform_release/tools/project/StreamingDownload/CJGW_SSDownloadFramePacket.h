#pragma once
#include "contents_xml/CRCCCITT.h"
#include "9x25_download_define.h"

namespace JGW
{
    class CCJGW_SSDownloadFramePacket
    {
    public:
        CCJGW_SSDownloadFramePacket(void);
        ~CCJGW_SSDownloadFramePacket(void);
        //! 构造Streaming_Download_Cmd栈，返回frame_buf 大小
        size_t BuildCMDFrameStreamPacket(BYTE* pStreamPacket,size_t nStreamPacketLen,BYTE** frame_buf);

        size_t BuildDataStreamPacket(BYTE* pStreamCMDPacket,size_t nStreamPacketLen,BYTE** frame_buf);

        size_t BuildDataStreamPacketToBuf(BYTE* pStreamCMDPacket,size_t nStreamPacketLen,char* frame_buf);
    private:
        CCRCCCITT m_crcCCITT;
    };
}


