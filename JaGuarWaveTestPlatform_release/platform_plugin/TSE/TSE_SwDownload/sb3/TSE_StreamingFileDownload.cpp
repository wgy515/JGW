#include "StdAfx.h"
#include "TSE_StreamingFileDownload.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>

namespace JGW
{

    CTSE_StreamingFileDownload::CTSE_StreamingFileDownload(void) : mbInit(false),mnMaximumDataSize(1024),mbReset(true)
    {
        mRxBuf = (char*)calloc(STREAM_MAX_BUF_SIZE, 1);
        mTxBuf = (char*)calloc(STREAM_MAX_BUF_SIZE, 1);
        mTempFileBuf = (char*)calloc(STREAM_MAX_BUF_SIZE,1);
        mpsStreamingErrorCode = (ps_streaming_error_packet)(mRxBuf + 1);
        mpsStreamCommand = (ps_stream_command)(mRxBuf + 1);
        mpsStreamingLoginfo = (ps_streaming_log_info)(mRxBuf + 1);
        mpStreamHelloResponsePacket = (ps_stream_hello_response_packet)(mRxBuf + 1);
    }


    CTSE_StreamingFileDownload::~CTSE_StreamingFileDownload(void)
    {
        if (mRxBuf)
        {
            free(mRxBuf);
        }
        if (mTxBuf)
        {
            free(mTxBuf);
        }
        if (mTempFileBuf)
        {
            free(mTempFileBuf);
        }
    }

    const wchar_t* CTSE_StreamingFileDownload::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_StreamingFileDownload\",\
                \"TestName\": \"streaming file download\",\
                \"SaharaSerialClassEnv\": \"get sahara serial class environment\",\
                \"DownloadFolderPathEnv\": \"Download Folder Path environment\",\
                \"ContentXMLFilePathCmd\": \"content xml file path cmd\",\
                \"ResetAfterDownload\": \"reset after download def:1\"\
                }";
    }

    bool CTSE_StreamingFileDownload::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"SaharaSerialClassEnv"))
        {
            mstrSaharaSerialClassEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"DownloadFolderPathEnv"))
        {
            mstrDownloadFolderPathEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ContentXMLFilePathCmd"))
        {
            mstrContentXMLFilePathCmd = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ResetAfterDownload"))
        {
            mbReset = 1 == _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_StreamingFileDownload::InitDwonloadConfig()
    {
        sDownloadConfigInfo.mbReset = mbReset;
        std::wstring strContentXMLFilePath(mstrContentXMLFilePathCmd);
        GetEnvironmentCommandString(strContentXMLFilePath);
        sDownloadConfigInfo.mstrContentXMLFilePath = JGW_W2A(strContentXMLFilePath);
        sDownloadConfigInfo.mstrDownloadPath = GetGlobalEnvironment()->GetAnsiString(mstrDownloadFolderPathEnv);
        return downloadConfig.LoadSSDwonloadConfig(&sDownloadConfigInfo);
    }

    bool CTSE_StreamingFileDownload::WriteSerialPort(char* buffer,size_t bytes_to_send)
    {
        if (mpCSerialPort)
        {
            size_t bytes_sent = 0, temp_bytes_sent = 0;
            while (bytes_sent < bytes_to_send)
            {
                if(!mpCSerialPort->WriteBufferToDownloadPort(buffer + bytes_sent, min(bytes_to_send - bytes_sent,STREAM_MAX_BUF_SIZE),  temp_bytes_sent)) 
                {
                    LogE(L"Error occurred while writing to COM port");
                    return false;
                }
                else {
                    bytes_sent += temp_bytes_sent;
                }

            }
            return true;
        }
        return false;
    }

    bool CTSE_StreamingFileDownload::ReadSerialPort(char* buffer, size_t bytes_to_read, size_t& bytes_read)
    {
        if (mpCSerialPort)
        {
            buffer[0] = 0x00;
            if (mpCSerialPort->ReadStreamingDownloadPort(buffer,min(bytes_to_read,STREAM_MAX_BUF_SIZE),bytes_read))
            {
                return true;
            }
        }
        return false;
    }

    bool CTSE_StreamingFileDownload::TSE_Run()
    {
        if (!mbInit)
        {
            if (!InitDwonloadConfig())
            {
                LogE(L"Load Download Config Fail\n");
                return false;
            }
            mbInit = true;
        }

        mpCSerialPort = (CCJGW_QcommDownloadSerialPort*)GetGlobalEnvironment()->GetInt(mstrSaharaSerialClassEnv);
        if (NULL == mpCSerialPort)
        {
            LogE_F(L"Could not connect to the Resource Context");
            return false;
        }

        size_t bytes_read = 0;
        for (std::vector<s_stream_protocol_command>::iterator it = downloadConfig.GetDownloadAllCommand().mvStreamProtCommand.begin();
            it != downloadConfig.GetDownloadAllCommand().mvStreamProtCommand.end();
            ++ it)
        {
            ps_stream_frame_header psStreamFrameHeader = (ps_stream_frame_header)it->mcommand_buf;
            meStreamingDownloadCmdEnum = (Streaming_Download_Cmd_Enum)psStreamFrameHeader->command;
            if (!WriteSerialPort((char*)it->mcommand_buf,it->mcommand_len))
            {
                LogE(L"Send Command Fail!!!");
                return false;
            }
            bytes_read = 0;
            if (!ReadSerialPort(mRxBuf,STREAM_MAX_BUF_SIZE,bytes_read) || 0 == bytes_read)
            {
                LogE(L"Recv Command Fail!!!");
                return false;
            }
            mRxBuf[bytes_read - 1] = '\0';
            if (!ResponseReadPacket(&it[0],bytes_read))
            {
                return false;
            }
        }
        return true;
    }

    bool CTSE_StreamingFileDownload::ResponseReadPacket(s_stream_protocol_command* psStreamProtocolCommand,size_t rx_len)
    {
        switch ((Streaming_Download_Cmd_Enum)mpsStreamCommand->command)
        {
        case HELLO_RSP_CMD:
            if (HELLO_CMD != meStreamingDownloadCmdEnum)
            { 
                LogE(L"send command and revc command mismatches");
                return false;
            }
            mnMaximumDataSize = mpStreamHelloResponsePacket->maximum;
            if (mnMaximumDataSize <= 0) mnMaximumDataSize = 1024;
            if (mnMaximumDataSize >= STREAM_MAX_BUF_SIZE - 1024) mnMaximumDataSize = STREAM_MAX_BUF_SIZE - 1024;
            //! mMaximum_write_len = 1536/*1024 * 2*/;
            LogI_F(L"Maximum data size acceptable for a write command(%d)",mnMaximumDataSize);
            break;
        case DWNLDR_SECURITY_MODE_RSP_CMD:
            if (DWNLDR_SECURITY_MODE_CMD != meStreamingDownloadCmdEnum)
            { 
                LogE(L"send command and revc command mismatches"); 
                return false; 
            }
            LogI_F(L"%s \n",JGW_A2W(szStreamingDownloadCmdEnumString[mpsStreamCommand->command]).c_str());
            break;
        case DWNLDR_PRTN_TBL_RSP_CMD:
            if (DWNLDR_PRTN_TBL_CMD != meStreamingDownloadCmdEnum)
            { 
                LogE(L"send command and revc command mismatches"); 
                return false;
            }
            LogI_F(L"%s \n",JGW_A2W(szStreamingDownloadCmdEnumString[mpsStreamCommand->command]).c_str());
            break;
        case ERROR_CMD:
            check_streaming_error_code(mpsStreamingErrorCode->error_code);
            return false;
        case LOG_CMD:  
            for (size_t i = 2;i < rx_len;i++) if (0x0A == mpsStreamingLoginfo->log[i]) mpsStreamingLoginfo->log[i] = '\0';
            LogI_F(L"error log: %s\n",JGW_A2W((char*)mpsStreamingLoginfo->log).c_str());
            return false;
        case DWNLDR_MULTI_IMG_OPEN_RSP_CMD:
            if (DWNLDR_MULTI_IMG_OPEN_CMD != meStreamingDownloadCmdEnum)
            { 
                LogE(L"send command and revc command mismatches"); 
                return false;
            }
            return StreamingMultiImageDownload(psStreamProtocolCommand);
        case DWNLDR_CLOSE_RSP_CMD:
            if (DWNLDR_CLOSE_CMD != meStreamingDownloadCmdEnum)
            { 
                LogE(L"send command and revc command mismatches"); 
                return false; 
            }
            LogI_F(L"%s \n",JGW_A2W(szStreamingDownloadCmdEnumString[mpsStreamCommand->command]).c_str());
            //! send_download_error_msg("Error response \n");
            break;
        case STRM_RESET_RSP_CMD:
            if (STRM_RESET_CMD != meStreamingDownloadCmdEnum)
            { 
                LogE(L"send command and revc command mismatches"); 
                return false; 
            }
            LogI_F(L"%s \n",JGW_A2W(szStreamingDownloadCmdEnumString[mpsStreamCommand->command]).c_str());
            break;
        default:
            LogE_F(L"unkonw response command: %d",mpsStreamCommand->command);
            //!print_error_command(rx_len);
            //! send_download_error_msg("");
            return false;
        }
        return true;
    }

    bool CTSE_StreamingFileDownload::StreamingMultiImageDownload(s_stream_protocol_command* psStreamProtocolCommand)
    {
        std::shared_ptr<FILE> fp(fopen(psStreamProtocolCommand->mstrPartitionMBNPath.c_str(), "rb"),JGW_CloseFilePtr_C);
        LogI_F(L"start send program:%s",JGW_A2W(psStreamProtocolCommand->mstrPartitionMBNPath).c_str());
        if (!fp) 
        {
            LogE_F(L"open fail(%s)",JGW_A2W(psStreamProtocolCommand->mstrPartitionMBNPath).c_str());
            return false;
        }
        //! 首次读取文件大小，判断文件是否为空文件
        size_t file_size = JGW_GetFilePtrFileSize_C(fp.get());
        if (file_size <= 0)
        {
            LogE_F(L"read file fail(%s)",JGW_A2W(psStreamProtocolCommand->mstrPartitionMBNPath).c_str());
            return false;
        }

        size_t nFilePos = 0,byte_read = 0;
        while (file_size > 0)
        {
            size_t write_len = file_size >= mnMaximumDataSize ? mnMaximumDataSize : file_size;
            file_size -= write_len;
            if (write_len != fread (mTempFileBuf, sizeof(char), write_len, fp.get()))
            {
                LogE_F(L"read file exception(%s)",JGW_A2W(psStreamProtocolCommand->mstrPartitionMBNPath).c_str());
                return false;
            }
            size_t pakcet_len = build_unframe_streaming_multi_image_packet(write_len,nFilePos);
            nFilePos += write_len;
            if (!WriteSerialPort(mTxBuf,pakcet_len))
            {
                LogE(L"write serial port fail");
                return false;
            }
            if (!ReadSerialPort(mRxBuf,STREAM_MAX_BUF_SIZE,byte_read))
            {
                LogE(L"read serial port fail");
                return false;
            }
            mRxBuf[byte_read - 1] = '\0';
            switch ((Streaming_Download_Cmd_Enum)mpsStreamCommand->command)
            {
            case ERROR_CMD:
                check_streaming_error_code(mpsStreamingErrorCode->error_code);
                return false;
            case LOG_CMD:  
                for (size_t i = 2;i < byte_read;i++) if (0x0A == mpsStreamingLoginfo->log[i]) mpsStreamingLoginfo->log[i] = '\0';
                LogI_F(L"error log: %s",JGW_A2W((char*)mpsStreamingLoginfo->log).c_str());
                return false;
            case STRM_WRITE_RSP_CMD:
                if (STRM_WRITE_CMD != meStreamingDownloadCmdEnum)
                { 
                    LogE(L"send command and revc command mismatches \n"); 
                    return false; 
                }
                break;
            case UNFRAMED_STREAM_WRITE_RSP_CMD:
                if (UNFRAMED_STREAM_WRITE_CMD != meStreamingDownloadCmdEnum)
                { 
                    LogE(L"send command and revc command mismatches \n"); 
                    return false; 
                }
                break;
            default:
                LogE_F(L"unkonw response command: %d",mpsStreamCommand->command);
                return false;
            }
        }

        return true;
    }

    size_t CTSE_StreamingFileDownload::build_unframe_streaming_multi_image_packet(size_t read_len,size_t file_pos)
    {
        ps_streaming_unframe_stream_write_packet psStreamWritePacket = (ps_streaming_unframe_stream_write_packet)mTxBuf;
        psStreamWritePacket->command = UNFRAMED_STREAM_WRITE_CMD;
        meStreamingDownloadCmdEnum = UNFRAMED_STREAM_WRITE_CMD;
        psStreamWritePacket->padding_bits[0] = 0;
        psStreamWritePacket->padding_bits[1] = 0;
        psStreamWritePacket->padding_bits[2] = 0;
        psStreamWritePacket->address = file_pos;
        psStreamWritePacket->length = read_len;
        memcpy(psStreamWritePacket->data,mTempFileBuf,read_len);
        return read_len + sizeof(s_streaming_unframe_stream_write_packet) - 1;
    }

    void CTSE_StreamingFileDownload::check_streaming_error_code(UINT32 error_code)
    {
        switch (error_code)
        {
        case E_STCMD_IIEGAL:
            LogE(L"Illegal reason (do not use)\n");
            break;
        case E_STCMD_RESERVED:
            LogE(L"Reserved\n");
            break;
        case E_STCMD_INV_DEST_ADDRESS:
            LogE(L"Invalid destination address\n");
            break;
        case E_STCMD_INV_LEN:
            LogE(L"Invalid length\n");
            break;
        case E_STCMD_UNEXPECTED:
            LogE(L"Unexpected end of packet\n");
            break;
        case E_STCMD_INV_CMD:
            LogE(L"Invalid command\n");
            break;
        case E_STCMD_RESERVERD_SIX:
            LogE(L"Reserved\n");
            break;
        case E_STCMD_OPERATION:
            LogE(L"Operation failed\n");
            break;
        case E_STCMD_WRONG_FLAS:
            LogE(L"Wrong Flash intelligent ID\n");
            break;
        case E_STCMD_BAD_PROGRAM:
            LogE(L"Bad programming voltage\n");
            break;
        case E_STCMD_WRITE_VERIFY:
            LogE(L"Write-verify failed\n");
            break;
        case E_STCMD_RESERVED_B:
            LogE(L"Reserved\n");
            break;
        case E_STCMD_INCORRECT_SECURITY:
            LogE(L"Incorrect security code\n");
            break;
        case E_STCMD_CANNOT_POWER_DOWN:
            LogE(L"Cannot power down phone\n");
            break;
        case E_STCMD_NAND_FLASH_RPOGRAM_NOT_SUPPORT:
            LogE(L"NAND Flash programming not supported\n");
            break;
        case E_STCMD_COMMAND_OUT_OF_SEQ:
            LogE(L"Command out of sequence\n");
            break;
        case E_STCMD_CLOSE_DID_NOT_SUCCEED:
            LogE(L"Close did not succeed\n");
            break;
        case E_STCMD_INCOMPATIBLE_FEATURE_BITS:
            LogE(L"Incompatible feature bits\n");
            break;
        case E_STCMD_OUT_OF_SPACE:
            LogE(L"Out of space\n");
            break;
        case E_STCMD_INVALID_SEC_MODE:
            LogE(L"Invalid Security mode\n");
            break;
        case E_STCMD_MULTI_IMAGE_NAND_NOT_SUPPORT:
            LogE(L"Multi-image NAND not supported\n");
            break;
        case E_STCMD_POWER_OFF:
            LogE(L"Power off command not supported\n");
            break;
        default:
            LogE_F(L"error_code:%d(%s)\n",error_code,JGW_A2W((char*)mpsStreamingErrorCode->text_data).c_str());
            break;
        }
    }
}

