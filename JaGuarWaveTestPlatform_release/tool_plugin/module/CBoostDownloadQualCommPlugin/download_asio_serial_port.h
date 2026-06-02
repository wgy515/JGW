#pragma once

#include <boost/progress.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "download_define.h"

//! 检查error code
#define CHECK_ERRORCODE_RETURN(ec,msg,ret_val) \
    if (ec)\
    {\
    dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,msg,JGW_A2W(ec.message()).c_str());\
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
    class download_asio_serial_port
    {
    public:
        download_asio_serial_port(boost::asio::io_service& ios,
            int nIndex
            );
        ~download_asio_serial_port(void);

	public:
        //! 开始下载 sahara 协议与 firehose 协议一起下载
        bool start_download(ps_download_all_file_info psDownloadAllFileInfo);
        //! 开始sahara协议下载
        bool start_sahara_download(bool bFirehoseDownload = false);
        //! <?xml version="1.0" ?> <data> <response value="NAK" AttemptRestart="1"/> </data> 失败后可以重新发送program命令，避免拆机
        //! 开始firehose协议下载
        bool start_firehose_download(const s_firehose_download_info* pVFirehose);
		
	public:
		S_DOWNLOAD_PROGRESS_INFO& GetDownladProgressInfo() { return msDownloadProgressInfo;}

		void set_port_option(int nbaud_rate = 9600,
			int ncharacter_size = 8,
			boost::asio::serial_port::flow_control::type eflow_control = boost::asio::serial_port::flow_control::none,
			boost::asio::serial_port::stop_bits::type estop_bits = boost::asio::serial_port::stop_bits::one);

	public:
//         //! 加载ARM PRG 非线程安全  因为一般arm prg文件都小于1M所以都保存在内存缓冲区，减少频繁从磁盘中读取文件
//         static bool reload_sahara_armprg_file_buffer(const char* szArmPrgFilePath);
//         //! 释放aramprg file buf 非线程安全
//         static void release_sahara_armprg_file_buffer();
//         //! 获取ARM PRG文件信息
//         static s_download_file_info& get_armprg_file_info();
        //! 第一次通信的时候才获取栈区信息
        static bool mbGetMaxPayloadSizeToTargetInBytes;
    private:
        //! 重新打开COM口
        bool reopen_serial_port();
        //! 发送配置命令
        void write_download_config_xml_to_port(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len);
        //! 读取配置命令字符串
        void read_download_config_xml_to_port(const boost::system::error_code& ec,size_t rx_len);
        //! 写入program下载配置文件
        void write_download_program_file_to_port(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len,boost::shared_ptr<FILE> fp);
        //! 读取program配置文件
        void read_download_program_file_to_port(const boost::system::error_code& ec,size_t rx_len);
        //! 检查当前设备是否处于rawmode模式
        bool check_firehose_device_is_rawmode();
        //! 正在发送firehose program镜像文件
        void send_firehose_program_image_end(boost::shared_ptr<FILE> fp);
        //! 开始发送firehose program镜像文件
        void send_firehose_program_image_start();
        /* sahara */
        void read_sahara_download_to_port(const boost::system::error_code& ec,size_t rx_len);
        //! 
        void write_sahara_download_to_port(const boost::system::error_code& ec,size_t tx_len_complete,size_t tx_len);
        //! 发送SAHARA 握手请求 填充hello请求结构体
        void send_sahara_hello_resp();
        //! 发送 ARM PRG镜像文件
        void send_sahara_armprg_file_image();
        //!
        void send_sahara_armprg_64bit_file_image();
        //! 镜像文件发送完毕后，发送给下载设备
        void send_sahara_image_end_done();
        //! 设备响应镜像下载完毕消息
        void send_sahara_image_end_done_resp();
        //!
        void send_sahara_reset();
        //! 关闭COM口
        void close_serial_port();
        //! 检查读写是否超时
        void check_deadline(const boost::system::error_code e);
        //! 初始化命令
        void init_command();
        //! 检查下载字符串  如果错误则返回true 
        bool is_error_download_response_command();
        //! 检查发送缓存的字节大小 -> 发送缓存必须SECTOR_SIZE_IN_BYTES的倍数
        size_t check_tx_buf_bytes(size_t tx_len);
        //! 判断sahara 下载状态
        bool is_ack_successful (int status);

        void GetMaxPayloadSizeToTargetInBytes();
        //! 写超时
        template <typename ConstBufferSequence, typename WriteHandler>
        void async_write_port_time_out(const ConstBufferSequence& buffers,
            BOOST_ASIO_MOVE_ARG(WriteHandler) handler)
        {
            boost::system::error_code ec;
            deadline_.expires_from_now(boost::posix_time::seconds(TX_TIMEOUT_SECOND),ec);
            PRINT_ERRORCODE(ec,L"deadline_.expires_from_now (%s)\n");
            deadline_.async_wait(boost::bind(&download_asio_serial_port::check_deadline, this,_1));
            sp.async_write_some(buffers,handler);
        }
        //! 读超时
        template <typename ConstBufferSequence, typename WriteHandler>
        void async_read_port_time_out(const ConstBufferSequence& buffers,
            BOOST_ASIO_MOVE_ARG(WriteHandler) handler)
        {
            boost::system::error_code ec;
            deadline_.expires_from_now(boost::posix_time::seconds(RX_TIMEOUT_SECOND),ec);
            PRINT_ERRORCODE(ec,L"deadline_.expires_from_now (%s)\n");
            deadline_.async_wait(boost::bind(&download_asio_serial_port::check_deadline, this,_1));     
            sp.async_read_some(buffers,handler);
        }
		//! 检查读取的长度是否为空，如果为空，则返回true
		bool	check_read_len_is_empty(size_t read_len);
		//! 检查预计发送的长度是否与实际长度一致 如果不一致则返回 true;
		bool	check_write_len_is_not_complete(size_t predict_length,size_t complete_length);
		//! 当前下载错误消息
		void	send_download_error_msg(const wchar_t* msg);
		//! 当前下载OK消息
		void	send_download_ok_msg(const wchar_t* msg);
    private:        
        //! 是否开启firehose协议下载
        bool mbFirehoseDownload;
		//! port option
        boost::asio::serial_port::flow_control::type meflow_control;
        boost::asio::serial_port::stop_bits::type mestop_bits;
        int mnbaud_rate;
        int mncharacter_size;

        //! 下载配置文件信息
        ps_download_all_file_info mps_download_all_file_info;

        //! 接收缓冲区
        char* mrx_buf/*[MAX_STRING_SIZE]*/;
        //! 发送缓冲区
        char* mtx_buf/*[FIREHOSE_TX_BUFFER_SIZE]*/;

		//! 重新发送program命令次数
		int  rewrite_program_command;
		boost::timer test_time; 

        //! 发送和接收超时器
        boost::asio::deadline_timer deadline_;
		//! 端口读写
		boost::asio::serial_port sp;
		//! sahara 信息结构体
		s_sahara_info sahara;

        s_firehose_all_command_info msfirehose_all_command_info;

		//! 下载进度信息
		S_DOWNLOAD_PROGRESS_INFO msDownloadProgressInfo;
    };
}


