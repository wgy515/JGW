#pragma once
#include <JGW_WindowsFuncPlugin/CJGW_CSerialPort.h>
namespace JGW
{
    class CCJGW_QcommDownloadSerialPort
    {
    public:
        CCJGW_QcommDownloadSerialPort(void);
        ~CCJGW_QcommDownloadSerialPort(void);
    public:
        //! 打开COM端口 strComName = "COM12" mdwFlagsAndAttributes 异步或者同步
        bool OpenSaharaSerialComPort(std::wstring strComName,int nbaud_rate = CBR_115200,DWORD dwFlagsAndAttributes = FILE_FLAG_OVERLAPPED);
        //! 是否打开MODEM端口 true: 打开MODEM false :
        bool IsOpenComPort();
        //! 关闭串口
        void CloseQcommDownloadSerialPort();
        //! 设置写入超时
        void SetTxTimeOutMSec(int timeout);
        //! 设置读取超时
        void SetRxTimeOutMSec(int timeout);
    public:
        //! 写缓冲区到下载端口
        bool WriteBufferToDownloadPort(const char* szBuf,size_t len,size_t& writen_len);
        //! 读取下载端口
        bool ReadDownloadPort(char* buffer, size_t bytes_to_read, size_t& bytes_read);
        //!
        //bool ReadSaharaDownloadPort(char* buffer, size_t bytes_to_read, size_t& bytes_read);
        //! 读stream下载协议内容
        bool ReadStreamingDownloadPort(char* buffer, size_t bytes_to_read, size_t& bytes_read);
    private:
        bool OpenPort(const wchar_t* szComName);

        bool SetControlCommState(int nbaud_rate);

        bool SetModemTimeOuts();
    private:
        int mnTxTimeOutMSec;
        int mnRxTimeOutMSec;
        DWORD mdwFlagsAndAttributes;
        OVERLAPPED mov;
        CCJGW_CSerialPort mSerialPort;
    };
}

