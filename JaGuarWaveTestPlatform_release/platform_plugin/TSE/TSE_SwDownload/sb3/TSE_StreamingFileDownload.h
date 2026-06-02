#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "CJGW_QcommDownloadSerialPort.h"
#include "CJGW_SSDownloadConfig.h"
#include "sw_sb3_download_define.h"

namespace JGW
{
    class CTSE_StreamingFileDownload : public CTSE_TestBase
    {
    public:
        CTSE_StreamingFileDownload(void);
        ~CTSE_StreamingFileDownload(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool InitDwonloadConfig();
        bool TSE_Run();
    private:
        //!
        bool ResponseReadPacket(s_stream_protocol_command* psStreamProtocolCommand,size_t readLen);
        //! 
        bool ReadSerialPort(char* buffer, size_t bytes_to_read, size_t& bytes_read);
        //! 
        bool WriteSerialPort(char* buffer,size_t bytes_to_send);
        //£¡
        void check_streaming_error_code(UINT32 error_code);
        //!
        bool StreamingMultiImageDownload(s_stream_protocol_command* psStreamProtocolCommand);
        //!
        size_t build_unframe_streaming_multi_image_packet(size_t read_len,size_t file_pos);
    private:
        bool mbReset;
        bool mbInit;
        Streaming_Download_Cmd_Enum meStreamingDownloadCmdEnum;
        char* mRxBuf;
        char* mTxBuf;
        char* mTempFileBuf;
        //! Maximum data size stream write data maximum
        size_t mnMaximumDataSize;
        ps_streaming_error_packet mpsStreamingErrorCode;
        ps_streaming_log_info mpsStreamingLoginfo;
        ps_stream_command mpsStreamCommand;
        ps_stream_hello_response_packet mpStreamHelloResponsePacket;

        CCJGW_QcommDownloadSerialPort* mpCSerialPort;
        std::wstring mstrDownloadFolderPathEnv;
        std::wstring mstrContentXMLFilePathCmd;
        std::wstring mstrSaharaSerialClassEnv;
        CCJGW_SSDownloadConfig downloadConfig;
        s_download_config_info sDownloadConfigInfo;
    };
}

