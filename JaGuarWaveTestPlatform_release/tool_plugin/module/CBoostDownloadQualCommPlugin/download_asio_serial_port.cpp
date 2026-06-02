#include "StdAfx.h"
#include "download_asio_serial_port.h"
#include "CJGW_DownloadConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>


namespace JGW
{
	int JGW_GetPageSize(void) 
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

	bool download_asio_serial_port::mbGetMaxPayloadSizeToTargetInBytes = false;

	download_asio_serial_port::download_asio_serial_port(boost::asio::io_service& ios,int nIndex):
		sp(ios),
		deadline_(ios),
		mbFirehoseDownload(false),
        mps_download_all_file_info(NULL)
	{
		msDownloadProgressInfo.mnIndex = (nIndex);
		msDownloadProgressInfo.mdw64SendCounts = 0;
		msDownloadProgressInfo.meDownloadStatus = E_IDLE_DOWNLOAD;
		/*        
		mrx_buf = (char*)calloc(MAX_STRING_SIZE, 1);
		mtx_buf = (char*)calloc(FIREHOSE_TX_BUFFER_SIZE, 1);
		*/
		mrx_buf = (char*)memalign (JGW_GetPageSize (), MAX_STRING_SIZE);
		mtx_buf = (char*)memalign (JGW_GetPageSize (), FIREHOSE_TX_BUFFER_SIZE);

		memset(mtx_buf,0x00,FIREHOSE_TX_BUFFER_SIZE);
		memset(mrx_buf,0x00,MAX_STRING_SIZE);

		sahara.sahara_hello = (sahara_packet_hello*)mrx_buf;
		sahara.sahara_hello_resp = (sahara_packet_hello_resp*)mtx_buf;
		sahara.sahara_end_image_tx = (sahara_packet_end_image_tx*)mrx_buf;
		sahara.sahara_read_data = (sahara_packet_read_data*)mrx_buf;
        sahara.sahara_read_data_64bit = (sahara_packet_read_data_64bit*)mrx_buf;
		sahara.sahara_done = (sahara_packet_done*)mtx_buf;
		sahara.sahara_done_resp = (sahara_packet_done_resp*)mrx_buf;
		sahara.sahara_reset = (sahara_packet_reset*)mtx_buf;

		set_port_option();
	}
	/*#pragma region */
	download_asio_serial_port::~download_asio_serial_port(void)
	{
		close_serial_port();
		ALIGNED_FREE(mrx_buf);
		ALIGNED_FREE(mtx_buf);
		/*     
		free(mrx_buf);
		free(mtx_buf);
		*/
	}

//     s_download_file_info& download_asio_serial_port::get_armprg_file_info()
//     {
//         static s_download_file_info sDonwloadFileInfo = {0};
//         return sDonwloadFileInfo;
//     }

	void download_asio_serial_port::set_port_option(int nbaud_rate /* = 9600 */, int ncharacter_size /* = 8 */, boost::asio::serial_port::flow_control::type eflow_control /* = boost::asio::serial_port::flow_control::none */, boost::asio::serial_port::stop_bits::type estop_bits /* = boost::asio::serial_port::stop_bits::one */)
	{
		meflow_control = eflow_control;
		mestop_bits = estop_bits;
		mnbaud_rate = nbaud_rate;
		mncharacter_size = ncharacter_size;
	}

	bool download_asio_serial_port::start_download(ps_download_all_file_info psDownloadAllFileInfo)
	{
		//! init 
        mps_download_all_file_info = psDownloadAllFileInfo;
		test_time.restart();
		mbFirehoseDownload = true;
		msDownloadProgressInfo.mdw64SendCounts = 0;
        msfirehose_all_command_info.mpVFirehose = &psDownloadAllFileInfo->ms_firehose_download_info;
		msDownloadProgressInfo.meDownloadStatus = E_BUSY_DOWNLOAD;
		dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,L"start sahara protocol download\n");

        if (!mps_download_all_file_info)
        {
            send_download_error_msg(L"The configuration information cannot be empty\n");
            return false;
        }

		return start_sahara_download(mbFirehoseDownload);
	}
	/*#pragma endregion*/
	/************************************************************************/
	/*   sahara protocol                                                    */
	/************************************************************************/

	bool download_asio_serial_port::reopen_serial_port()
	{
		boost::system::error_code ec;
		if (sp.is_open()) sp.close(ec);
		sp.open(msDownloadProgressInfo.mstrComPort.c_str(),ec);
        CHECK_ERRORCODE_RETURN(ec,L"open port fail(%s)\n",false);
		sp.set_option(boost::asio::serial_port::baud_rate(mnbaud_rate));
		sp.set_option(boost::asio::serial_port::character_size(mncharacter_size));
		sp.set_option(boost::asio::serial_port::flow_control(meflow_control));
		sp.set_option(boost::asio::serial_port::stop_bits(mestop_bits));

		return sp.is_open();
	}

	bool download_asio_serial_port::start_sahara_download(bool bFirehoseDownload /* = false */)
	{
		//! init sahara status
		{
			mbFirehoseDownload = bFirehoseDownload;
			sahara.offset = 0;
			msDownloadProgressInfo.meDownloadStatus = E_BUSY_DOWNLOAD;
			sahara.sahara_state = SAHARA_WAIT_HELLO;
			sahara.sahara_hello->version = 2;
			sahara.sahara_hello->mode = 0x00;
		}
		//! 打开下载端口
		if(!reopen_serial_port())
		{
			send_download_error_msg(L"open serial port fail\n");
			return false;
		}
		dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,L"open serial port(%s) ok\n",JGW_A2W(msDownloadProgressInfo.mstrComPort).c_str());
#if 1
		//! 读取设备的返回字符串 实际也可以直接忽略设备的返回字符串，直接发送sahara hello请求给设备
		async_read_port_time_out(boost::asio::buffer(mrx_buf,MAX_STRING_SIZE),boost::bind(&download_asio_serial_port::read_sahara_download_to_port,this,_1,_2));
#else
		send_sahara_hello_resp();
#endif
		return true;
	}

	void download_asio_serial_port::send_sahara_hello_resp()
	{
		sahara.sahara_hello_resp->header.command = SAHARA_HELLO_RESP_ID;
		sahara.sahara_hello_resp->header.length = sizeof(sahara_packet_hello_resp);
		sahara.sahara_hello_resp->version = sahara.sahara_hello->version;
		sahara.sahara_hello_resp->version_supported = sahara.sahara_hello->version;
		sahara.sahara_hello_resp->status = SAHARA_STATUS_SUCCESS;
		sahara.sahara_hello_resp->mode = sahara.sahara_hello->mode;

		sahara.sahara_hello_resp->reserved0 = 1;
		sahara.sahara_hello_resp->reserved1 = 2;
		sahara.sahara_hello_resp->reserved2 = 3;
		sahara.sahara_hello_resp->reserved3 = 4;
		sahara.sahara_hello_resp->reserved4 = 5;
		sahara.sahara_hello_resp->reserved5 = 6;
		async_write_port_time_out(boost::asio::buffer(sahara.sahara_hello_resp,sizeof(sahara_packet_hello_resp)),
			boost::bind(&download_asio_serial_port::write_sahara_download_to_port,this,_1,_2,sizeof(sahara_packet_hello_resp)));
	}

    void download_asio_serial_port::send_sahara_armprg_64bit_file_image()
    {
        //! 编译量+数据长度大于文件长度的时候报错
        if (sahara.sahara_read_data_64bit->data_offset + sahara.sahara_read_data_64bit->data_length > mps_download_all_file_info->ms_sahara_arm_prg_file_info.file_len)
        {
            dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex, L"Invalid length %I64d bytes request to be transmitted", sahara.sahara_read_data_64bit->data_length);
            dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"File offset %I64d, file size %d", sahara.sahara_read_data_64bit->data_offset, mps_download_all_file_info->ms_sahara_arm_prg_file_info.file_len);
            send_download_error_msg(L"");
            return;
        }

        async_write_port_time_out(boost::asio::buffer((char*)mps_download_all_file_info->ms_sahara_arm_prg_file_info.file_buf + sahara.sahara_read_data_64bit->data_offset,
            (size_t)sahara.sahara_read_data_64bit->data_length),
            boost::bind(&download_asio_serial_port::write_sahara_download_to_port,
            this,_1,_2,(size_t)sahara.sahara_read_data_64bit->data_length));
    }

	void download_asio_serial_port::send_sahara_armprg_file_image()
	{
		//! 编译量+数据长度大于文件长度的时候报错
		if (sahara.sahara_read_data->data_offset + sahara.sahara_read_data->data_length > mps_download_all_file_info->ms_sahara_arm_prg_file_info.file_len)
		{
			dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex, L"Invalid length %d bytes request to be transmitted", sahara.sahara_read_data->data_length);
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"File offset %d, file size %d", sahara.sahara_read_data->data_offset, mps_download_all_file_info->ms_sahara_arm_prg_file_info.file_len);
			send_download_error_msg(L"");
			return;
		}

		async_write_port_time_out(boost::asio::buffer((char*)mps_download_all_file_info->ms_sahara_arm_prg_file_info.file_buf + sahara.sahara_read_data->data_offset,
			(size_t)sahara.sahara_read_data->data_length),
			boost::bind(&download_asio_serial_port::write_sahara_download_to_port,
			this,_1,_2,(size_t)sahara.sahara_read_data->data_length));
	}

	void download_asio_serial_port::send_sahara_image_end_done()
	{
		sahara.sahara_done->header.command = SAHARA_DONE_ID;
		sahara.sahara_done->header.length = sizeof(sahara_packet_done);
		sahara.sahara_state = SAHARA_WAIT_DONE_RESP;
		async_write_port_time_out(boost::asio::buffer(sahara.sahara_done,sizeof(sahara_packet_done)),
			boost::bind(&download_asio_serial_port::write_sahara_download_to_port,this,_1,_2,sizeof(sahara_packet_done)));
	}

	void download_asio_serial_port::send_sahara_reset()
	{
		sahara.sahara_reset->header.command = SAHARA_RESET_ID;
		sahara.sahara_reset->header.length = sizeof(sahara_packet_reset);
		sahara.sahara_state = SAHARA_WAIT_RESET_RESP;
		async_write_port_time_out(boost::asio::buffer(sahara.sahara_done,sizeof(sahara_packet_reset)),
			boost::bind(&download_asio_serial_port::write_sahara_download_to_port,this,_1,_2,sizeof(sahara_packet_reset)));
	}

	void download_asio_serial_port::send_sahara_image_end_done_resp()
	{
		if (SAHARA_MODE_IMAGE_TX_COMPLETE == sahara.sahara_done_resp->image_tx_status) dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,L"\n\nSuccessfully uploaded all images\n");

		//! 提示下载完成
		close_serial_port();
		dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,L"Sahara protocol Download Successfully!!!\n");

		//! 开启firehose 下载
		if (mbFirehoseDownload) 
		{
			start_firehose_download(msfirehose_all_command_info.mpVFirehose);
			return;
		}
		//! 是否进行firehose下载 默认为false 
		send_download_ok_msg(L"download complete\n");
	}

	bool download_asio_serial_port::check_read_len_is_empty(size_t read_len)
	{
		if (read_len > 0 && read_len < MAX_STRING_SIZE - 1) return false;
		send_download_error_msg(L"port read buf is empty\n");
		return true;
	}

	bool download_asio_serial_port::check_write_len_is_not_complete(size_t predict_length,size_t complete_length)
	{
		if (predict_length == complete_length) return false;
		send_download_error_msg(L"port write buf predict_length != complete_length\n");
		return true;
	}

	void download_asio_serial_port::read_sahara_download_to_port(const boost::system::error_code& ec,size_t rx_len)
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
				if (is_ack_successful(sahara.sahara_end_image_tx->status))
				{
					send_sahara_image_end_done();
				}
				else
				{
					dbg(LOG_INFO,msDownloadProgressInfo.mnIndex, L"SENDING --> SAHARA_RESET\n");
					send_sahara_reset();
				}
			}
            else if (SAHARA_64_BITS_READ_DATA_ID == sahara.sahara_hello->header.command)
            {
                send_sahara_armprg_64bit_file_image();
            }
			break;
		case SAHARA_WAIT_DONE_RESP:
			send_sahara_image_end_done_resp();
			break;
		case SAHARA_WAIT_RESET_RESP:
			send_download_error_msg(L"current device reset\n");
			break;
		}
	}

	void download_asio_serial_port::write_sahara_download_to_port(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len)
	{
		if (check_write_len_is_not_complete(tx_len,tx_len_complete)) return;

		msDownloadProgressInfo.mdw64SendCounts += tx_len_complete;
		switch (sahara.sahara_state)
		{
		case SAHARA_WAIT_HELLO:
			async_read_port_time_out(boost::asio::buffer(mrx_buf,MAX_STRING_SIZE),boost::bind(&download_asio_serial_port::read_sahara_download_to_port,this,_1,_2));
			sahara.sahara_state = SAHARA_WAIT_COMMAND;
			break;
		case SAHARA_WAIT_COMMAND:
		case SAHARA_WAIT_DONE_RESP:
		case SAHARA_WAIT_RESET_RESP:
			async_read_port_time_out(boost::asio::buffer(mrx_buf,MAX_STRING_SIZE),boost::bind(&download_asio_serial_port::read_sahara_download_to_port,this,_1,_2));
			break;
		}

	}

	/************************************************************************/
	/* firehose protocol                                                    */
	/************************************************************************/
	bool download_asio_serial_port::start_firehose_download(const s_firehose_download_info* mpVFirehose)
	{
		msfirehose_all_command_info.mpVFirehose = mpVFirehose;
		msfirehose_all_command_info.mnXMLPos = 0;
		msDownloadProgressInfo.meDownloadStatus = E_BUSY_DOWNLOAD;
		msfirehose_all_command_info.mbRawMode = false;
		rewrite_program_command = 0;

		dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,L"start firehose protocol download\n");
		//! 打开下载端口
		if(!reopen_serial_port())
		{
			send_download_error_msg(L"open serial port fail\n");
			return false;
		}
		dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,L"open serial port(%s) ok\n",JGW_A2W(msDownloadProgressInfo.mstrComPort).c_str());
		//! 异步发送命令
		init_command();
		async_write_port_time_out(boost::asio::buffer(msfirehose_all_command_info.sfirehose_info.mszCommand,msfirehose_all_command_info.sfirehose_info.mcommand_len),
			boost::bind(&download_asio_serial_port::write_download_config_xml_to_port,this,_1,_2,msfirehose_all_command_info.sfirehose_info.mcommand_len));

		return true;
	}

	void download_asio_serial_port::init_command()
	{
		msfirehose_all_command_info.sfirehose_info.mszCommand = msfirehose_all_command_info.mpVFirehose->at(msfirehose_all_command_info.mnXMLPos).mstrXMLConfig.c_str();
		msfirehose_all_command_info.sfirehose_info.mcommand_len = msfirehose_all_command_info.mpVFirehose->at(msfirehose_all_command_info.mnXMLPos).mstrXMLConfig.length();
		msfirehose_all_command_info.sfirehose_info.mpos = msfirehose_all_command_info.mnXMLPos;
		msfirehose_all_command_info.mnXMLPos ++;
	}

	bool download_asio_serial_port::is_error_download_response_command()
	{
		char szBuf[260] = {0};
		std::string str_error_msg = "";
		char* buf = strstr(mrx_buf,"<response");

		//! 如果未找到响应字符串 则直接返回true 表达没有错误
		while (buf)
		{
			JGW::CCJGW_DownloadConfig::GetProgramNodeTag(buf,"value",szBuf);
			if (NULL != JGW_StrCaseStr(szBuf,"NAK")) break;
			buf = strstr(buf + strlen("<response"),"<response");
		}
		if (!buf) return false;
		//! 查找<log 则返回NULL
		buf = strstr(mrx_buf,"<log");
		while (buf)
		{
			JGW::CCJGW_DownloadConfig::GetProgramNodeTag(buf,"value",szBuf);
			str_error_msg += szBuf;
			str_error_msg += ",";
			buf = strstr(buf + strlen("<log"),"<log");
		}
		dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,L"error: %s\n",JGW_A2W(str_error_msg).c_str());

		return true;
	}

	void download_asio_serial_port::write_download_config_xml_to_port(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len)
	{
		if (check_write_len_is_not_complete(tx_len_complete,tx_len)) return;
		msDownloadProgressInfo.mdw64SendCounts += tx_len_complete;
		memset(mrx_buf,0x00,MAX_STRING_SIZE);
		dbg_debug(LOG_DEBUG,msDownloadProgressInfo.mnIndex,L"write:%s\n",JGW_A2W_A(msfirehose_all_command_info.sfirehose_info.mszCommand).c_str());
		//! 如果写入命令包含program 则转入到read_download_program_file_to_port 否则转到read_download_config_xml_to_port
		async_read_port_time_out(boost::asio::buffer(mrx_buf,MAX_STRING_SIZE),boost::bind((NULL == JGW_StrCaseStr(msfirehose_all_command_info.sfirehose_info.mszCommand,"<program"))?&download_asio_serial_port::read_download_config_xml_to_port:&download_asio_serial_port::read_download_program_file_to_port,this,_1,_2));
	}

	void download_asio_serial_port::GetMaxPayloadSizeToTargetInBytes()
	{
		char szBuf[260] = {0};
		JGW::CCJGW_DownloadConfig::GetProgramNodeTag(mrx_buf,"MaxPayloadSizeToTargetInBytes",szBuf);

		int MaxPayloadSizeToTargetInBytes = atoi(szBuf);
		if (MaxPayloadSizeToTargetInBytes > FIREHOSE_TX_BUFFER_SIZE || MaxPayloadSizeToTargetInBytes < 64*1024) return;
		fh.attrs.MaxPayloadSizeFromTargetInBytes = MaxPayloadSizeToTargetInBytes;
		mbGetMaxPayloadSizeToTargetInBytes = true;
		dbg(LOG_WARN,msDownloadProgressInfo.mnIndex,L"MaxPayloadSizeFromTargetInBytes: %d\n",MaxPayloadSizeToTargetInBytes);
	}

	void download_asio_serial_port::send_download_error_msg(const wchar_t* msg)
	{
		close_serial_port();
		msDownloadProgressInfo.meDownloadStatus = E_ERROR_DOWNLOAD;
		dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,msg);
		dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,L"download time: %.2lf s \n",test_time.elapsed());
	}

	void download_asio_serial_port::send_download_ok_msg(const wchar_t* msg)
	{
		close_serial_port();
		msDownloadProgressInfo.meDownloadStatus = E_SUCCESSFUL_DOWNLOAD;
		dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,msg);
		dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,L"download time: %.2lf s \n",test_time.elapsed());
	}


	void download_asio_serial_port::read_download_config_xml_to_port(const boost::system::error_code& ec,size_t rx_len)
	{
		if (check_read_len_is_empty(rx_len)) return;
		mrx_buf[rx_len] = '\0';
		dbg_debug(LOG_DEBUG,msDownloadProgressInfo.mnIndex,L"read:%s\n",JGW_A2W_A(mrx_buf).c_str());

		//! 判断当前命令是否发送错误
		{
			if (is_error_download_response_command())
			{
				send_download_error_msg(L"response command is error status\n");
				return ;
			}
			//! 获取当前发送最大长度
			if (!mbGetMaxPayloadSizeToTargetInBytes && 0 == msfirehose_all_command_info.sfirehose_info.mpos)
				GetMaxPayloadSizeToTargetInBytes();
		}

		//! 所有XML config都发送完毕
		if (msfirehose_all_command_info.sfirehose_info.mpos >= msfirehose_all_command_info.mpVFirehose->size() - 1) 
		{
			send_download_ok_msg(L"Firehose protocol Download Successfully!!!\n");
			return;
		}  
		//! 继续下发
		{
			init_command();
			async_write_port_time_out(boost::asio::buffer(msfirehose_all_command_info.sfirehose_info.mszCommand,msfirehose_all_command_info.sfirehose_info.mcommand_len),
				boost::bind(&download_asio_serial_port::write_download_config_xml_to_port,this,_1,_2,msfirehose_all_command_info.sfirehose_info.mcommand_len));
		}
	}

	size_t download_asio_serial_port::check_tx_buf_bytes(size_t tx_len)
	{
		size_t tmp = tx_len % msfirehose_all_command_info.mpVFirehose->at(msfirehose_all_command_info.sfirehose_info.mpos).SectorSizeInBytes;
		//! 判断发送的长度是否等于最大的发送长度
		if (tx_len != fh.attrs.MaxPayloadSizeToTargetInBytes && 0 != tmp)
			//! 如果发送长度不能整除扇区长度，则填充为扇区长度
			tx_len = tx_len + msfirehose_all_command_info.mpVFirehose->at(msfirehose_all_command_info.sfirehose_info.mpos).SectorSizeInBytes - tmp;

		return tx_len;
	}

	void download_asio_serial_port::send_firehose_program_image_start()
	{
		rewrite_program_command = 0;
		boost::shared_ptr<FILE> fp(NULL,close_file);
		size_t bytesRead = msfirehose_all_command_info.mpVFirehose->at(msfirehose_all_command_info.sfirehose_info.mpos).mfile_buf_len;

		dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,L"start send program:%s\n",JGW_A2W(msfirehose_all_command_info.mpVFirehose->at(msfirehose_all_command_info.sfirehose_info.mpos).mstrProgramFilePath).c_str());
		//! 如果当前文件大小小于等于最大文件缓冲区大小
		if (bytesRead <= MAX_FILE_CACHE_BUFFER)
		{      
			async_write_port_time_out(boost::asio::buffer(msfirehose_all_command_info.mpVFirehose->at(msfirehose_all_command_info.sfirehose_info.mpos).mfile_buf,bytesRead),boost::bind
				(&download_asio_serial_port::write_download_program_file_to_port,this,_1,_2,bytesRead,fp));
		}
		else
		{ 
			//!  打开文件
			fp.reset(fopen (msfirehose_all_command_info.mpVFirehose->at(msfirehose_all_command_info.sfirehose_info.mpos).mstrProgramFilePath.c_str(), "rb"),close_file);
			if (!fp) 
			{
				dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,L"open fail(%s)\n",JGW_A2W(msfirehose_all_command_info.mpVFirehose->at(msfirehose_all_command_info.sfirehose_info.mpos).mstrProgramFilePath).c_str());
				send_download_error_msg(L"");
				return;
			}
			//! 读取文件内容
			memset(mtx_buf,0x00,FIREHOSE_TX_BUFFER_SIZE);
			bytesRead = fread (mtx_buf, sizeof (char), fh.attrs.MaxPayloadSizeToTargetInBytes, fp.get());
			//! 如果读取到的文件buf 不足fh.attrs.MaxPayloadSizeToTargetInBytes 则按照SectorSizeInBytes的倍数写入 后面写入数值置为‘\0’
			bytesRead = check_tx_buf_bytes(bytesRead);

			async_write_port_time_out(boost::asio::buffer(mtx_buf,bytesRead),boost::bind(&download_asio_serial_port::write_download_program_file_to_port,this,_1,_2,bytesRead,fp));
		}
	}

	bool download_asio_serial_port::check_firehose_device_is_rawmode()
	{
		char szBuf[260] = {0};
		char* prawmode = strstr(mrx_buf,"rawmode");
		while (prawmode)
		{
			JGW::CCJGW_DownloadConfig::GetProgramNodeTag(prawmode,"rawmode",szBuf);
			if (NULL != JGW::JGW_StrCaseStr(szBuf,"true")) msfirehose_all_command_info.mbRawMode = true;
			if (NULL != JGW::JGW_StrCaseStr(szBuf,"false")) msfirehose_all_command_info.mbRawMode = false;
			prawmode += strlen("rawmode");
			prawmode = strstr(prawmode,"rawmode");
		}

		return msfirehose_all_command_info.mbRawMode;
	}

	void download_asio_serial_port::read_download_program_file_to_port(const boost::system::error_code& ec,size_t rx_len)
	{
		if (check_read_len_is_empty(rx_len)) return;
		dbg_debug(LOG_DEBUG,msDownloadProgressInfo.mnIndex,L"read:%s\n",JGW_A2W_A(mrx_buf).c_str());

		if (is_error_download_response_command())
		{
			send_download_error_msg(L"response command is error status\n");
			return ;
		}
		//! 如果手机处于 rawmode 模式
		if (check_firehose_device_is_rawmode())
		{
			send_firehose_program_image_start();
		}
		else
		{
			dbg(LOG_WARN,msDownloadProgressInfo.mnIndex,L"write program command retry\n");
			//! 命令重发机制
			if ( rewrite_program_command < MAX_REWRITE_PROGRAM_COMMAND)
			{
				async_write_port_time_out(boost::asio::buffer(msfirehose_all_command_info.sfirehose_info.mszCommand,msfirehose_all_command_info.sfirehose_info.mcommand_len),
				boost::bind(&download_asio_serial_port::write_download_config_xml_to_port,this,_1,_2,msfirehose_all_command_info.sfirehose_info.mcommand_len));
			}
			else
			{
				send_download_error_msg(L"send program command time out\n");
			}
			rewrite_program_command ++;
		}  
	}

	void download_asio_serial_port::send_firehose_program_image_end(boost::shared_ptr<FILE> fp)
	{
		size_t bytesRead = 0;
		memset(mtx_buf,0x00,FIREHOSE_TX_BUFFER_SIZE);

		if (fp) bytesRead = fread (mtx_buf, sizeof (char), fh.attrs.MaxPayloadSizeToTargetInBytes, fp.get());
		//! 当文件读取完成时候 说明文件BUF已经全部发送OK 手机会发送一条当前program写入成功信息
		if (0 == bytesRead)
		{
			async_read_port_time_out(boost::asio::buffer(mrx_buf,MAX_STRING_SIZE),boost::bind(&download_asio_serial_port::read_download_config_xml_to_port,this,_1,_2));
			dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,L"send program image ok\n");
			return ;
		}
		//! 如果读取到的文件buf 不足fh.attrs.MaxPayloadSizeToTargetInBytes 则按照SectorSizeInBytes的倍数写入 后面写入数值置为‘\0’
		bytesRead = check_tx_buf_bytes(bytesRead);
		async_write_port_time_out(boost::asio::buffer(mtx_buf,bytesRead),boost::bind(&download_asio_serial_port::write_download_program_file_to_port,this,_1,_2,bytesRead,fp));
	}

	void download_asio_serial_port::write_download_program_file_to_port(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len,boost::shared_ptr<FILE> fp)
	{
		//! 当准备发送的长度与最终发送长度不一致时候
		if (check_write_len_is_not_complete(tx_len,tx_len_complete)) return;
		msDownloadProgressInfo.mdw64SendCounts += tx_len_complete;
		send_firehose_program_image_end(fp);
	}

	void download_asio_serial_port::close_serial_port()
	{
        boost::system::error_code ec;
		if (sp.is_open()) sp.close(ec);
        PRINT_ERRORCODE(ec,L"close_serial_port(%s)\n");
		deadline_.expires_at(boost::posix_time::pos_infin,ec);
        PRINT_ERRORCODE(ec,L"close_serial_port(%s)\n");
		dbg(LOG_WARN,msDownloadProgressInfo.mnIndex,L"close_serial_port\n");
	}

	void download_asio_serial_port::check_deadline(const boost::system::error_code e)  
	{
		if (e) return;
		//! 真正的等待超时
		if (deadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())  
		{
			send_download_error_msg(L"wait time out\n");	
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

	bool download_asio_serial_port::is_ack_successful (int status)
	{
		switch (status) {

			/*Success*/
		case SAHARA_STATUS_SUCCESS :
			dbg(LOG_INFO,msDownloadProgressInfo.mnIndex, L"SAHARA_STATUS_SUCCESS\n");
			return true;

			/*Invalid command received in current state*/
		case SAHARA_NAK_INVALID_CMD:
			dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex, L"SAHARA_NAK_INVALID_CMD\n");
			break;

			/*Protocol mismatch between host and target*/
		case SAHARA_NAK_PROTOCOL_MISMATCH:
			dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex, L"SAHARA_NAK_PROTOCOL_MISMATCH\n");
			break;

			/*Invalid target protocol version*/
		case SAHARA_NAK_INVALID_TARGET_PROTOCOL:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_INVALID_TARGET_PROTOCOL\n");
			break;

			/*Invalid host protocol version*/
		case SAHARA_NAK_INVALID_HOST_PROTOCOL:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_INVALID_HOST_PROTOCOL\n");
			break;

			/*Invalid packet size received*/
		case SAHARA_NAK_INVALID_PACKET_SIZE:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_INVALID_PACKET_SIZE\n");
			break;

			/*Unexpected image ID received*/
		case SAHARA_NAK_UNEXPECTED_IMAGE_ID:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_UNEXPECTED_IMAGE_ID\n");
			break;

			/*Invalid image header size received*/
		case SAHARA_NAK_INVALID_HEADER_SIZE:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_INVALID_HEADER_SIZE\n");
			break;

			/*Invalid image data size received*/
		case SAHARA_NAK_INVALID_DATA_SIZE:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_INVALID_DATA_SIZE\n");
			break;

			/*Unsupported image type received*/
		case SAHARA_NAK_INVALID_IMAGE_TYPE:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_INVALID_IMAGE_TYPE\n");
			break;

			/*Invalid tranmission length*/
		case SAHARA_NAK_INVALID_TX_LENGTH:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_INVALID_TX_LENGTH\n");
			break;

			/*Invalid reception length*/
		case SAHARA_NAK_INVALID_RX_LENGTH :
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_INVALID_RX_LENGTH\n");
			break;

			/*General transmission or reception error*/
		case  SAHARA_NAK_GENERAL_TX_RX_ERROR:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_GENERAL_TX_RX_ERROR\n");
			break;

			/*Error while transmitting READ_DATA packet*/
		case SAHARA_NAK_READ_DATA_ERROR:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_READ_DATA_ERROR\n");
			break;

			/*Cannot receive specified number of program headers*/
		case SAHARA_NAK_UNSUPPORTED_NUM_PHDRS:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_UNSUPPORTED_NUM_PHDRS\n");
			break;

			/*Invalid data length received for program headers*/
		case SAHARA_NAK_INVALID_PDHR_SIZE:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_INVALID_PDHR_SIZE\n");
			break;

			/*Multiple shared segments found in ELF image*/
		case SAHARA_NAK_MULTIPLE_SHARED_SEG:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_MULTIPLE_SHARED_SEG\n");
			break;

			/*Uninitialized program header location*/
		case SAHARA_NAK_UNINIT_PHDR_LOC:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_UNINIT_PHDR_LOC\n");
			break;

			/* Invalid destination address*/
		case  SAHARA_NAK_INVALID_DEST_ADDR:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_INVALID_DEST_ADDR\n");
			break;

			/* Invalid data size receieved in image header*/
		case SAHARA_NAK_INVALID_IMG_HDR_DATA_SIZE:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_INVALID_IMG_HDR_DATA_SIZE\n");
			break;

			/* Invalid ELF header received*/
		case SAHARA_NAK_INVALID_ELF_HDR:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_INVALID_ELF_HDR\n");
			break;

			/* Unknown host error received in HELLO_RESP*/
		case SAHARA_NAK_UNKNOWN_HOST_ERROR:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_UNKNOWN_HOST_ERROR\n");
			break;

			// Timeout while receiving data
		case SAHARA_NAK_TIMEOUT_RX:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_TIMEOUT_RX\n");
			break;

			// Timeout while transmitting data
		case SAHARA_NAK_TIMEOUT_TX:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_TIMEOUT_TX\n");
			break;

			// Invalid mode received from host
		case SAHARA_NAK_INVALID_HOST_MODE:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_INVALID_HOST_MODE\n");
			break;

			// Invalid memory read access
		case SAHARA_NAK_INVALID_MEMORY_READ:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_INVALID_MEMORY_READ\n");
			break;

			// Host cannot handle read data size requested
		case SAHARA_NAK_INVALID_DATA_SIZE_REQUEST:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_INVALID_DATA_SIZE_REQUEST\n");
			break;

			// Memory debug not supported
		case SAHARA_NAK_MEMORY_DEBUG_NOT_SUPPORTED:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_MEMORY_DEBUG_NOT_SUPPORTED\n");
			break;

			// Invalid mode switch
		case SAHARA_NAK_INVALID_MODE_SWITCH:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_INVALID_MODE_SWITCH\n");
			break;

			// Failed to execute command
		case SAHARA_NAK_CMD_EXEC_FAILURE:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_CMD_EXEC_FAILURE\n");
			break;

			// Invalid parameter passed to command execution
		case SAHARA_NAK_EXEC_CMD_INVALID_PARAM:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_EXEC_CMD_INVALID_PARAM\n");
			break;

			// Unsupported client command received
		case SAHARA_NAK_EXEC_CMD_UNSUPPORTED:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_EXEC_CMD_UNSUPPORTED\n");
			break;

			// Invalid client command received for data response
		case SAHARA_NAK_EXEC_DATA_INVALID_CLIENT_CMD:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_EXEC_DATA_INVALID_CLIENT_CMD\n");
			break;

			// Failed to authenticate hash table
		case SAHARA_NAK_HASH_TABLE_AUTH_FAILURE:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_HASH_TABLE_AUTH_FAILURE\n");
			break;

			// Failed to verify hash for a given segment of ELF image
		case SAHARA_NAK_HASH_VERIFICATION_FAILURE:
			dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex, L"SAHARA_NAK_HASH_VERIFICATION_FAILURE\n");
			break;

			// Failed to find hash table in ELF image
		case SAHARA_NAK_HASH_TABLE_NOT_FOUND:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_HASH_TABLE_NOT_FOUND\n");
			break;

		case SAHARA_NAK_LAST_CODE:
			dbg(LOG_ERROR, msDownloadProgressInfo.mnIndex,L"SAHARA_NAK_LAST_CODE\n");
			break;

		default:
			dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex, L"Invalid status field %d\n", status);
			break;
		}
		return false;
	}
}
