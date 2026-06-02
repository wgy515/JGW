#include "StdAfx.h"
#include "CJGW_DownloadPumaAsioSerialPort.h"

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

    CCJGW_DownloadPumaAsioSerialPort::CCJGW_DownloadPumaAsioSerialPort(boost::asio::io_service& ios,int nIndex):msp(ios),mdeadline_(ios),mpsPumaDownloadInfo(NULL),mnRecvIndex(0)
    {
        msDownloadProgressInfo.mnIndex = (nIndex);
        msDownloadProgressInfo.mdw64SendCounts = 0;
        msDownloadProgressInfo.meDownloadStatus = E_IDLE_DOWNLOAD;

        mRecvBuf = (char*)memalign (JGW_GetPageSize (), MAX_PUMA_RSP_BUF_SIZE);
        memset(mRecvBuf,0x00,MAX_PUMA_RSP_BUF_SIZE);

        SetPortOption();
    }


    CCJGW_DownloadPumaAsioSerialPort::~CCJGW_DownloadPumaAsioSerialPort(void)
    {
        CloseDownloadPumaAsioSerialPort();
        ALIGNED_FREE(mRecvBuf);
    }

    bool CCJGW_DownloadPumaAsioSerialPort::StartDownloadPumaImgae(PS_PUMA_DOWNLOAD_INFO psPumaDownloadInfo)
    {
        mpsPumaDownloadInfo = psPumaDownloadInfo;
        mnValidateImageDataIndex = 0;
        mnDownloadImageDataIndex = 0;
        msDownloadProgressInfo.mdw64SendCounts = 0;
        mTestTime.restart();
        msDownloadProgressInfo.meDownloadStatus = E_BUSY_DOWNLOAD;

        dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,L"Start Imgae Download\n");
#ifdef ASYNC_START_DOWNLOAD_DEFINE
		dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,L"Start Async Serial Port(%s)\n",JGW_A2W(msDownloadProgressInfo.mstrComPort).c_str());
		boost::system::error_code ec;
		mdeadline_.expires_from_now(boost::posix_time::seconds(1),ec);
		PRINT_ERRORCODE(ec,L"deadline_.expires_from_now (%s)\n");
		mdeadline_.async_wait(boost::bind(&CCJGW_DownloadPumaAsioSerialPort::AsyncStartDownload, this,_1));
#else
#if 1
        //! 打开下载端口
        if(!ReopenDownloadPumaAsioSerialPort())
        {
            SendDownloadErrorMsg("Open Serial Port Fail\n");
            return false;
        }
        dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"Open Serial Port(%s) Ok\n",msDownloadProgressInfo.mstrComPort.c_str());

        meDownloadStep = E_SPI_Enable;
        async_write_port_time_out("\r",1,
            boost::bind(
            &CCJGW_DownloadPumaAsioSerialPort::WriteDownloadCmdToPortCompleted,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred,1));
#else
        Sleep(2*1000);
        SendDownloadOkMsg("Debug Mode");
#endif
#endif
        return true;
    }

	void CCJGW_DownloadPumaAsioSerialPort::AsyncStartDownload(const boost::system::error_code e)
	{
		//! 打开下载端口
		int i = 0;
		for (;i < 10;i ++)
		{
			if(ReopenDownloadPumaAsioSerialPort()) break;
			dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,L"Open Serial Port(%s) Fail\n",JGW_A2W(msDownloadProgressInfo.mstrComPort).c_str());
			Sleep(1000);
		}

		if (i >= 10)
		{
			SendDownloadErrorMsg(L"Open Serial Port Fail\n");
			return ;
		}

		dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,L"Open Serial Port(%s) Ok\n",JGW_A2W(msDownloadProgressInfo.mstrComPort).c_str());

		meDownloadStep = E_SPI_Enable;
		async_write_port_time_out("\r",1,
			boost::bind(
			&CCJGW_DownloadPumaAsioSerialPort::WriteDownloadCmdToPortCompleted,
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred,1));
	}

    void CCJGW_DownloadPumaAsioSerialPort::WriteDownloadCmdToPortCompleted(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len)
    {
        msDownloadProgressInfo.mdw64SendCounts += tx_len;
        if (CheckWriteLenIsNotComplete(tx_len,tx_len_complete)) return;

        async_read_port_time_out(boost::bind(
            &CCJGW_DownloadPumaAsioSerialPort::ReadRspDownCmdCompleted,
            this,_1,_2));
    }

    void CCJGW_DownloadPumaAsioSerialPort::ReadRspDownCmdCompleted(const boost::system::error_code& ec,size_t rx_len)
    {
        if (CheckReadLenIsEmptyAndIsRespTerminator(rx_len)) return;
        
        switch (meDownloadStep)
        {
        case E_SPI_Enable:
            meDownloadStep = E_SPI_SetBaud;
            async_write_port_time_out(spiEnable,spiEnableLen,
                boost::bind(
                &CCJGW_DownloadPumaAsioSerialPort::WriteDownloadCmdToPortCompleted,
                this,_1,_2,spiEnableLen));
            break;
        case E_SPI_SetBaud:
            meDownloadStep = E_FLASH_Identity;
            async_write_port_time_out(spiSetBaud,spiSetBaudLen,
                boost::bind(
                &CCJGW_DownloadPumaAsioSerialPort::WriteDownloadCmdToPortCompleted,
                this,_1,_2,spiSetBaudLen)
                );
            break;
        case E_FLASH_Identity:
            meDownloadStep = mpsPumaDownloadInfo->mbFormatPDBImage?E_ERASE_FLASH:E_CHECK_FLASH_BLANK;
            async_write_port_time_out(flashIdentity,flashIdentityLen,
                boost::bind(
                &CCJGW_DownloadPumaAsioSerialPort::WriteDownloadCmdToPortCompleted,
                this,_1,_2,flashIdentityLen)
                );
            break;
        case E_CHECK_FLASH_BLANK: //! 检查FLASH是否为空白
            dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,L"Check Flash Is Blank\n");
            if (mpsPumaDownloadInfo->mbFormatPDBImage)
            {
                mnCheckBlankLength = (PERASO_DEVICE_SIZE+(FLASH_PAGE_SIZE-1)) & (~(FLASH_PAGE_SIZE-1)); //! 0xff
            }
            else
            {
                mnCheckBlankLength = (mpsPumaDownloadInfo->mnImageFileSize+(FLASH_PAGE_SIZE-1)) & (~(FLASH_PAGE_SIZE-1)); //! 0xff
            }
            
            a_address = 0;
            a_address &= 0xff0000;
            mread_length = (mnCheckBlankLength > FLASH_READ_SIZE) ? FLASH_READ_SIZE : mnCheckBlankLength;
            GenReadFlashDataCmd(mread_length);
            async_write_port_time_out(cmd,strlen(cmd),
                boost::bind(
                &CCJGW_DownloadPumaAsioSerialPort::WriteReadFlashDataCmdToPortCompleted,
                this,_1,_2,strlen(cmd))
                );
            break;
        case E_ERASE_FLASH:
            mnEraseImageDataIndex = 0;
            dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,L"Start Erease Flash\n");
            async_write_port_time_out(mpsPumaDownloadInfo->mvPumaEraseImageDataInfo->at(0).mpFmtBuffer,mpsPumaDownloadInfo->mvPumaEraseImageDataInfo->at(0).mFmtBufferSize,
                boost::bind(
                &CCJGW_DownloadPumaAsioSerialPort::WriteEraseFlashDataCmdToPortCompleted,
                this,_1,_2,mpsPumaDownloadInfo->mvPumaEraseImageDataInfo->at(0).mFmtBufferSize)
                );
            break;
        case E_Donwload_ImageDataInfo:
            if (CheckDownloadImageIsCompleted()) return;
#ifdef FLASH_ENABLE_ARRAY
            DownloadImageDataInfo();
#else
            async_write_port_time_out(flashEnable,flashEnableLen,
                boost::bind(
                &CCJGW_DownloadPumaAsioSerialPort::WriteFlashEnableCmdCompleted,
                this,_1,_2,flashEnableLen)
                );
#endif
            break;
        case E_Validate_DownloadDataInfo:
            ValidateReadImageDataInfo();
            break;
        }
    }
#ifdef FLASH_ENABLE_ARRAY
    void CCJGW_DownloadPumaAsioSerialPort::DownloadImageDataInfo()
    {
        async_write_port_time_out(
            mpsPumaImageDataInfo->mpFmtBuffer,
            mpsPumaImageDataInfo->mFmtBufferSize,
            boost::bind(
            &CCJGW_DownloadPumaAsioSerialPort::WriteDownloadCmdToPortCompleted,
            this,_1,_2,
            mpsPumaImageDataInfo->mFmtBufferSize)
            );
        mnDownloadImageDataIndex ++;
        mpsPumaImageDataInfo = &mpsPumaDownloadInfo->mpPumaImageDataInfo->at(mnDownloadImageDataIndex);
    }
#else
    void CCJGW_DownloadPumaAsioSerialPort::WriteFlashEnableCmdCompleted(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len)
    {
        msDownloadProgressInfo.mdw64SendCounts += tx_len;
        if (CheckWriteLenIsNotComplete(tx_len,tx_len_complete)) return;

        async_read_port_time_out(boost::bind(
            &CCJGW_DownloadPumaAsioSerialPort::DownloadImageDataInfo,
            this,_1,_2));
    }

    void CCJGW_DownloadPumaAsioSerialPort::DownloadImageDataInfo(const boost::system::error_code& ec,size_t rx_len)
    {
        mnRecvIndex += rx_len;
        msDownloadProgressInfo.mdw64SendCounts += rx_len;
        if (0 == rx_len || RESP_TERMINATOR != mRecvBuf[mnRecvIndex - 1])
        {
            async_read_port_time_out(boost::bind(
                &CCJGW_DownloadPumaAsioSerialPort::DownloadImageDataInfo,
                this,_1,_2));
            return ;
        }
        mRecvBuf[mnRecvIndex] = '\0';
        dbg_debug(LOG_DEBUG,msDownloadProgressInfo.mnIndex,JGW_A2W_A(mRecvBuf));
        mpsPumaImageDataInfo = &mpsPumaDownloadInfo->mpPumaImageDataInfo->at(mnDownloadImageDataIndex++);
        async_write_port_time_out(
            mpsPumaImageDataInfo->mpFmtBuffer,
            mpsPumaImageDataInfo->mFmtBufferSize,
            boost::bind(
            &CCJGW_DownloadPumaAsioSerialPort::WriteDownloadCmdToPortCompleted,
            this,_1,_2,
            mpsPumaImageDataInfo->mFmtBufferSize)
            );
    }
#endif

    bool CCJGW_DownloadPumaAsioSerialPort::CheckDownloadImageIsCompleted()
    {
        if (mnDownloadImageDataIndex < mpsPumaDownloadInfo->mpPumaImageDataInfo->size()) return false;

        dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,L"download image file completed download time: %.2lf s \n",mTestTime.elapsed());
        dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,L"start validate image file...\n");

        if (mpsPumaDownloadInfo->mbValidateImageDataInfo)
        {
            mTestTime.restart();
            meDownloadStep = E_Validate_DownloadDataInfo;  
            ValidateReadImageDataInfo();  
        }
        else
        {
            SendDownloadOkMsg(L"Download Image Complete\n");
        }
        return true;
    }

    //! 正在写
    void CCJGW_DownloadPumaAsioSerialPort::ValidateReadImageDataInfo()
    {
        if (mnValidateImageDataIndex >= mpsPumaDownloadInfo->mpPumaValidateImageDataInfo->size())
        {
            SendDownloadOkMsg(L"Download Validate Image Complete\n");
            return ;
        }
        mpsPumaImageDataInfo = &mpsPumaDownloadInfo->mpPumaValidateImageDataInfo->at(mnValidateImageDataIndex++);
        async_write_port_time_out( mpsPumaImageDataInfo->mpDownloadCmd,
            strlen(mpsPumaImageDataInfo->mpDownloadCmd),
            boost::bind(
            &CCJGW_DownloadPumaAsioSerialPort::ValidateReadImageDataInfo,
            this,_1,_2,strlen(mpsPumaImageDataInfo->mpDownloadCmd))
            );
    }
    //! 写完成
    void CCJGW_DownloadPumaAsioSerialPort::ValidateReadImageDataInfo(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len)
    {
        msDownloadProgressInfo.mdw64SendCounts += tx_len;
        if (CheckWriteLenIsNotComplete(tx_len,tx_len_complete)) return;

        async_read_port_time_out(boost::bind(
            &CCJGW_DownloadPumaAsioSerialPort::ValidateReadImageDataInfo,
            this,_1,_2));
    }
    //! 读完成
    void CCJGW_DownloadPumaAsioSerialPort::ValidateReadImageDataInfo(const boost::system::error_code& ec,size_t rx_len)
    {
        mnRecvIndex += rx_len;
        msDownloadProgressInfo.mdw64SendCounts += rx_len;
        if (0 == rx_len || RESP_TERMINATOR != mRecvBuf[mnRecvIndex - 1])
        {
            async_read_port_time_out(boost::bind(
                &CCJGW_DownloadPumaAsioSerialPort::ValidateReadImageDataInfo,
                this,_1,_2));
            return ;
        }
        //! 验证镜像数据信息
        if (!CheckValidateImageDataInfo()) return ;
        mRecvBuf[mnRecvIndex] = '\0';
        dbg_debug(LOG_DEBUG,msDownloadProgressInfo.mnIndex,JGW_A2W_A(mRecvBuf).c_str()); 
        ValidateReadImageDataInfo();
    }

    bool CCJGW_DownloadPumaAsioSerialPort::CheckValidateImageDataInfo()
    {
        if (mnRecvIndex != mpsPumaImageDataInfo->mFmtBufferSize)
        {
            SendDownloadErrorMsg(L"Validate Image Data Fail\n");
            return false;
        }

        if (0 != memcmp(mpsPumaImageDataInfo->mpFmtBuffer,mRecvBuf,mnRecvIndex))
        {
            SendDownloadErrorMsg(L"Validate Image Data Fail\n");
            return false;
        }
        return true;
    }

    bool CCJGW_DownloadPumaAsioSerialPort::CheckReadLenIsEmptyAndIsRespTerminator(size_t read_len)
    {
        mnRecvIndex += read_len;
        msDownloadProgressInfo.mdw64SendCounts += read_len;
        if (read_len > 0 && RESP_TERMINATOR == mRecvBuf[mnRecvIndex - 1])
        {
            mRecvBuf[mnRecvIndex] = '\0';
            dbg_debug(LOG_DEBUG,msDownloadProgressInfo.mnIndex,JGW_A2W_A(mRecvBuf).c_str());
            return false;
        }
        async_read_port_time_out(boost::bind(
            &CCJGW_DownloadPumaAsioSerialPort::ReadRspDownCmdCompleted,
            this,_1,_2));
        return true;
    }

    bool CCJGW_DownloadPumaAsioSerialPort::CheckReadFlashDataLenIsEmptyAndIsRespTerminator(size_t read_len)
    {
        mnRecvIndex += read_len;
        msDownloadProgressInfo.mdw64SendCounts += read_len;
        if (read_len > 0 && RESP_TERMINATOR == mRecvBuf[mnRecvIndex - 1])
        {
            mRecvBuf[mnRecvIndex] = '\0';
            dbg_debug(LOG_DEBUG,msDownloadProgressInfo.mnIndex,JGW_A2W_A(mRecvBuf).c_str());
            return false;
        }
        async_read_port_time_out(boost::bind(
            &CCJGW_DownloadPumaAsioSerialPort::ReadFlashDataCmdToPortCompleted,
            this,_1,_2));
        return true;
    }


    void CCJGW_DownloadPumaAsioSerialPort::GenReadFlashDataCmd(unsigned int a_length)
    {
        int i, index;
        int nbrOfLine = (a_length/16) + ((a_length % 16) ? 1 : 0);
        int rcvLength = (nbrOfLine * 2) + ((a_length * 3) - nbrOfLine) + 3;

        memcpy(cmd, scSpiReadCmd, sizeof(scSpiReadCmd));    // spi command for read 
        index = sizeof(scSpiReadCmd) - 1;
        cmd[index++] = '4';     // count of read command byte to follow
        cmd[index++] = ' ';
        cmd[index++] = OPCODE_READ_DATA;
        cmd[index++] = ' ';

        // calculate and format the count of SPI data
        for (i=3; i > 0; i--)
        {
            cmd[index++] = scTblHex2Ascii[0x0f & (a_address >> ((i-1)*8 + 4))]; 
            cmd[index++] = scTblHex2Ascii[0x0f & (a_address >> ((i-1)*8))]; 
            cmd[index++] = 0x20;
        }

        cmd[index++] = scTblHex2Ascii[0x0f & (a_length >> 12)]; 
        cmd[index++] = scTblHex2Ascii[0x0f & (a_length >> 8)]; 
        cmd[index++] = scTblHex2Ascii[0x0f & (a_length >> 4)]; 
        cmd[index++] = scTblHex2Ascii[0x0f & (a_length)]; 
        cmd[index++] = '\r';
        cmd[index++] = 0;
    }

    void CCJGW_DownloadPumaAsioSerialPort::WriteReadFlashDataCmdToPortCompleted(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len)
    {
        msDownloadProgressInfo.mdw64SendCounts += tx_len;
        if (CheckWriteLenIsNotComplete(tx_len,tx_len_complete)) return;

        async_read_port_time_out(boost::bind(
            &CCJGW_DownloadPumaAsioSerialPort::ReadFlashDataCmdToPortCompleted,
            this,_1,_2));
    }

    int unformatReceiveBuffer(char *a_pBuf, char *a_pData, int a_length)
    {
        int i = 0;
        unsigned char data, byteHi, byteLo;
        unsigned char * pData;

        // The loops repacks ASCII data string from Falcon-B bootloader into hex string
        // The data returns from Falcon-B bootloader has a specific pattern:
        // [\n\r,[byte1,byte2,space]x15,[byte1,byte2]]x N_lines, ends with \n\r>
        while (a_length)
        {
            int length = a_length > 16 ? 16 : a_length;
            pData = (unsigned char *)(a_pData + 2);
            a_pData += (2 + (length * 3)-1);     // move buffer head to next line
            a_length -= length;

            // pack 2 ASCII data to 1 hex value
            for (i = 1; i <= length; i++)
            {
                data = *pData++;
                byteHi = (data < 'A') ? (data - '0') : (data - 'A' + 0xa);
                data = *pData++;
                byteLo = (data < 'A') ? (data - '0') : (data - 'A' + 0xa);        
                *a_pBuf++ = (0xf0 & (byteHi << 4)) | (0x0f & (byteLo));
                pData++;  // skip space
            }
        }
        return i;
    }

    void CCJGW_DownloadPumaAsioSerialPort::ReadFlashDataCmdToPortCompleted(const boost::system::error_code& ec,size_t rx_len)
    {
        if (CheckReadFlashDataLenIsEmptyAndIsRespTerminator(rx_len)) return;
        unformatReceiveBuffer(mRecvBuf,mRecvBuf,mread_length);
        meDownloadStep = E_Donwload_ImageDataInfo;
        unsigned char blankChar = 0xff;
        char* a_pData = (char*)&blankChar;

        for (size_t i = 0; i < mread_length; i++)
        {
            if (mRecvBuf[i] != a_pData[0])
            { 
                meDownloadStep = E_ERASE_FLASH;
                break;
            }
        }

        mRecvBuf[0] = RESP_TERMINATOR;
        mnRecvIndex = 0;
        ReadRspDownCmdCompleted(ec,1);
    }

    void CCJGW_DownloadPumaAsioSerialPort::ReadEraseFlashDataCmdToPortCompleted(const boost::system::error_code& ec,size_t rx_len)
    {
        if (CheckReadEraseFlashDataLenIsEmptyAndIsRespTerminator(rx_len)) return;
        mnEraseImageDataIndex ++;

        if (mpsPumaDownloadInfo->mvPumaEraseImageDataInfo->size() <= mnEraseImageDataIndex)
        {
            meDownloadStep = E_Donwload_ImageDataInfo;
            dbg(LOG_WARN,msDownloadProgressInfo.mnIndex,L"Flash Erase the success\n");
            mRecvBuf[0] = RESP_TERMINATOR;
            mnRecvIndex = 0;
            ReadRspDownCmdCompleted(ec,1);
        }
        else
        {
            async_write_port_time_out(mpsPumaDownloadInfo->mvPumaEraseImageDataInfo->at(mnEraseImageDataIndex).mpFmtBuffer,mpsPumaDownloadInfo->mvPumaEraseImageDataInfo->at(mnEraseImageDataIndex).mFmtBufferSize,
                boost::bind(
                &CCJGW_DownloadPumaAsioSerialPort::WriteEraseFlashDataCmdToPortCompleted,
                this,_1,_2,mpsPumaDownloadInfo->mvPumaEraseImageDataInfo->at(mnEraseImageDataIndex).mFmtBufferSize)
                );
        }
    }

    bool CCJGW_DownloadPumaAsioSerialPort::CheckReadEraseFlashDataLenIsEmptyAndIsRespTerminator(size_t read_len)
    {
        mnRecvIndex += read_len;
        msDownloadProgressInfo.mdw64SendCounts += read_len;
        if (read_len > 0 && RESP_TERMINATOR == mRecvBuf[mnRecvIndex - 1])
        {
            mRecvBuf[mnRecvIndex] = '\0';
            dbg_debug(LOG_DEBUG,msDownloadProgressInfo.mnIndex,mRecvBuf);
            return false;
        }
        async_read_port_time_out(boost::bind(
            &CCJGW_DownloadPumaAsioSerialPort::ReadEraseFlashDataCmdToPortCompleted,
            this,_1,_2));
        return true;
    }


    void CCJGW_DownloadPumaAsioSerialPort::WriteEraseFlashDataCmdToPortCompleted(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len)
    {
        msDownloadProgressInfo.mdw64SendCounts += tx_len;
        if (CheckWriteLenIsNotComplete(tx_len,tx_len_complete)) return;
        async_read_port_time_out(boost::bind(
            &CCJGW_DownloadPumaAsioSerialPort::ReadEraseFlashDataCmdToPortCompleted,
            this,_1,_2));
    }

    bool CCJGW_DownloadPumaAsioSerialPort::CheckWriteLenIsNotComplete(size_t predict_length,size_t complete_length)
    {
        if (predict_length == complete_length) return false;
        SendDownloadErrorMsg(L"port write buf predict_length != complete_length\n");
        return true;
    }

    void CCJGW_DownloadPumaAsioSerialPort::SetPortOption(int nbaud_rate /* = 115200 */, int ncharacter_size /* = 8 */, boost::asio::serial_port::flow_control::type eflow_control /* = boost::asio::serial_port::flow_control::none */, boost::asio::serial_port::stop_bits::type estop_bits /* = boost::asio::serial_port::stop_bits::one */)
    {
        meflow_control = eflow_control;
        mestop_bits = estop_bits;
        mnbaud_rate = nbaud_rate;
        mncharacter_size = ncharacter_size;
    }

    bool CCJGW_DownloadPumaAsioSerialPort::ReopenDownloadPumaAsioSerialPort()
    {
        boost::system::error_code ec;
        if (msp.is_open()) msp.close(ec);
        msp.open(msDownloadProgressInfo.mstrComPort.c_str(),ec);
        CHECK_ERRORCODE_RETURN(ec,L"open port fail(%s)\n",false);
        msp.set_option(boost::asio::serial_port::baud_rate(mnbaud_rate));
        msp.set_option(boost::asio::serial_port::character_size(mncharacter_size));
        msp.set_option(boost::asio::serial_port::flow_control(meflow_control));
        msp.set_option(boost::asio::serial_port::stop_bits(mestop_bits));

        return msp.is_open();
    }

    void CCJGW_DownloadPumaAsioSerialPort::CloseDownloadPumaAsioSerialPort()
    {
        boost::system::error_code ec;
        if (msp.is_open()) msp.close(ec);
        PRINT_ERRORCODE(ec,L"close_serial_port(%s)\n");
        mdeadline_.expires_at(boost::posix_time::pos_infin,ec);
        PRINT_ERRORCODE(ec,L"close_serial_port(%s)\n");
        dbg(LOG_WARN,msDownloadProgressInfo.mnIndex,L"close_serial_port\n");
    }

    void CCJGW_DownloadPumaAsioSerialPort::CheckRWDeadline(const boost::system::error_code e)
    {
        if (e) return;
        //! 真正的等待超时
        if (mdeadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())  
        {
            SendDownloadErrorMsg(L"wait time out\n");	
            return;
        }  
    }

    void CCJGW_DownloadPumaAsioSerialPort::SendDownloadErrorMsg(const wchar_t* msg,E_DOWNLOAD_STATUS eDownloadStatus /* = E_ERROR_DOWNLOAD */)
    {
        SendDownloadOkMsg(msg,E_ERROR_DOWNLOAD);
    }

    void CCJGW_DownloadPumaAsioSerialPort::SendDownloadOkMsg(const wchar_t* msg,E_DOWNLOAD_STATUS eDownloadStatus /* = E_SUCCESSFUL_DOWNLOAD */)
    {
        CloseDownloadPumaAsioSerialPort();
        msDownloadProgressInfo.meDownloadStatus = eDownloadStatus;
        dbg(E_ERROR_DOWNLOAD != eDownloadStatus?LOG_INFO:LOG_ERROR,msDownloadProgressInfo.mnIndex,msg);
        dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,L"download time: %.2lf s \n",mTestTime.elapsed());
    }
}