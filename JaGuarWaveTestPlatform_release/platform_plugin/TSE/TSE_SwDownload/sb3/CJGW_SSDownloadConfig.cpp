#include "StdAfx.h"
#include "CJGW_SSDownloadConfig.h"
#include "contents_xml/ParseContentsXMLConfig.h"
#include <JGW_FoundationFunc/JGW_Exception.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#include "contents_xml/Packet.h"

namespace JGW
{
    CCJGW_SSDownloadConfig::CCJGW_SSDownloadConfig(void):mpSDownloadConfigInfo(NULL)
    {

    }

    CCJGW_SSDownloadConfig::~CCJGW_SSDownloadConfig(void)
    {
        for (size_t i = 0;i < msDownloadAllCommand.mvStreamProtCommand.size();i ++)
        {
            if (msDownloadAllCommand.mvStreamProtCommand.at(i).mcommand_buf)
            {
                delete msDownloadAllCommand.mvStreamProtCommand.at(i).mcommand_buf;
                msDownloadAllCommand.mvStreamProtCommand.at(i).mcommand_buf = NULL;
            }
        }

    }

    bool CCJGW_SSDownloadConfig::LoadSSDwonloadConfig(ps_download_config_info pSDownloadConfigInfo)
    {
        mpSDownloadConfigInfo = pSDownloadConfigInfo;
        mvPartitionInfo.clear();
        for (size_t i = 0;i < msDownloadAllCommand.mvStreamProtCommand.size();i ++)
        {
            if (msDownloadAllCommand.mvStreamProtCommand.at(i).mcommand_buf)
            {
                delete msDownloadAllCommand.mvStreamProtCommand.at(i).mcommand_buf;
                msDownloadAllCommand.mvStreamProtCommand.at(i).mcommand_buf = NULL;
            }
        }

        if (!mpSDownloadConfigInfo)
        {
            return false;
        }

        return LoadContentXMLFile();
    }

    bool CCJGW_SSDownloadConfig::LoadContentXMLFile()
    {
        CParseContentsXMLConfig parese_contents;
        try
        {
            if (!parese_contents.ParseContentsXml(mpSDownloadConfigInfo->mstrContentXMLFilePath.c_str()) || 
                !parese_contents.GetPartitionInfo(mvPartitionInfo))
            {
                return false;
            }
        }
        catch (CJGW_Exception&)
        {
            return false;
        }

        if (!BuildHelloCmd() || 
            !BuildStreamSecurityModeCmd() ||
            !BuildStreamPartitionCmd() ||
            !BuildStreamWriteCmd() || 
            !BuildResetPacketCmd())
        {
            return false;
        }

        return true;
    }

    bool CCJGW_SSDownloadConfig::BuildHelloCmd()
    {
#if 1
        {
            s_stream_hello_packet sStreamHelloPacket = {0};
            sStreamHelloPacket.command = HELLO_CMD;

            memcpy(sStreamHelloPacket.magic_number,"QCOM fast download protocol host",32);
            sStreamHelloPacket.version_number = 0x04;
            sStreamHelloPacket.compatible_version = 0x02;
            sStreamHelloPacket.feature_bits = 0x01;

            s_stream_protocol_command sStreamProtocolCommand = {0,0,"",""};

            sStreamProtocolCommand.mcommand_len = mFramePacket.BuildCMDFrameStreamPacket((BYTE*)&sStreamHelloPacket,sizeof(s_stream_hello_packet),&sStreamProtocolCommand.mcommand_buf);

            msDownloadAllCommand.mvStreamProtCommand.push_back(sStreamProtocolCommand);

            return sStreamProtocolCommand.mcommand_buf && sStreamProtocolCommand.mcommand_len;
        }
#else
        {
            s_stream_hello_packet sStreamHelloPacket = {0};
            sStreamHelloPacket.command = HELLO_CMD;

            memcpy(sStreamHelloPacket.magic_number,"QCOM fast download protocol host",32);
            sStreamHelloPacket.version_number = 0x02;
            sStreamHelloPacket.compatible_version = 0x02;
            sStreamHelloPacket.feature_bits = 0x01;

            s_stream_protocol_command sStreamProtocolCommand = {0,0,"",""};

            sStreamProtocolCommand.mcommand_len = mFramePacket.BuildCMDFrameStreamPacket(&sStreamHelloPacket,sizeof(s_stream_hello_packet),&sStreamProtocolCommand.mcommand_buf);

            msDownloadAllCommand.mvStreamProtCommand.push_back(sStreamProtocolCommand);
        }




        {
            s_stream_hello_packet sStreamHelloPacket = {0};
            sStreamHelloPacket.command = HELLO_CMD;

            memcpy(sStreamHelloPacket.magic_number,"QCOM fast download protocol host",32);
            sStreamHelloPacket.version_number = 0x03;
            sStreamHelloPacket.compatible_version = 0x03;
            sStreamHelloPacket.feature_bits = 0x09;

            s_stream_protocol_command sStreamProtocolCommand = {0,0,"",""};

            sStreamProtocolCommand.mcommand_len = mFramePacket.BuildCMDFrameStreamPacket(&sStreamHelloPacket,sizeof(s_stream_hello_packet),&sStreamProtocolCommand.mcommand_buf);

            msDownloadAllCommand.mvStreamProtCommand.push_back(sStreamProtocolCommand);
        }



#endif

#if 0
        wchar_t szTemp[4096] = {0};
        wchar_t szBuffer[10] = {0};
        for(size_t i = 0; i < sStreamProtocolCommand.mcommand_len; i++)
        {
            if(i%16 == 0) _tcscat_s(szTemp,4096,L"\n");
            _stprintf_s(szBuffer,10, L" R%02X", (UINT8)((char*)sStreamProtocolCommand.mcommand_buf)[i]);
            _tcscat_s(szTemp,4096,szBuffer);
        }
        _tcscat_s(szTemp,4096,L"\n");
        OutputDebugString(szTemp);

        CPacket packet;
        BYTE szBuf[1024] = {0};


        packet.BuildCmdPacket(HELLO_CMD,sStreamHelloPacket.magic_number,sizeof(s_stream_hello_packet)-1);
        packet.GetPacket(szBuf);
        DWORD dwLen = packet.GetPacketCountLength();

        memset(szTemp,0x00,4096);
        for(size_t i = 0; i < dwLen; i++)
        {
            if(i%16 == 0) _tcscat_s(szTemp,4096,L"\n");
            _stprintf_s(szBuffer,10, L" R%02X", (UINT8)((char*)szBuf)[i]);
            _tcscat_s(szTemp,4096,szBuffer);
        }
        _tcscat_s(szTemp,4096,L"\n");
        OutputDebugString(szTemp);
#endif

        return true/*sStreamProtocolCommand.mcommand_buf && sStreamProtocolCommand.mcommand_len*/;
    }

    bool CCJGW_SSDownloadConfig::BuildStreamSecurityModeCmd()
    {
        s_stream_security_mode_packet sStreamSecModePacket = {DWNLDR_SECURITY_MODE_CMD,0x01};
        s_stream_protocol_command sStreamProtocolCommand = {0,0,"",""};

        sStreamProtocolCommand.mcommand_len = mFramePacket.BuildCMDFrameStreamPacket((BYTE*)&sStreamSecModePacket,sizeof(s_stream_security_mode_packet),&sStreamProtocolCommand.mcommand_buf);
        msDownloadAllCommand.mvStreamProtCommand.push_back(sStreamProtocolCommand);

        return sStreamProtocolCommand.mcommand_buf && sStreamProtocolCommand.mcommand_len;
    }

    bool CCJGW_SSDownloadConfig::BuildClosePacketCmd()
    {
        s_stream_close_packet sStreamingClosePacket = {DWNLDR_CLOSE_CMD};
        s_stream_protocol_command sStreamProtocolCommand = {0,0,"",""};

        sStreamProtocolCommand.mcommand_len = mFramePacket.BuildCMDFrameStreamPacket((BYTE*)&sStreamingClosePacket,sizeof(s_stream_close_packet),&sStreamProtocolCommand.mcommand_buf);
        msDownloadAllCommand.mvStreamProtCommand.push_back(sStreamProtocolCommand);

        return sStreamProtocolCommand.mcommand_buf && sStreamProtocolCommand.mcommand_len;
    }

    bool CCJGW_SSDownloadConfig::BuildResetPacketCmd()
    {
        if (!mpSDownloadConfigInfo->mbReset) return true;
        s_stream_reset_packet sStreamingClosePacket = {STRM_RESET_CMD};
        s_stream_protocol_command sStreamProtocolCommand = {0,0,"",""};

        sStreamProtocolCommand.mcommand_len = mFramePacket.BuildCMDFrameStreamPacket((BYTE*)&sStreamingClosePacket,sizeof(s_stream_reset_packet),&sStreamProtocolCommand.mcommand_buf);
        msDownloadAllCommand.mvStreamProtCommand.push_back(sStreamProtocolCommand);

        return sStreamProtocolCommand.mcommand_buf && sStreamProtocolCommand.mcommand_len;
    }


    bool CCJGW_SSDownloadConfig::BuildStreamPartitionCmd()
    {
        std::string strPartitionFilePath;

        for ( std::vector<S_PARTITION_INFO>::iterator it = mvPartitionInfo.begin();
            it != mvPartitionInfo.end();
            ++it )
        {
            if ( 0 == JGW_StrComparenoCaseStr(it->strPartitionName.c_str(),"0:MIBIB")/*.compare("0:MIBIB")*/ )
            {
                strPartitionFilePath = it->strPartitionImageName;
                break;
            }
        }

        size_t nPartitionFileSize = JGW_GetFileSize_C(strPartitionFilePath.c_str());
        if (nPartitionFileSize <= 0) return false;

        s_stream_partition_table_packet* psStreamPartitionTablePacket = (s_stream_partition_table_packet*)calloc(sizeof(s_stream_partition_table_packet) + nPartitionFileSize - 1,1);
        s_stream_protocol_command sStreamProtocolCommand = {0,0,"",""};

        if (!JGW_GetFileBufToPath_BufLen_C(strPartitionFilePath.c_str(),(char*)&psStreamPartitionTablePacket->partition_table[0],nPartitionFileSize))
        {
            free(psStreamPartitionTablePacket);
            return false;
        }

        psStreamPartitionTablePacket->command = DWNLDR_PRTN_TBL_CMD;
        psStreamPartitionTablePacket->override = 0x00;
        sStreamProtocolCommand.mcommand_len = mFramePacket.BuildDataStreamPacket((BYTE*)psStreamPartitionTablePacket,sizeof(s_stream_partition_table_packet) + nPartitionFileSize - 1,&sStreamProtocolCommand.mcommand_buf);
        msDownloadAllCommand.mvStreamProtCommand.push_back(sStreamProtocolCommand);


#if 0
        CPacket packet;

        s_stream_command cmdParam;
        cmdParam.command = 0x00;
        BYTE szFilebuffer[PACKET_SIZE] = {0};
        if (!JGW_GetFileBufToPath_BufLen_C(strPartitionFilePath.c_str(),(char*)szFilebuffer,nPartitionFileSize))
        {
            free(psStreamPartitionTablePacket);
            return false;
        }

        packet.BuildDataPacket(DWNLDR_PRTN_TBL_CMD, szFilebuffer, nPartitionFileSize, &cmdParam, sizeof(s_stream_command));

        BYTE *buffer = (BYTE*)calloc(1024,1);
        memset(buffer, 0, sizeof(buffer));
        packet.GetPacket(buffer);
        size_t dwLen = packet.GetPacketCountLength();


        wchar_t szTemp[4096] = {0};
        wchar_t szBuffer[10] = {0};
        for(size_t i = 0; i < dwLen; i++)
        {
            if(i%16 == 0) _tcscat_s(szTemp,4096,L"\n");
            _stprintf_s(szBuffer,10, L" R%02X", (UINT8)buffer[i]);
            _tcscat_s(szTemp,4096,szBuffer);
        }
        _tcscat_s(szTemp,4096,L"\n");
        OutputDebugString(szTemp);

        sStreamProtocolCommand.mcommand_buf = buffer;
        sStreamProtocolCommand.mcommand_len = dwLen;
#endif
        free(psStreamPartitionTablePacket);

        return sStreamProtocolCommand.mcommand_buf && sStreamProtocolCommand.mcommand_len;
    }
//#pragma pack(push, 1)
//
//    typedef struct  
//    {
//        BYTE command; //! 0x1b
//        BYTE type; //! 0x0E
//        BYTE byte[1];
//    }s_test;
//#pragma pack(pop)

    bool CCJGW_SSDownloadConfig::BuildStreamWriteCmd()
    {
        for ( std::vector<S_PARTITION_INFO>::iterator it = mvPartitionInfo.begin();
            it != mvPartitionInfo.end();
            ++it )
        {
            if ( 0 == JGW_StrComparenoCaseStr(it->strPartitionName.c_str(),"0:MIBIB")/*.compare("0:MIBIB")*/ )
            {
                continue;
            }
            size_t streaming_packet_len = sizeof(s_stream_multi_image_packet) + it->strPartitionName.length();
            ps_stream_multi_image_packet psStreamMultiImagePacket = (ps_stream_multi_image_packet)calloc(streaming_packet_len,1);
            memset(psStreamMultiImagePacket,0x00,streaming_packet_len);
            psStreamMultiImagePacket->command = DWNLDR_MULTI_IMG_OPEN_CMD;
            psStreamMultiImagePacket->type = 0x0E;
            strcpy_s((char*)psStreamMultiImagePacket->data,it->strPartitionName.length() + 1,it->strPartitionName.c_str());

            //streaming_packet_len = streaming_packet_len - 1;
            s_stream_protocol_command sStreamProtocolCommand = {0,0,it->strPartitionName,it->strPartitionImageName};
            sStreamProtocolCommand.mcommand_len = mFramePacket.BuildDataStreamPacket((BYTE*)psStreamMultiImagePacket,streaming_packet_len,&sStreamProtocolCommand.mcommand_buf);

            if (!sStreamProtocolCommand.mcommand_buf || !sStreamProtocolCommand.mcommand_len) return false;
            msDownloadAllCommand.mvStreamProtCommand.push_back(sStreamProtocolCommand);


            if (!BuildClosePacketCmd()) return false;
        }

        return true;
    }
}

