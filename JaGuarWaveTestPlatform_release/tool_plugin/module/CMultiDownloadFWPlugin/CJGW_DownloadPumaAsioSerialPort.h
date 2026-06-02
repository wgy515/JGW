#pragma once
#include <boost/progress.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "PumaDownload_Define.h"

#define ASYNC_START_DOWNLOAD_DEFINE
namespace JGW
{
    class CCJGW_DownloadPumaAsioSerialPort
    {
    public:
        CCJGW_DownloadPumaAsioSerialPort(boost::asio::io_service& ios,int nIndex);
        ~CCJGW_DownloadPumaAsioSerialPort(void);
    public:
        bool StartDownloadPumaImgae(PS_PUMA_DOWNLOAD_INFO psPumaDownloadInfo);

        void SetPortOption(int nbaud_rate = 115200,
            int ncharacter_size = 8,
            boost::asio::serial_port::flow_control::type eflow_control = boost::asio::serial_port::flow_control::none,
            boost::asio::serial_port::stop_bits::type estop_bits = boost::asio::serial_port::stop_bits::one);

        S_DOWNLOAD_PROGRESS_INFO& GetDownladProgressInfo() { return msDownloadProgressInfo;}

    private:
        //! 异步开始下载
		void AsyncStartDownload(const boost::system::error_code e);
#ifdef FLASH_ENABLE_ARRAY
        void DownloadImageDataInfo();
#else
        void WriteFlashEnableCmdCompleted(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len);
        void DownloadImageDataInfo(const boost::system::error_code& ec,size_t rx_len);
#endif
        //! 正在写
        void ValidateReadImageDataInfo();

        bool CheckDownloadImageIsCompleted();

        void ValidateReadImageDataInfo(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len);   

        void ValidateReadImageDataInfo(const boost::system::error_code& ec,size_t rx_len);

        bool CheckValidateImageDataInfo();
        //! 检查是否到响应文件尾
        //! void CheckIsRespTerminator(size_t read_len);
        //! 检查读取的长度是否为空
        bool CheckReadLenIsEmptyAndIsRespTerminator(size_t read_len);
/*        void read_sahara_download_to_port(const boost::system::error_code& ec,size_t rx_len);*/
        //! 检查预计发送的长度是否与实际长度一致 如果不一致则返回 true;
        bool CheckWriteLenIsNotComplete(size_t predict_length,size_t complete_length);
        //! 读完成
        void ReadRspDownCmdCompleted(const boost::system::error_code& ec,size_t rx_len);
        //! 写完成
        void WriteDownloadCmdToPortCompleted(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len);
        //! 写超时
#if 0
        template <typename ConstBufferSequence, typename WriteHandler>
        void async_write_port_time_out(const ConstBufferSequence& buffers,
            BOOST_ASIO_MOVE_ARG(WriteHandler) handler)
        {
            boost::system::error_code ec;
            mdeadline_.expires_from_now(boost::posix_time::seconds(TX_TIMEOUT_SECOND),ec);
            PRINT_ERRORCODE(ec,"deadline_.expires_from_now (%s)\n");
            mdeadline_.async_wait(boost::bind(&CCJGW_DownloadPumaAsioSerialPort::CheckRWDeadline, this,_1));
            mnRecvIndex = 0;
            msp.async_write_some(buffers,handler);
        }

        //! 读超时
        template <typename ConstBufferSequence, typename WriteHandler>
        void async_read_port_time_out(const ConstBufferSequence& buffers,
            BOOST_ASIO_MOVE_ARG(WriteHandler) handler)
        {
            boost::system::error_code ec;
            mdeadline_.expires_from_now(boost::posix_time::seconds(RX_TIMEOUT_SECOND),ec);
            PRINT_ERRORCODE(ec,"deadline_.expires_from_now (%s)\n");
            mdeadline_.async_wait(boost::bind(&CCJGW_DownloadPumaAsioSerialPort::CheckRWDeadline, this,_1));     
            msp.async_read_some(buffers,handler);
        }
#else
        template <typename WriteHandler>
        void async_write_port_time_out(const char* strCmd,size_t len,
            BOOST_ASIO_MOVE_ARG(WriteHandler) handler)
        {
            boost::system::error_code ec;
            mdeadline_.expires_from_now(boost::posix_time::seconds(TX_TIMEOUT_SECOND),ec);
            PRINT_ERRORCODE(ec,L"deadline_.expires_from_now (%s)\n");
            mdeadline_.async_wait(boost::bind(&CCJGW_DownloadPumaAsioSerialPort::CheckRWDeadline, this,_1));
            mnRecvIndex = 0;
            dbg_debug(LOG_DEBUG,msDownloadProgressInfo.mnIndex,JGW_A2W_A(strCmd).c_str());
            msp.async_write_some(boost::asio::buffer(strCmd,len),handler);
        }

        //! 读超时
        template <typename WriteHandler>
        void async_read_port_time_out(BOOST_ASIO_MOVE_ARG(WriteHandler) handler)
        {
            boost::system::error_code ec;
            mdeadline_.expires_from_now(boost::posix_time::seconds(RX_TIMEOUT_SECOND),ec);
            PRINT_ERRORCODE(ec,L"deadline_.expires_from_now (%s)\n");
            mdeadline_.async_wait(boost::bind(&CCJGW_DownloadPumaAsioSerialPort::CheckRWDeadline, this,_1));     
            msp.async_read_some(boost::asio::buffer(mRecvBuf + mnRecvIndex,1),handler);
        }
#endif
    private:
        void GenReadFlashDataCmd(unsigned int a_length);
        bool CheckReadFlashDataLenIsEmptyAndIsRespTerminator(size_t read_len);
        void WriteReadFlashDataCmdToPortCompleted(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len);
        void WriteEraseFlashDataCmdToPortCompleted(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len);
        void ReadFlashDataCmdToPortCompleted(const boost::system::error_code& ec,size_t rx_len);
        void ReadEraseFlashDataCmdToPortCompleted(const boost::system::error_code& ec,size_t rx_len);
        bool CheckReadEraseFlashDataLenIsEmptyAndIsRespTerminator(size_t read_len);
        /*void GenReadFlashDataCmd(unsigned int a_length);*/
    private:
        //! 打开puma下载端口
        bool ReopenDownloadPumaAsioSerialPort();
        //! 关闭下载端口
        void CloseDownloadPumaAsioSerialPort();
		//! 检查读写是否超时
		void CheckRWDeadline(const boost::system::error_code e);
        //! 当前下载错误消息
        void SendDownloadErrorMsg(const wchar_t* msg,E_DOWNLOAD_STATUS eDownloadStatus = E_ERROR_DOWNLOAD);
        //! 当前下载OK消息
        void SendDownloadOkMsg(const wchar_t* msg,E_DOWNLOAD_STATUS eDownloadStatus = E_SUCCESSFUL_DOWNLOAD);
    private:
        //! 下载步骤
        EDownloadStep meDownloadStep;
       
        //! port option
        boost::asio::serial_port::flow_control::type meflow_control;
        boost::asio::serial_port::stop_bits::type mestop_bits;
        int mnbaud_rate;
        int mncharacter_size;
        //!
        size_t mnCheckBlankLength;
        char cmd[32];
        size_t a_address;
        size_t mread_length;
        size_t nbrOfSectors;
        //! 下载镜像当前索引
        size_t mnDownloadImageDataIndex;
        //! 验证下载镜像索引 
        size_t mnValidateImageDataIndex;
        //! 擦除flash镜像索引
        size_t mnEraseImageDataIndex;
        //! puma下载信息
        PS_PUMA_DOWNLOAD_INFO mpsPumaDownloadInfo;
        //!
        const S_PUMA_IMAGE_DATA_INFO* mpsPumaImageDataInfo;
        //! 接收缓冲区
        char* mRecvBuf/*[MAX_STRING_SIZE]*/;
        //! 接收索引当前的位置
        int mnRecvIndex ;
        //! 发送和接收超时器
        boost::asio::deadline_timer mdeadline_;
        //! 端口读写
        boost::asio::serial_port msp;
        //! 测试时间
        boost::timer mTestTime; 
        //! 下载进度信息
        S_DOWNLOAD_PROGRESS_INFO msDownloadProgressInfo;
    };
}


