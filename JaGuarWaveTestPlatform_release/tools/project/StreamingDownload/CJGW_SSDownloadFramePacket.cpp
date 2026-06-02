#include "StdAfx.h"
#include "CJGW_SSDownloadFramePacket.h"

namespace JGW
{
    CCJGW_SSDownloadFramePacket::CCJGW_SSDownloadFramePacket(void)
    {
    }


    CCJGW_SSDownloadFramePacket::~CCJGW_SSDownloadFramePacket(void)
    {
    }

    size_t CCJGW_SSDownloadFramePacket::BuildCMDFrameStreamPacket(BYTE* pStreamCMDPacket,size_t nStreamPacketLen,BYTE** frame_buf)
    {
        char* pStreamPacket = (char*)calloc(nStreamPacketLen + sizeof(s_stream_frame) + sizeof(s_stream_frame_end),1);
        s_stream_frame* psStreamFrame = (s_stream_frame*)pStreamPacket;
        s_stream_frame_end* psStreamFrameEnd = (s_stream_frame_end*)(pStreamPacket + sizeof(s_stream_frame) + nStreamPacketLen);

        psStreamFrameEnd->flag = FLAG_FRAME;
        psStreamFrame->frame_header = FLAG_FRAME;
        memcpy(pStreamPacket + sizeof(s_stream_frame),pStreamCMDPacket,nStreamPacketLen);

        unsigned short crc;
        crc = m_crcCCITT.DoCrc((unsigned char*)pStreamPacket + sizeof(s_stream_frame),nStreamPacketLen);
        psStreamFrameEnd->fcs[0] = crc &0x00ff;
        psStreamFrameEnd->fcs[1] = (crc >> 8)&0x00ff;
        *frame_buf = (BYTE*)pStreamPacket;
#if 0
        wchar_t szTemp[4096] = {0};
        wchar_t szBuffer[10] = {0};
        for(size_t i = 0; i < nStreamPacketLen + sizeof(s_stream_frame) + sizeof(s_stream_frame_end); i++)
        {
            if(i%16 == 0) _tcscat_s(szTemp,4096,L"\n");
            _stprintf_s(szBuffer,10, L" R%02X", (UINT8)pStreamPacket[i]);
            _tcscat_s(szTemp,4096,szBuffer);
        }
        _tcscat_s(szTemp,4096,L"\n");
        OutputDebugString(szTemp);
#endif


        return nStreamPacketLen + sizeof(s_stream_frame) + sizeof(s_stream_frame_end);
    }

    size_t CCJGW_SSDownloadFramePacket::BuildDataStreamPacket(BYTE* pStreamCMDPacket,size_t nStreamPacketLen,BYTE** frame_buf)
    {
        size_t dwlen = 0;
        BYTE fcs[2] = {0};
        unsigned short crc = m_crcCCITT.DoCrc((BYTE*)pStreamCMDPacket,nStreamPacketLen);
        fcs[0] = crc &0x00ff;
        fcs[1] = (crc >> 8)&0x00ff;
        BYTE* max_frame = (BYTE*)calloc(PACKET_SIZE,1);
        *frame_buf = max_frame;
        *max_frame++ = FLAG_FRAME;
        dwlen++;

        for (size_t i = 0;i < nStreamPacketLen;i ++)
        {
            if (0x7D == pStreamCMDPacket[i])
            {
                *max_frame++ = 0x7D;
                dwlen++;
                *max_frame++ = 0x5D;
                dwlen++;

            }
            else if (0x7E == pStreamCMDPacket[i])
            {
                *max_frame++ = 0x7D;
                dwlen++;
                *max_frame++ = 0x5E;
                dwlen++;
            }
            else
            {
                *max_frame++ = pStreamCMDPacket[i];
                dwlen++;
            }
        }

        for (size_t i = 0;i < 2;i ++)
        {
            if (0x7D == fcs[i])
            {
                *max_frame++ = 0x7D;
                dwlen++;
                *max_frame++ = 0x5D;
                dwlen++;

            }
            else if (0x7E == fcs[i])
            {
                *max_frame++ = 0x7D;
                dwlen++;
                *max_frame++ = 0x5E;
                dwlen++;
            }
            else
            {
                *max_frame++ = fcs[i];
                dwlen++;
            }
        }
        *max_frame++ = FLAG_FRAME;
        dwlen++;
#if 1
        wchar_t szTemp[4096] = {0};
        wchar_t szBuffer[10] = {0};
        for(size_t i = 0; i < dwlen; i++)
        {
            if(i%16 == 0) _tcscat_s(szTemp,4096,L"\n");
            _stprintf_s(szBuffer,10, L" R%02X", (UINT8)((char*)(*frame_buf))[i]);
            _tcscat_s(szTemp,4096,szBuffer);
        }
        _tcscat_s(szTemp,4096,L"\n");
        OutputDebugString(szTemp);
#endif

        return dwlen;
#if 0
        //! packet len = frame len + data len + frame end len;
        size_t stream_packet_len = nStreamPacketLen + sizeof(s_stream_frame) + sizeof(s_stream_frame_end);
        char* pStreamPacket = (char*)calloc(stream_packet_len,1);
        s_stream_frame* psStreamFrame = (s_stream_frame*)pStreamPacket;
        s_stream_frame_end* psStreamFrameEnd = (s_stream_frame_end*)(pStreamPacket + sizeof(s_stream_frame) + nStreamPacketLen);

        psStreamFrameEnd->flag = FLAG_FRAME;
        psStreamFrame->frame_header = FLAG_FRAME;
        memcpy(pStreamPacket + sizeof(s_stream_frame),pStreamCMDPacket,nStreamPacketLen);

        unsigned short crc;
        crc = m_crcCCITT.DoCrc((unsigned char*)pStreamPacket + sizeof(s_stream_frame),nStreamPacketLen);
        psStreamFrameEnd->fcs[0] = crc &0x00ff;
        psStreamFrameEnd->fcs[1] = (crc >> 8)&0x00ff;
        char* max_frame = (char*)calloc(PACKET_SIZE,1);
        *frame_buf = max_frame;

        *max_frame++ = FLAG_FRAME;
        for (size_t i = 1;i < stream_packet_len - 1;i ++)
        {
            if (0x7D == pStreamPacket[i])
            {
                *max_frame++ = 0x7D;
                *max_frame++ = 0x5D;
            }
            else if (0x7E == pStreamPacket[i])
            {
                *max_frame++ = 0x7D;
                *max_frame++ = 0x5E;
            }
            else
            {
                *max_frame++ = pStreamPacket[i];
            }
        }
        free(pStreamPacket);
        *max_frame++ = FLAG_FRAME;
        size_t dwlen = max_frame - (char*)(*frame_buf) ;

        wchar_t szTemp[4096] = {0};
        wchar_t szBuffer[10] = {0};
        for(size_t i = 0; i < dwlen; i++)
        {
            if(i%16 == 0) _tcscat_s(szTemp,4096,L"\n");
            _stprintf_s(szBuffer,10, L" R%02X", (UINT8)((char*)(*frame_buf))[i]);
            _tcscat_s(szTemp,4096,szBuffer);
        }
        _tcscat_s(szTemp,4096,L"\n");
        OutputDebugString(szTemp);
#endif

        return dwlen;
    }

    size_t CCJGW_SSDownloadFramePacket::BuildDataStreamPacketToBuf(BYTE* pStreamCMDPacket,size_t nStreamPacketLen,/*const*/ char* frame_buf)
    {
        size_t dwlen = 0;
        BYTE fcs[2] = {0};
        unsigned short crc = m_crcCCITT.DoCrc((BYTE*)pStreamCMDPacket,nStreamPacketLen);
        fcs[0] = crc &0x00ff;
        fcs[1] = (crc >> 8)&0x00ff;
        *frame_buf++ = FLAG_FRAME;
        dwlen++;

        for (size_t i = 0;i < nStreamPacketLen;i ++)
        {
            if (0x7D == pStreamCMDPacket[i])
            {
                *frame_buf++ = 0x7D;
                dwlen++;
                *frame_buf++ = 0x5D;
                dwlen++;

            }
            else if (0x7E == pStreamCMDPacket[i])
            {
                *frame_buf++ = 0x7D;
                dwlen++;
                *frame_buf++ = 0x5E;
                dwlen++;
            }
            else
            {
                *frame_buf++ = pStreamCMDPacket[i];
                dwlen++;
            }
        }

        for (size_t i = 0;i < 2;i ++)
        {
            if (0x7D == fcs[i])
            {
                *frame_buf++ = 0x7D;
                dwlen++;
                *frame_buf++ = 0x5D;
                dwlen++;

            }
            else if (0x7E == fcs[i])
            {
                *frame_buf++ = 0x7D;
                dwlen++;
                *frame_buf++ = 0x5E;
                dwlen++;
            }
            else
            {
                *frame_buf++ = fcs[i];
                dwlen++;
            }
        }
        *frame_buf++ = FLAG_FRAME;
        dwlen++;
        return dwlen;

#if 0
        //! packet len = frame len + data len + frame end len;
        size_t stream_packet_len = nStreamPacketLen + sizeof(s_stream_frame) + sizeof(s_stream_frame_end);
        size_t packet_len = 0;
        char* pStreamPacket = (char*)calloc(stream_packet_len,1);
        s_stream_frame* psStreamFrame = (s_stream_frame*)pStreamPacket;
        s_stream_frame_end* psStreamFrameEnd = (s_stream_frame_end*)(pStreamPacket + sizeof(s_stream_frame) + nStreamPacketLen);

        psStreamFrameEnd->flag = FLAG_FRAME;
        psStreamFrame->frame_header = FLAG_FRAME;
        memcpy(pStreamPacket + sizeof(s_stream_frame),pStreamCMDPacket,nStreamPacketLen);

        unsigned short crc;
        crc = m_crcCCITT.DoCrc((unsigned char*)pStreamPacket + sizeof(s_stream_frame),nStreamPacketLen);
        psStreamFrameEnd->fcs[0] = crc &0x00ff;
        psStreamFrameEnd->fcs[1] = (crc >> 8)&0x00ff;

        *frame_buf++ = FLAG_FRAME;
        packet_len++;
        for (size_t i = 1;i < stream_packet_len - 1;i ++)
        {
            if (0x7D == pStreamPacket[i])
            {
                *frame_buf++ = 0x7D;
                packet_len++;
                *frame_buf++ = 0x5D;
                packet_len++;

            }
            else if (0x7E == pStreamPacket[i])
            {
                *frame_buf++ = 0x7D;
                packet_len++;
                *frame_buf++ = 0x5E;
                packet_len++;
            }
            else
            {
                *frame_buf++ = pStreamPacket[i];
                packet_len++;
            }
        }
        free(pStreamPacket);
        *frame_buf++ = FLAG_FRAME;
        packet_len++;

        wchar_t szTemp[4096] = {0};
        wchar_t szBuffer[10] = {0};
        for(size_t i = 0; i < dwlen; i++)
        {
            if(i%16 == 0) _tcscat_s(szTemp,4096,L"\n");
            _stprintf_s(szBuffer,10, L" R%02X", (UINT8)((char*)(*frame_buf))[i]);
            _tcscat_s(szTemp,4096,szBuffer);
        }
        _tcscat_s(szTemp,4096,L"\n");
        OutputDebugString(szTemp);
#endif

        return dwlen;
    }

}

