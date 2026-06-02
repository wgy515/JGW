#include "StdAfx.h"
#include "ss_download_asio_serial_port.h"

#include <JGW_FoundationFunc/JGW_FilePath.h>


namespace JGW
{
    int getpagesize(void) 
    {
        static DWORD page_size = 0;
        if (0 == page_size) 
        {
            SYSTEM_INFO system_info;
            GetSystemInfo (&system_info);
            page_size = system_info.dwPageSize;
        }
        return page_size;
    }

    bool ss_download_asio_serial_port::mbGetMaxPayloadSizeToTargetInBytes = false;

    ss_download_asio_serial_port::ss_download_asio_serial_port(boost::asio::io_service& ios,int nIndex):
    sp(ios),
        deadline_(ios),
        mbStreamingDownload(false),
        mMaximum_write_len(1024),
        meStreamingDownloadCmdEnum(HELLO_CMD)
    {
        msDownloadProgressInfo.mnIndex = (nIndex);
        msDownloadProgressInfo.mdw64SendCounts = 0;
        msDownloadProgressInfo.meDownloadStatus = E_IDLE_DOWNLOAD;
  
        mrx_buf = (char*)memalign (getpagesize (), MAX_STRING_SIZE);
        mtx_buf = (char*)memalign (getpagesize (), FIREHOSE_TX_BUFFER_SIZE);
        msFileTempBufInfo.mnFileTempBuf = (char*)memalign (getpagesize (), FIREHOSE_TX_BUFFER_SIZE);

        memset(mtx_buf,0x00,FIREHOSE_TX_BUFFER_SIZE);
        memset(mrx_buf,0x00,MAX_STRING_SIZE);
        memset(msFileTempBufInfo.mnFileTempBuf,0x00,FIREHOSE_TX_BUFFER_SIZE);

        sahara.sahara_hello = (sahara_packet_hello*)mrx_buf;
        sahara.sahara_hello_resp = (sahara_packet_hello_resp*)mtx_buf;
        sahara.sahara_end_image_tx = (sahara_packet_end_image_tx*)mrx_buf;
        sahara.sahara_read_data = (sahara_packet_read_data*)mrx_buf;
        sahara.sahara_done = (sahara_packet_done*)mtx_buf;
        sahara.sahara_done_resp = (sahara_packet_done_resp*)mrx_buf;
        sahara.sahara_switch_mode = (sahara_switch_mode_packet*)mtx_buf;
        sahara.sahara_reset = (sahara_packet_reset*)mtx_buf;

        streaming.mpsStreamingErrorCode = (ps_streaming_error_packet)((char*)mrx_buf + 1);
        streaming.mpStreamHelloResponsePacket = (ps_stream_hello_response_packet)((char*)mrx_buf + 1);
        streaming.mpsStreamCommand = (ps_stream_command)((char*)mrx_buf + 1);
        streaming.mpsStreamingLoginfo = (ps_streaming_log_info)((char*)mrx_buf + 1);

        set_port_option();
    }
    /*#pragma region */
    ss_download_asio_serial_port::~ss_download_asio_serial_port(void)
    {
        close_serial_port();
        ALIGNED_FREE(mrx_buf);
        ALIGNED_FREE(mtx_buf);
        ALIGNED_FREE(msFileTempBufInfo.mnFileTempBuf);
    }

    void ss_download_asio_serial_port::set_port_option(int nbaud_rate /* = 9600 */, int ncharacter_size /* = 8 */, boost::asio::serial_port::flow_control::type eflow_control /* = boost::asio::serial_port::flow_control::none */, boost::asio::serial_port::stop_bits::type estop_bits /* = boost::asio::serial_port::stop_bits::one */)
    {
        meflow_control = eflow_control;
        mestop_bits = estop_bits;
        mnbaud_rate = nbaud_rate;
        mncharacter_size = ncharacter_size;
    }

    bool ss_download_asio_serial_port::start_download(DWORD64 nFileSizeCounts,ps_sahara_protocol_armprg_file_info psSaharahProtocolArmPrgFile,s_streaming_download_info* ps_streaming_download_info,bool bStreamingDownload /* = true */)
    {
        //! init 
        msSSDownloadAsioConfigInfo.mnFileSizeCounts = nFileSizeCounts;
        msSSDownloadAsioConfigInfo.mpsSahara = psSaharahProtocolArmPrgFile;
        msSSDownloadAsioConfigInfo.mpsStreaming = ps_streaming_download_info;
        test_time.restart();
        mbStreamingDownload = bStreamingDownload;
        msDownloadProgressInfo.mdw64SendCounts = 0;
  
        msDownloadProgressInfo.meDownloadStatus = E_BUSY_DOWNLOAD;
        dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"start sahara protocol download\n");

        return start_sahara_download(mbStreamingDownload);
    }
    /*#pragma endregion*/
    /************************************************************************/
    /*   sahara protocol                                                    */
    /************************************************************************/

    bool ss_download_asio_serial_port::reopen_serial_port()
    {
        boost::system::error_code ec;
        if (sp.is_open()) sp.close(ec);
        sp.open(msDownloadProgressInfo.mstrComPort.c_str(),ec);
        CHECK_ERRORCODE_RETURN(ec,"open port fail(%s)\n",false);
        sp.set_option(boost::asio::serial_port::baud_rate(mnbaud_rate));
        sp.set_option(boost::asio::serial_port::character_size(mncharacter_size));
        sp.set_option(boost::asio::serial_port::flow_control(meflow_control));
        sp.set_option(boost::asio::serial_port::stop_bits(mestop_bits));

        return sp.is_open();
    }

    bool ss_download_asio_serial_port::start_sahara_download(bool bFirehoseDownload /* = false */)
    {
        if (!msSSDownloadAsioConfigInfo.mpsSahara)
        {
            send_download_error_msg("配置信息不能为空\n");
            return false;
        }
        //! init sahara status
        {
            mbStreamingDownload = bFirehoseDownload;
            sahara.offset = 0;
            msDownloadProgressInfo.meDownloadStatus = E_BUSY_DOWNLOAD;
            sahara.sahara_state = SAHARA_WAIT_HELLO;
            sahara.sahara_hello->version = 2;
            sahara.sahara_hello->mode = 0x00;
            memset(&ms_sahara_hello_memory,0x00,sizeof(sahara_packet_hello));
        }
        //! 打开下载端口
        if(!reopen_serial_port())
        {
            send_download_error_msg("open serial port fail\n");
            return false;
        }
        dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"open serial port(%s) ok\n",msDownloadProgressInfo.mstrComPort.c_str());
#if 1
        //! 读取设备的返回字符串 实际也可以直接忽略设备的返回字符串，直接发送sahara hello请求给设备
        async_read_port_time_out(boost::asio::buffer(mrx_buf,MAX_STRING_SIZE),boost::bind(&ss_download_asio_serial_port::read_sahara_download_to_port,this,_1,_2));
#else
        send_sahara_hello_resp();
#endif
        return true;
    }

    void ss_download_asio_serial_port::send_sahara_hello_resp()
    {
        sahara.sahara_hello_resp->header.command = SAHARA_HELLO_RESP_ID;
        sahara.sahara_hello_resp->header.length = sizeof(sahara_packet_hello_resp);
        sahara.sahara_hello_resp->version = sahara.sahara_hello->version;
        sahara.sahara_hello_resp->version_supported = sahara.sahara_hello->version;
        sahara.sahara_hello_resp->status = SAHARA_STATUS_SUCCESS;
        sahara.sahara_hello_resp->mode = sahara.sahara_hello->mode;

        ms_sahara_hello_memory.header.command = SAHARA_HELLO_ID;
        ms_sahara_hello_memory.header.length = sizeof(sahara_packet_hello);;
        ms_sahara_hello_memory.version = sahara.sahara_hello->version;
        ms_sahara_hello_memory.version_supported = sahara.sahara_hello->version;
        //ms_sahara_hello_memory.cmd_packet_length
        ms_sahara_hello_memory.mode = SAHARA_MODE_MEMORY_DEBUG;

        sahara.sahara_hello_resp->reserved0 = 1;
        sahara.sahara_hello_resp->reserved1 = 2;
        sahara.sahara_hello_resp->reserved2 = 3;
        sahara.sahara_hello_resp->reserved3 = 4;
        sahara.sahara_hello_resp->reserved4 = 5;
        sahara.sahara_hello_resp->reserved5 = 6;
        async_write_port_time_out(boost::asio::buffer(sahara.sahara_hello_resp,sizeof(sahara_packet_hello_resp)),
            boost::bind(&ss_download_asio_serial_port::write_sahara_download_to_port,this,_1,_2,sizeof(sahara_packet_hello_resp)));
    }

    void ss_download_asio_serial_port::send_sahara_armprg_file_image()
    {
        if (7 == sahara.sahara_read_data->image_id)
        {
            sArmPrgFileInfo.mfile_buf = msSSDownloadAsioConfigInfo.mpsSahara->ms_NArmPrgFileInfo.mfile_buf;
            sArmPrgFileInfo.mfile_len = msSSDownloadAsioConfigInfo.mpsSahara->ms_NArmPrgFileInfo.mfile_len;
        }
        else
        {
            sArmPrgFileInfo.mfile_buf = msSSDownloadAsioConfigInfo.mpsSahara->ms_EnArmPrgFileInfo.mfile_buf;
            sArmPrgFileInfo.mfile_len = msSSDownloadAsioConfigInfo.mpsSahara->ms_EnArmPrgFileInfo.mfile_len;
        }

        //! 编译量+数据长度大于文件长度的时候报错
        if (sahara.sahara_read_data->data_offset + sahara.sahara_read_data->data_length > sArmPrgFileInfo.mfile_len)
        {
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex, "Invalid length %d bytes request to be transmitted", sahara.sahara_read_data->data_length);
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"File offset %d, file size %d", sahara.sahara_read_data->data_offset, sArmPrgFileInfo.mfile_len);
            send_download_error_msg("");
            return;
        }

        async_write_port_time_out(boost::asio::buffer((char*)sArmPrgFileInfo.mfile_buf + sahara.sahara_read_data->data_offset,
            (size_t)sahara.sahara_read_data->data_length),
            boost::bind(&ss_download_asio_serial_port::write_sahara_download_to_port,
            this,_1,_2,(size_t)sahara.sahara_read_data->data_length));
    }

    void ss_download_asio_serial_port::send_sahara_image_end_done()
    {
        sahara.sahara_done->header.command = SAHARA_DONE_ID;
        sahara.sahara_done->header.length = sizeof(sahara_packet_done);
        sahara.sahara_state = SAHARA_WAIT_DONE_RESP;
        async_write_port_time_out(boost::asio::buffer(sahara.sahara_done,sizeof(sahara_packet_done)),
            boost::bind(&ss_download_asio_serial_port::write_sahara_download_to_port,this,_1,_2,sizeof(sahara_packet_done)));
    }

    void ss_download_asio_serial_port::send_sahara_reset()
    {
        sahara.sahara_reset->header.command = SAHARA_RESET_ID;
        sahara.sahara_reset->header.length = sizeof(sahara_packet_reset);
        sahara.sahara_state = SAHARA_WAIT_RESET_RESP;
        async_write_port_time_out(boost::asio::buffer(sahara.sahara_reset,sizeof(sahara_packet_reset)),
            boost::bind(&ss_download_asio_serial_port::write_sahara_download_to_port,this,_1,_2,sizeof(sahara_packet_reset)));
    }

    void ss_download_asio_serial_port::send_sahara_image_end_done_resp()
    {
        //!if (SAHARA_MODE_IMAGE_TX_COMPLETE == sahara.sahara_done_resp->image_tx_status) dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"\n\nSuccessfully uploaded all images\n");

        //! 提示下载完成
        close_serial_port();
        dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"Sahara protocol Download Successfully!!!\n");

        //! 开启firehose 下载
        if (mbStreamingDownload) 
        {
            start_streaming_download(msSSDownloadAsioConfigInfo.mnFileSizeCounts,msSSDownloadAsioConfigInfo.mpsStreaming);
            return;
        }
        //! 是否进行firehose下载 默认为false 
        send_download_ok_msg("download complete\n");
    }

    bool ss_download_asio_serial_port::check_read_len_is_empty(size_t read_len)
    {
        if (read_len > 0 && read_len < MAX_STRING_SIZE - 1) return false;
        send_download_error_msg("port read buf is empty\n");
        return true;
    }

    bool ss_download_asio_serial_port::check_write_len_is_not_complete(size_t predict_length,size_t complete_length)
    {
        if (predict_length == complete_length) return false;
        dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"port write buf predict_length(%d) != complete_length(%d)\n",predict_length,complete_length);
        send_download_error_msg("");
        return true;
    }

    void ss_download_asio_serial_port::read_sahara_download_to_port(const boost::system::error_code& ec,size_t rx_len)
    {
        if (check_read_len_is_empty(rx_len)) return;

        switch (sahara.sahara_state)
        {
        case SAHARA_WAIT_HELLO:
            send_sahara_hello_resp();
            break;
        case SAHARA_WAIT_COMMAND:
            //! 开始发送ARM PRG文件
            if (SAHARA_READ_DATA_ID == sahara.sahara_hello->header.command)
            {
                send_sahara_armprg_file_image();
            }
            //! ARM PRG文件发送完成 开始发送 SAHARA_DONE
            else if(SAHARA_END_IMAGE_TX_ID == sahara.sahara_hello->header.command)
            {
                if (!is_ack_successful(sahara.sahara_end_image_tx->status))
                {
                    dbg(LOG_INFO,msDownloadProgressInfo.mnIndex, "SENDING --> SAHARA_RESET\n");
                    send_sahara_reset();
                    break;
                }
                send_sahara_image_end_done();
            }
            break;
        case SAHARA_WAIT_DONE_RESP:
            send_sahara_switch_mode_packet();
            break;
        case SAHARA_WAIT_COMMAND_SWITCH_MODE:
            send_sahara_image_end_done_resp();
            break;
        case SAHARA_WAIT_RESET_RESP:
            send_download_error_msg("current device reset\n");
            break;
        }
    }

    void ss_download_asio_serial_port::send_sahara_switch_mode_packet()
    {
        if (SAHARA_MODE_IMAGE_TX_COMPLETE == sahara.sahara_done_resp->image_tx_status) 
        {
            dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"\n\nSuccessfully uploaded all images\n");
        }
        /*else
        {
        send_download_error_msg("shara download image fail\n");
        return ;
        }*/
        //ms_sahara_hello_memory

        //sahara.sahara_switch_mode->header.command = SAHARA_RESET_ID;
        //sahara.sahara_switch_mode->header.length = sizeof(sahara_switch_mode_packet);
        //sahara.sahara_switch_mode->mode = 2;
        sahara.sahara_state = SAHARA_WAIT_COMMAND_SWITCH_MODE;
        async_write_port_time_out(boost::asio::buffer(&ms_sahara_hello_memory,sizeof(sahara_packet_hello)),
            boost::bind(&ss_download_asio_serial_port::write_sahara_switch_mode_packet_to_port,this,_1,_2,sizeof(sahara_packet_hello)));
    }

    void ss_download_asio_serial_port::write_sahara_switch_mode_packet_to_port(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len)
    {
        if (check_write_len_is_not_complete(tx_len,tx_len_complete)) return;
        msDownloadProgressInfo.mdw64SendCounts += tx_len_complete;
        read_sahara_download_to_port(ec,1);
    }

    void ss_download_asio_serial_port::write_sahara_download_to_port(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len)
    {
        if (check_write_len_is_not_complete(tx_len,tx_len_complete)) return;

        msDownloadProgressInfo.mdw64SendCounts += tx_len_complete;
        switch (sahara.sahara_state)
        {
        case SAHARA_WAIT_HELLO:
            //! async_read_port_time_out(boost::asio::buffer(mrx_buf,MAX_STRING_SIZE),boost::bind(&ss_download_asio_serial_port::read_sahara_download_to_port,this,_1,_2));
            sahara.sahara_state = SAHARA_WAIT_COMMAND;
            break;
        case SAHARA_WAIT_COMMAND:
        case SAHARA_WAIT_DONE_RESP:
        case SAHARA_WAIT_RESET_RESP:   
            break;
        default:
            break;
        }

        async_read_port_time_out(boost::asio::buffer(mrx_buf,MAX_STRING_SIZE),boost::bind(&ss_download_asio_serial_port::read_sahara_download_to_port,this,_1,_2));
    }

    /************************************************************************/
    /* streaming protocol                                                    */
    /************************************************************************/

    void ss_download_asio_serial_port::init_command()
    {
        if (msSSDownloadAsioConfigInfo.mnPos >= msSSDownloadAsioConfigInfo.mpsStreaming->size())
        {
            send_download_error_msg("command array out\n");
            return;
        }
        mps_stream_protocol_command = &msSSDownloadAsioConfigInfo.mpsStreaming->at(msSSDownloadAsioConfigInfo.mnPos);
        ps_stream_frame_header psStreamFrameHeader = (ps_stream_frame_header)mps_stream_protocol_command->mcommand_buf;
        meStreamingDownloadCmdEnum = (Streaming_Download_Cmd_Enum)psStreamFrameHeader->command;
        dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"DonwloadCmd:%s\n",szStreamingDownloadCmdEnumString[meStreamingDownloadCmdEnum]);
        msSSDownloadAsioConfigInfo.mnPos++;
    }

    bool ss_download_asio_serial_port::start_streaming_download(DWORD64 nFileSizeCounts,s_streaming_download_info* psStreamingDownloadInfo)
    {
        msSSDownloadAsioConfigInfo.mnFileSizeCounts = nFileSizeCounts;
        msSSDownloadAsioConfigInfo.mnPos = 0;
        msSSDownloadAsioConfigInfo.mpsStreaming = psStreamingDownloadInfo;
        msDownloadProgressInfo.meDownloadStatus = E_BUSY_DOWNLOAD;

        dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"start streaming protocol download\n");
        if (!msSSDownloadAsioConfigInfo.mpsStreaming)
        {
            send_download_error_msg("配置信息不能为空\n");
            return false;
        }

        //! 打开下载端口
        if(!reopen_serial_port())
        {
            send_download_error_msg("open serial port fail\n");
            return false;
        }
        dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"open serial port(%s) ok\n",msDownloadProgressInfo.mstrComPort.c_str());
        //! 异步发送命令
        init_command();
        async_write_port_time_out(boost::asio::buffer(mps_stream_protocol_command->mcommand_buf,mps_stream_protocol_command->mcommand_len),
            boost::bind(&ss_download_asio_serial_port::write_streaming_hello_command,this,_1,_2,mps_stream_protocol_command->mcommand_len));

        return true;
    }

    void ss_download_asio_serial_port::write_streaming_hello_command(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len)
    {
        if (check_write_len_is_not_complete(tx_len,tx_len_complete)) return;
        msDownloadProgressInfo.mdw64SendCounts += tx_len;
        async_read_port_time_out(boost::asio::buffer(mrx_buf,MAX_STRING_SIZE),boost::bind(&ss_download_asio_serial_port::read_streaming_hello_response_command,this,_1,_2));
    }

    void ss_download_asio_serial_port::read_streaming_hello_response_command(const boost::system::error_code& ec,size_t rx_len)
    {
        //! 很大可能出现rx_len为空情况 重新读端口 反应不过来
        if (0 == rx_len)
        {
            dbg(LOG_WARN,msDownloadProgressInfo.mnIndex,"read len is empty \n");
            if (sp.is_open()) async_read_port_time_out(boost::asio::buffer(mrx_buf,MAX_STRING_SIZE),boost::bind(&ss_download_asio_serial_port::read_streaming_hello_response_command,this,_1,_2));
            return ;
        }
        mrx_buf[rx_len - 1] = '\0';
        print_error_command(rx_len);
        switch ((Streaming_Download_Cmd_Enum)streaming.mpsStreamCommand->command)
        {
        case HELLO_RSP_CMD:
            if (HELLO_CMD != meStreamingDownloadCmdEnum){ send_download_error_msg("send command and revc command mismatches \n"); return; }
            mMaximum_write_len = streaming.mpStreamHelloResponsePacket->maximum;
            if (mMaximum_write_len <= 0) mMaximum_write_len = 1024;
            //! mMaximum_write_len = 1536/*1024 * 2*/;
            dbg(LOG_WARN,msDownloadProgressInfo.mnIndex,"Maximum data size acceptable for a write command(%d)\n",mMaximum_write_len);
            break;
        case DWNLDR_SECURITY_MODE_RSP_CMD:
            if (DWNLDR_SECURITY_MODE_CMD != meStreamingDownloadCmdEnum){ send_download_error_msg("send command and revc command mismatches \n"); return; }
            dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"%s \n",szStreamingDownloadCmdEnumString[streaming.mpsStreamCommand->command]);
            break;
        case DWNLDR_PRTN_TBL_RSP_CMD:
            if (DWNLDR_PRTN_TBL_CMD != meStreamingDownloadCmdEnum){ send_download_error_msg("send command and revc command mismatches \n"); return; }
            dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"%s \n",szStreamingDownloadCmdEnumString[streaming.mpsStreamCommand->command]);
            break;
        case ERROR_CMD:
            check_streaming_error_code(streaming.mpsStreamingErrorCode->error_code);
            send_download_error_msg("");
            return;
        case LOG_CMD:  
            for (size_t i = 2;i < rx_len;i++) if (0x0A == streaming.mpsStreamingLoginfo->log[i]) streaming.mpsStreamingLoginfo->log[i] = '\0';
            dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"error log: %s\n",streaming.mpsStreamingLoginfo->log);
            send_download_error_msg("");
            return;
        case DWNLDR_MULTI_IMG_OPEN_RSP_CMD:
            if (DWNLDR_MULTI_IMG_OPEN_CMD != meStreamingDownloadCmdEnum){ send_download_error_msg("send command and revc command mismatches \n"); return; }
            response_streaming_multi_image_open_cmd();
            return;
        case DWNLDR_CLOSE_RSP_CMD:
            if (DWNLDR_CLOSE_CMD != meStreamingDownloadCmdEnum){ send_download_error_msg("send command and revc command mismatches \n"); return; }
            dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"%s \n",szStreamingDownloadCmdEnumString[streaming.mpsStreamCommand->command]);
           //! send_download_error_msg("Error response \n");
            break;
        case STRM_RESET_RSP_CMD:
            if (STRM_RESET_CMD != meStreamingDownloadCmdEnum){ send_download_error_msg("send command and revc command mismatches \n"); return; }
            dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"%s \n",szStreamingDownloadCmdEnumString[streaming.mpsStreamCommand->command]);
            break;
        default:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"unkonw response command: %d",streaming.mpsStreamCommand->command);
            print_error_command(rx_len);
            send_download_error_msg("");
            return;
        }
        //! 下载完成
        if (msSSDownloadAsioConfigInfo.mnPos >= msSSDownloadAsioConfigInfo.mpsStreaming->size())
        {
            send_download_ok_msg("Streaming protocol Download Successfully!!!\n");
            return ;
        }
        //! 初始化下一条命令
        init_command();
        async_write_port_time_out(boost::asio::buffer(mps_stream_protocol_command->mcommand_buf,mps_stream_protocol_command->mcommand_len),
            boost::bind(&ss_download_asio_serial_port::write_streaming_hello_command,this,_1,_2,mps_stream_protocol_command->mcommand_len));
    }

    void ss_download_asio_serial_port::print_error_command(size_t read_len)
    {
#ifdef _DEBUG
        char szTemp[4096] = {0};
        char szBuffer[10] = {0};
        for(size_t i = 0; i < read_len; i++)
        {
            if(i%16 == 0) strcat_s(szTemp,4096,"\n");
            sprintf_s(szBuffer,10, " R%02X", (UINT8)mrx_buf[i]);
            strcat_s(szTemp,4096,szBuffer);
        }
        strcat_s(szTemp,4096,"\n");
        dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,szTemp);
#endif
    }

    void ss_download_asio_serial_port::response_streaming_multi_image_open_cmd()
    {
        msFileTempBufInfo.mnFile_Pos = 0;
        msFileTempBufInfo.mnBuf_len = 0;
        msFileTempBufInfo.mnBuf_Pos = 0;

        boost::shared_ptr<FILE> fp(fopen (mps_stream_protocol_command->mstrPartitionMBNPath.c_str(), "rb"),JGW_CloseFilePtr_C);
        dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"start send program:%s\n",mps_stream_protocol_command->mstrPartitionMBNPath.c_str());
        if (!fp) 
        {
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"open fail(%s)\n",mps_stream_protocol_command->mstrPartitionMBNPath.c_str());
            send_download_error_msg("");
            return;
        }
        //! 首次读取文件大小，判断文件是否为空文件
        msFileTempBufInfo.mnBuf_len = fread (msFileTempBufInfo.mnFileTempBuf, sizeof (char), FIREHOSE_TX_BUFFER_SIZE, fp.get());
        if (msFileTempBufInfo.mnBuf_len <= 0)
        {
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"read file fail(%s)\n",mps_stream_protocol_command->mstrPartitionMBNPath.c_str());
            send_download_error_msg("");
            return;
        }
        write_streaming_multi_image_buf_to_port(fp);
    }

    void ss_download_asio_serial_port::write_streaming_multi_image_buf_to_port(boost::shared_ptr<FILE> fp)
    {
        //! 缓存读取的缓存文件buf的大小为0的时候,重新读取
        if (0 == msFileTempBufInfo.mnBuf_len)
        {
            msFileTempBufInfo.mnBuf_len = fread (msFileTempBufInfo.mnFileTempBuf, sizeof (char), FIREHOSE_TX_BUFFER_SIZE, fp.get());
            msFileTempBufInfo.mnBuf_Pos = 0;
        }
        //! 当读到文件尾时，当前文件写入完成
        if (0 >= msFileTempBufInfo.mnBuf_len)
        {
            init_command();
            async_write_port_time_out(boost::asio::buffer(mps_stream_protocol_command->mcommand_buf,mps_stream_protocol_command->mcommand_len),
                boost::bind(&ss_download_asio_serial_port::write_streaming_hello_command,this,_1,_2,mps_stream_protocol_command->mcommand_len));
            dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"send program image ok\n");
            return ;
        }
        //! multi大小大于最大栈大小的时候
        size_t pakcet_len = build_unframe_streaming_multi_image_packet(msFileTempBufInfo.mnBuf_len >= mMaximum_write_len?mMaximum_write_len:msFileTempBufInfo.mnBuf_len);
        async_write_port_time_out(boost::asio::buffer(mtx_buf,pakcet_len),
            boost::bind(&ss_download_asio_serial_port::write_streaming_download_image_file,this,_1,_2,pakcet_len,fp));
        
    }

    size_t ss_download_asio_serial_port::build_unframe_streaming_multi_image_packet(size_t read_len)
    {
        ps_streaming_unframe_stream_write_packet psStreamWritePacket = (ps_streaming_unframe_stream_write_packet)mtx_buf;
        psStreamWritePacket->command = UNFRAMED_STREAM_WRITE_CMD;
        meStreamingDownloadCmdEnum = UNFRAMED_STREAM_WRITE_CMD;
        psStreamWritePacket->padding_bits[0] = 0;
        psStreamWritePacket->padding_bits[1] = 0;
        psStreamWritePacket->padding_bits[2] = 0;
        psStreamWritePacket->address = msFileTempBufInfo.mnFile_Pos;
        psStreamWritePacket->length = read_len;
        memcpy(psStreamWritePacket->data,msFileTempBufInfo.mnFileTempBuf + msFileTempBufInfo.mnBuf_Pos,read_len);
        msFileTempBufInfo.mnFile_Pos += read_len;
        msFileTempBufInfo.mnBuf_len -= read_len;
        msFileTempBufInfo.mnBuf_Pos += read_len;
        return read_len + sizeof(s_streaming_unframe_stream_write_packet) - 1;
    }

    size_t ss_download_asio_serial_port::build_streaming_multi_image_packet(size_t read_len)
    {
        ps_stream_write_packet psStreamWritePacket = (ps_stream_write_packet)msFileTempBufInfo.mnFileTempBuf;
        psStreamWritePacket->command = STRM_WRITE_CMD;
        meStreamingDownloadCmdEnum = STRM_WRITE_CMD;
        psStreamWritePacket->address = msFileTempBufInfo.mnFile_Pos;
        memcpy((char*)psStreamWritePacket->data,mtx_buf,read_len);
        msFileTempBufInfo.mnFile_Pos += read_len;
        return mFramePacket.BuildDataStreamPacketToBuf((BYTE*)psStreamWritePacket,sizeof(s_stream_write_packet) + read_len - 1,mtx_buf);
    }

    void ss_download_asio_serial_port::write_streaming_download_image_file(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len,boost::shared_ptr<FILE> fp)
    {
    
        if (check_write_len_is_not_complete(tx_len,tx_len_complete)) return;
        msDownloadProgressInfo.mdw64SendCounts += tx_len;
        //! dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"write : %d\n",msFileTempBufInfo.mnFile_Pos);
        async_read_port_time_out(boost::asio::buffer(mrx_buf,MAX_STRING_SIZE),boost::bind(&ss_download_asio_serial_port::read_streaming_download_image_file_response,this,_1,_2,fp));
    }

    void ss_download_asio_serial_port::read_streaming_download_image_file_response(const boost::system::error_code& ec,size_t rx_len,boost::shared_ptr<FILE> fp)
    {
        //! 很大可能出现rx_len为空情况反应不过来
        if (0 == rx_len)
        {
            dbg(LOG_WARN,msDownloadProgressInfo.mnIndex,"read len is empty \n");
            if (sp.is_open()) async_read_port_time_out(boost::asio::buffer(mrx_buf,MAX_STRING_SIZE),boost::bind(&ss_download_asio_serial_port::read_streaming_download_image_file_response,this,_1,_2,fp));
            return ;
        }
        mrx_buf[rx_len - 1] = '\0';
        print_error_command(rx_len);

        switch ((Streaming_Download_Cmd_Enum)streaming.mpsStreamCommand->command)
        {
        case ERROR_CMD:
            check_streaming_error_code(streaming.mpsStreamingErrorCode->error_code);
            print_error_command(rx_len);
            send_download_error_msg("");
            return;
        case LOG_CMD:  
            for (size_t i = 2;i < rx_len;i++) if (0x0A == streaming.mpsStreamingLoginfo->log[i]) streaming.mpsStreamingLoginfo->log[i] = '\0';
            dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"error log: %s\n",streaming.mpsStreamingLoginfo->log);
            send_download_error_msg("");
            print_error_command(rx_len);
            return;
        case STRM_WRITE_RSP_CMD:
            if (STRM_WRITE_CMD != meStreamingDownloadCmdEnum){ send_download_error_msg("send command and revc command mismatches \n"); return; }
            break;
        case UNFRAMED_STREAM_WRITE_RSP_CMD:
            if (UNFRAMED_STREAM_WRITE_CMD != meStreamingDownloadCmdEnum){ send_download_error_msg("send command and revc command mismatches \n"); return; }
            break;
        default:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"unkonw response command: %d",streaming.mpsStreamCommand->command);
            print_error_command(rx_len);
            send_download_error_msg("");
            return;
        }
        write_streaming_multi_image_buf_to_port(fp);
    }

    void ss_download_asio_serial_port::check_streaming_error_code(UINT32 error_code)
    {
        switch (error_code)
        {
        case E_STCMD_IIEGAL:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Illegal reason (do not use)\n");
            break;
        case E_STCMD_RESERVED:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Reserved\n");
            break;
        case E_STCMD_INV_DEST_ADDRESS:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Invalid destination address\n");
            break;
        case E_STCMD_INV_LEN:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Invalid length\n");
            break;
        case E_STCMD_UNEXPECTED:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Unexpected end of packet\n");
            break;
        case E_STCMD_INV_CMD:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Invalid command\n");
            break;
        case E_STCMD_RESERVERD_SIX:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Reserved\n");
            break;
        case E_STCMD_OPERATION:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Operation failed\n");
            break;
        case E_STCMD_WRONG_FLAS:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Wrong Flash intelligent ID\n");
            break;
        case E_STCMD_BAD_PROGRAM:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Bad programming voltage\n");
            break;
        case E_STCMD_WRITE_VERIFY:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Write-verify failed\n");
            break;
        case E_STCMD_RESERVED_B:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Reserved\n");
            break;
        case E_STCMD_INCORRECT_SECURITY:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Incorrect security code\n");
            break;
        case E_STCMD_CANNOT_POWER_DOWN:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Cannot power down phone\n");
            break;
        case E_STCMD_NAND_FLASH_RPOGRAM_NOT_SUPPORT:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"NAND Flash programming not supported\n");
            break;
        case E_STCMD_COMMAND_OUT_OF_SEQ:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Command out of sequence\n");
            break;
        case E_STCMD_CLOSE_DID_NOT_SUCCEED:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Close did not succeed\n");
            break;
        case E_STCMD_INCOMPATIBLE_FEATURE_BITS:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Incompatible feature bits\n");
            break;
        case E_STCMD_OUT_OF_SPACE:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Out of space\n");
            break;
        case E_STCMD_INVALID_SEC_MODE:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Invalid Security mode\n");
            break;
        case E_STCMD_MULTI_IMAGE_NAND_NOT_SUPPORT:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Multi-image NAND not supported\n");
            break;
        case E_STCMD_POWER_OFF:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"Power off command not supported\n");
            break;
        default:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"error_code:%d(%s)\n",error_code,streaming.mpsStreamingErrorCode->text_data);
            break;
        }
    }

    /************************************************************************/
    /* common func                                                          */
    /************************************************************************/
    void ss_download_asio_serial_port::send_download_error_msg(const char* msg)
    {
        close_serial_port();
        msDownloadProgressInfo.meDownloadStatus = E_ERROR_DOWNLOAD;
        dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,msg);
        dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"download time: %.2lf s \n",test_time.elapsed());
    }

    void ss_download_asio_serial_port::send_download_ok_msg(const char* msg)
    {
        close_serial_port();
        msDownloadProgressInfo.meDownloadStatus = E_SUCCESSFUL_DOWNLOAD;
        dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,msg);
        dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"download time: %.2lf s \n",test_time.elapsed());
    }


    void ss_download_asio_serial_port::close_serial_port()
    {
        boost::system::error_code ec;
        if (sp.is_open()) sp.close(ec);
        PRINT_ERRORCODE(ec,"close_serial_port(%s)\n");
        deadline_.expires_at(boost::posix_time::pos_infin,ec);
        PRINT_ERRORCODE(ec,"close_serial_port(%s)\n");
        dbg(LOG_WARN,msDownloadProgressInfo.mnIndex,"close_serial_port\n");
    }

    void ss_download_asio_serial_port::check_deadline(const boost::system::error_code e)  
    {
        if (e) return;
        //! 真正的等待超时
        if (deadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())  
        {
            send_download_error_msg("wait time out\n");	
            return;
        }  

#if 0
        //! 检查端口打开，但是被关闭的情况
        if (mbOpen_Port && !sp.is_open())
        {
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,"check_deadline port is not open\n");
            msDownloadProgressInfo.meDownloadStatus = E_ERROR_DOWNLOAD;
            sp.close();// 关闭对应的socket 包括连接超时/recv超时  
            deadline_.expires_at(boost::posix_time::pos_infin);// 定时器 设定为永不超时/不可用状态  
            return ;
        }
#endif
        //! 检查端口是否已经打开
        //! if (!sp.is_open()/* && !mbOpen_Port*/ ) return;



        //! 如果不是真正的超时，定是其操作已成功/用户重新设置了定时器  
        //! 重新启动定时器  
        //! deadline_.async_wait(boost::bind(&download_asio_serial_port::check_deadline, this));  
    }

    bool ss_download_asio_serial_port::is_ack_successful (int status)
    {
        switch (status) {

            /*Success*/
        case SAHARA_STATUS_SUCCESS :
            dbg(LOG_INFO,msDownloadProgressInfo.mnIndex, "SAHARA_STATUS_SUCCESS\n");
            return true;

            /*Invalid command received in current state*/
        case SAHARA_NAK_INVALID_CMD:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex, "SAHARA_NAK_INVALID_CMD\n");
            break;

            /*Protocol mismatch between host and target*/
        case SAHARA_NAK_PROTOCOL_MISMATCH:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex, "SAHARA_NAK_PROTOCOL_MISMATCH\n");
            break;

            /*Invalid target protocol version*/
        case SAHARA_NAK_INVALID_TARGET_PROTOCOL:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_INVALID_TARGET_PROTOCOL\n");
            break;

            /*Invalid host protocol version*/
        case SAHARA_NAK_INVALID_HOST_PROTOCOL:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_INVALID_HOST_PROTOCOL\n");
            break;

            /*Invalid packet size received*/
        case SAHARA_NAK_INVALID_PACKET_SIZE:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_INVALID_PACKET_SIZE\n");
            break;

            /*Unexpected image ID received*/
        case SAHARA_NAK_UNEXPECTED_IMAGE_ID:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_UNEXPECTED_IMAGE_ID\n");
            break;

            /*Invalid image header size received*/
        case SAHARA_NAK_INVALID_HEADER_SIZE:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_INVALID_HEADER_SIZE\n");
            break;

            /*Invalid image data size received*/
        case SAHARA_NAK_INVALID_DATA_SIZE:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_INVALID_DATA_SIZE\n");
            break;

            /*Unsupported image type received*/
        case SAHARA_NAK_INVALID_IMAGE_TYPE:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_INVALID_IMAGE_TYPE\n");
            break;

            /*Invalid tranmission length*/
        case SAHARA_NAK_INVALID_TX_LENGTH:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_INVALID_TX_LENGTH\n");
            break;

            /*Invalid reception length*/
        case SAHARA_NAK_INVALID_RX_LENGTH :
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_INVALID_RX_LENGTH\n");
            break;

            /*General transmission or reception error*/
        case  SAHARA_NAK_GENERAL_TX_RX_ERROR:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_GENERAL_TX_RX_ERROR\n");
            break;

            /*Error while transmitting READ_DATA packet*/
        case SAHARA_NAK_READ_DATA_ERROR:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_READ_DATA_ERROR\n");
            break;

            /*Cannot receive specified number of program headers*/
        case SAHARA_NAK_UNSUPPORTED_NUM_PHDRS:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_UNSUPPORTED_NUM_PHDRS\n");
            break;

            /*Invalid data length received for program headers*/
        case SAHARA_NAK_INVALID_PDHR_SIZE:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_INVALID_PDHR_SIZE\n");
            break;

            /*Multiple shared segments found in ELF image*/
        case SAHARA_NAK_MULTIPLE_SHARED_SEG:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_MULTIPLE_SHARED_SEG\n");
            break;

            /*Uninitialized program header location*/
        case SAHARA_NAK_UNINIT_PHDR_LOC:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_UNINIT_PHDR_LOC\n");
            break;

            /* Invalid destination address*/
        case  SAHARA_NAK_INVALID_DEST_ADDR:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_INVALID_DEST_ADDR\n");
            break;

            /* Invalid data size receieved in image header*/
        case SAHARA_NAK_INVALID_IMG_HDR_DATA_SIZE:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_INVALID_IMG_HDR_DATA_SIZE\n");
            break;

            /* Invalid ELF header received*/
        case SAHARA_NAK_INVALID_ELF_HDR:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_INVALID_ELF_HDR\n");
            break;

            /* Unknown host error received in HELLO_RESP*/
        case SAHARA_NAK_UNKNOWN_HOST_ERROR:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_UNKNOWN_HOST_ERROR\n");
            break;

            // Timeout while receiving data
        case SAHARA_NAK_TIMEOUT_RX:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_TIMEOUT_RX\n");
            break;

            // Timeout while transmitting data
        case SAHARA_NAK_TIMEOUT_TX:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_TIMEOUT_TX\n");
            break;

            // Invalid mode received from host
        case SAHARA_NAK_INVALID_HOST_MODE:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_INVALID_HOST_MODE\n");
            break;

            // Invalid memory read access
        case SAHARA_NAK_INVALID_MEMORY_READ:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_INVALID_MEMORY_READ\n");
            break;

            // Host cannot handle read data size requested
        case SAHARA_NAK_INVALID_DATA_SIZE_REQUEST:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_INVALID_DATA_SIZE_REQUEST\n");
            break;

            // Memory debug not supported
        case SAHARA_NAK_MEMORY_DEBUG_NOT_SUPPORTED:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_MEMORY_DEBUG_NOT_SUPPORTED\n");
            break;

            // Invalid mode switch
        case SAHARA_NAK_INVALID_MODE_SWITCH:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_INVALID_MODE_SWITCH\n");
            break;

            // Failed to execute command
        case SAHARA_NAK_CMD_EXEC_FAILURE:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_CMD_EXEC_FAILURE\n");
            break;

            // Invalid parameter passed to command execution
        case SAHARA_NAK_EXEC_CMD_INVALID_PARAM:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_EXEC_CMD_INVALID_PARAM\n");
            break;

            // Unsupported client command received
        case SAHARA_NAK_EXEC_CMD_UNSUPPORTED:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_EXEC_CMD_UNSUPPORTED\n");
            break;

            // Invalid client command received for data response
        case SAHARA_NAK_EXEC_DATA_INVALID_CLIENT_CMD:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_EXEC_DATA_INVALID_CLIENT_CMD\n");
            break;

            // Failed to authenticate hash table
        case SAHARA_NAK_HASH_TABLE_AUTH_FAILURE:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_HASH_TABLE_AUTH_FAILURE\n");
            break;

            // Failed to verify hash for a given segment of ELF image
        case SAHARA_NAK_HASH_VERIFICATION_FAILURE:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex, "SAHARA_NAK_HASH_VERIFICATION_FAILURE\n");
            break;

            // Failed to find hash table in ELF image
        case SAHARA_NAK_HASH_TABLE_NOT_FOUND:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_HASH_TABLE_NOT_FOUND\n");
            break;

        case SAHARA_NAK_LAST_CODE:
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,"SAHARA_NAK_LAST_CODE\n");
            break;

        default:
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex, "Invalid status field %d\n", status);
            break;
        }
        return false;
    }
}
