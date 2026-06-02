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

        dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"Start Imgae Download\n");
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

        return true;
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
            meDownloadStep = E_Donwload_ImageDataInfo;
            async_write_port_time_out(flashIdentity,flashIdentityLen,
                boost::bind(
                &CCJGW_DownloadPumaAsioSerialPort::WriteDownloadCmdToPortCompleted,
                this,_1,_2,flashIdentityLen)
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
        if (0 == rx_len || RESP_TERMINATOR != mRecvBuf[mnRecvIndex - 1])
        {
            async_read_port_time_out(boost::bind(
                &CCJGW_DownloadPumaAsioSerialPort::DownloadImageDataInfo,
                this,_1,_2));
            return ;
        }
        mRecvBuf[mnRecvIndex] = '\0';
        dbg_debug(LOG_DEBUG,msDownloadProgressInfo.mnIndex,mRecvBuf);
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

        dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"download image file completed download time: %.2lf s \n",mTestTime.elapsed());
        dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"start validate image file...\n");

        if (mpsPumaDownloadInfo->mbValidateImageDataInfo)
        {
            mTestTime.restart();
            meDownloadStep = E_Validate_DownloadDataInfo;  
            ValidateReadImageDataInfo();  
        }
        else
        {
            SendDownloadOkMsg("Download Image Complete\n");
        }
        return true;
    }

    //! 正在写
    void CCJGW_DownloadPumaAsioSerialPort::ValidateReadImageDataInfo()
    {
        if (mnValidateImageDataIndex >= mpsPumaDownloadInfo->mpPumaValidateImageDataInfo->size())
        {
            SendDownloadOkMsg("Download Validate Image Complete\n");
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
        dbg_debug(LOG_DEBUG,msDownloadProgressInfo.mnIndex,mRecvBuf); 
        ValidateReadImageDataInfo();
    }

    bool CCJGW_DownloadPumaAsioSerialPort::CheckValidateImageDataInfo()
    {
        if (mnRecvIndex != mpsPumaImageDataInfo->mFmtBufferSize)
        {
            SendDownloadErrorMsg("Validate Image Data Fail\n");
            return false;
        }

        if (0 != memcmp(mpsPumaImageDataInfo->mpFmtBuffer,mRecvBuf,mnRecvIndex))
        {
            SendDownloadErrorMsg("Validate Image Data Fail\n");
            return false;
        }
        return true;
    }

    bool CCJGW_DownloadPumaAsioSerialPort::CheckReadLenIsEmptyAndIsRespTerminator(size_t read_len)
    {
        mnRecvIndex += read_len;
        if (read_len > 0 && RESP_TERMINATOR == mRecvBuf[mnRecvIndex - 1])
        {
            mRecvBuf[mnRecvIndex] = '\0';
            dbg_debug(LOG_DEBUG,msDownloadProgressInfo.mnIndex,mRecvBuf);
            return false;
        }
        async_read_port_time_out(boost::bind(
            &CCJGW_DownloadPumaAsioSerialPort::ReadRspDownCmdCompleted,
            this,_1,_2));
        return true;
    }

    bool CCJGW_DownloadPumaAsioSerialPort::CheckWriteLenIsNotComplete(size_t predict_length,size_t complete_length)
    {
        if (predict_length == complete_length) return false;
        SendDownloadErrorMsg("port write buf predict_length != complete_length\n");
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
        CHECK_ERRORCODE_RETURN(ec,"open port fail(%s)\n",false);
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
        PRINT_ERRORCODE(ec,"close_serial_port(%s)\n");
        mdeadline_.expires_at(boost::posix_time::pos_infin,ec);
        PRINT_ERRORCODE(ec,"close_serial_port(%s)\n");
        dbg(LOG_WARN,msDownloadProgressInfo.mnIndex,"close_serial_port\n");
    }

    void CCJGW_DownloadPumaAsioSerialPort::CheckRWDeadline(const boost::system::error_code e)
    {
        if (e) return;
        //! 真正的等待超时
        if (mdeadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())  
        {
            SendDownloadErrorMsg("wait time out\n");	
            return;
        }  
    }

    void CCJGW_DownloadPumaAsioSerialPort::SendDownloadErrorMsg(const char* msg,E_DOWNLOAD_STATUS eDownloadStatus /* = E_ERROR_DOWNLOAD */)
    {
        SendDownloadOkMsg(msg,E_ERROR_DOWNLOAD);
    }

    void CCJGW_DownloadPumaAsioSerialPort::SendDownloadOkMsg(const char* msg,E_DOWNLOAD_STATUS eDownloadStatus /* = E_SUCCESSFUL_DOWNLOAD */)
    {
        CloseDownloadPumaAsioSerialPort();
        msDownloadProgressInfo.meDownloadStatus = eDownloadStatus;
        dbg(E_ERROR_DOWNLOAD != eDownloadStatus?LOG_INFO:LOG_ERROR,msDownloadProgressInfo.mnIndex,msg);
        dbg(LOG_INFO,msDownloadProgressInfo.mnIndex,"download time: %.2lf s \n",mTestTime.elapsed());
    }
}