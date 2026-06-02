#pragma once

#include <boost/progress.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "9x25_download_define.h"
#include "CJGW_SSDownloadFramePacket.h"

//! 检查error code
#define CHECK_ERRORCODE_RETURN(ec,msg,ret_val) \
    if (ec)\
    {\
    dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,msg,ec.message().c_str());\
    return ret_val;\
    }
//! 检查error code
#define CHECK_ERRORCODE(ec,msg) \
    if (ec)\
    {\
    dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,msg,ec.message().c_str());\
    return;\
    }
//! 打印错误消息
#define PRINT_ERRORCODE(ec,msg)\
    if (ec) dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,msg,ec.message().c_str());
//! 小于1M的文件都保存在缓冲区中
namespace JGW
{
    class ss_download_asio_serial_port
    {
    public:
        ss_download_asio_serial_port(boost::asio::io_service& ios,
            int nIndex
            );
        ~ss_download_asio_serial_port(void);

	public:
        //! 开始下载 sahara 协议与 firehose 协议一起下载
        bool start_download(DWORD64 nFileSizeCounts,ps_sahara_protocol_armprg_file_info psSaharahProtocolArmPrgFile,s_streaming_download_info* ps_streaming_download_info,bool bStreamingDownload = true);
        //! 开始sahara协议下载
        bool start_sahara_download(bool bFirehoseDownload = false);
		//! 开始streaming协议下载
        bool start_streaming_download(DWORD64 nFileSizeCounts,s_streaming_download_info* psStreamingDownloadInfo);
	public:
		S_DOWNLOAD_PROGRESS_INFO& GetDownladProgressInfo() { return msDownloadProgressInfo;}

		void set_port_option(int nbaud_rate = 9600,
			int ncharacter_size = 8,
			boost::asio::serial_port::flow_control::type eflow_control = boost::asio::serial_port::flow_control::none,
			boost::asio::serial_port::stop_bits::type estop_bits = boost::asio::serial_port::stop_bits::one);

	public:
        static bool mbGetMaxPayloadSizeToTargetInBytes;
    private:
        //! 重新打开COM口
        bool reopen_serial_port();
        /* sahara */
        void read_sahara_download_to_port(const boost::system::error_code& ec,size_t rx_len);
        //! 
        void write_sahara_download_to_port(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len);
        //! 发送SAHARA 握手请求 填充hello请求结构体
        void send_sahara_hello_resp();
        //! 发送 ARM PRG镜像文件
        void send_sahara_armprg_file_image();
        //! 镜像文件发送完毕后，发送给下载设备
        void send_sahara_image_end_done();
        //! 设备响应镜像下载完毕消息
        void send_sahara_image_end_done_resp();
        //!
        void send_sahara_switch_mode_packet();
        //! 
        void write_sahara_switch_mode_packet_to_port(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len);
        //!
        void send_sahara_reset();
        //! streaming 
        void init_command();
        //! 
        void write_streaming_download_image_file(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len,boost::shared_ptr<FILE> fp);
        //!
        void read_streaming_download_image_file_response(const boost::system::error_code& ec,size_t rx_len,boost::shared_ptr<FILE> fp);
        void write_streaming_hello_command(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len);

        void read_streaming_hello_response_command(const boost::system::error_code& ec,size_t rx_len);

        void response_streaming_multi_image_open_cmd();

        void check_streaming_error_code(UINT32 error_code);

        void write_streaming_multi_image_buf_to_port(boost::shared_ptr<FILE> fp);

        size_t build_unframe_streaming_multi_image_packet(size_t read_len);

        size_t build_streaming_multi_image_packet(size_t read_len/*,size_t address*/);

        //! void print_error_command_to_buf(char* error_buf,size_t read_len);

        void print_error_command(size_t read_len);
        //! 关闭COM口
        void close_serial_port();
        //! 检查读写是否超时
        void check_deadline(const boost::system::error_code e);
        //! 判断sahara 下载状态
        bool is_ack_successful (int status);

        //! void GetMaxPayloadSizeToTargetInBytes();
        //! 写超时
        template <typename ConstBufferSequence, typename WriteHandler>
        void async_write_port_time_out(const ConstBufferSequence& buffers,
            BOOST_ASIO_MOVE_ARG(WriteHandler) handler)
        {
            boost::system::error_code ec;
            deadline_.expires_from_now(boost::posix_time::seconds(TX_TIMEOUT_SECOND),ec);
            PRINT_ERRORCODE(ec,"deadline_.expires_from_now (%s)\n");
            deadline_.async_wait(boost::bind(&ss_download_asio_serial_port::check_deadline, this,_1));
            sp.async_write_some(buffers,handler);
        }
        //! 读超时
        template <typename ConstBufferSequence, typename WriteHandler>
        void async_read_port_time_out(const ConstBufferSequence& buffers,
            BOOST_ASIO_MOVE_ARG(WriteHandler) handler)
        {
            boost::system::error_code ec;
            deadline_.expires_from_now(boost::posix_time::seconds(RX_TIMEOUT_SECOND),ec);
            PRINT_ERRORCODE(ec,"deadline_.expires_from_now (%s)\n");
            deadline_.async_wait(boost::bind(&ss_download_asio_serial_port::check_deadline, this,_1));     
            sp.async_read_some(buffers,handler);
        }
		//! 检查读取的长度是否为空，如果为空，则返回true
		bool	check_read_len_is_empty(size_t read_len);
		//! 检查预计发送的长度是否与实际长度一致 如果不一致则返回 true;
		bool	check_write_len_is_not_complete(size_t predict_length,size_t complete_length);
		//! 当前下载错误消息
		void	send_download_error_msg(const char* msg);
		//! 当前下载OK消息
		void	send_download_ok_msg(const char* msg);
    private:
        //! 流协议下载命令枚举
        Streaming_Download_Cmd_Enum meStreamingDownloadCmdEnum;
        //! 是否开启streaming协议下载
        bool mbStreamingDownload;
		//! port option
        boost::asio::serial_port::flow_control::type meflow_control;
        boost::asio::serial_port::stop_bits::type mestop_bits;
        int mnbaud_rate;
        int mncharacter_size;
        //! streaming protoclo command
        s_stream_protocol_command* mps_stream_protocol_command;
        //! 下载配置文件信息
        s_ss_download_asio_config_info msSSDownloadAsioConfigInfo;

        //! 接收缓冲区
        char* mrx_buf/*[MAX_STRING_SIZE]*/;
        //! 发送缓冲区
        char* mtx_buf/*[FIREHOSE_TX_BUFFER_SIZE]*/;
        //! 镜像文件临时保存buf
        s_file_temp_buf_info msFileTempBufInfo;
        //! SAHARA_MODE_MEMORY_DEBUG
        sahara_packet_hello ms_sahara_hello_memory;
		//! 重新发送program命令次数
		int  rewrite_program_command;
		boost::timer test_time; 
        //! 最大的写入命令 默认为1024
        size_t mMaximum_write_len;
        //! streaming 封栈实例类
        CCJGW_SSDownloadFramePacket mFramePacket;
        //! 发送和接收超时器
        boost::asio::deadline_timer deadline_;
		//! 端口读写
		boost::asio::serial_port sp;
		//! sahara 信息结构体
		s_sahara_info sahara;
        //! streaming 信息
        s_streaming_info streaming;
        //! arm prg文件信息
        s_file_info sArmPrgFileInfo;
		//! 下载进度信息
		S_DOWNLOAD_PROGRESS_INFO msDownloadProgressInfo;
    };
}