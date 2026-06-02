#pragma once
#include "CJGW_CSerialPort.h"
#define TX_QUEUE_COMPORT_BUFFER_LEN 2048
#define RX_QUEUE_COMPORT_BUFFER_LEN 2048
namespace JGW
{
    class  CCJGW_CSerialComPort
    {
    public:
        CCJGW_CSerialComPort(const char* endStr = ">");
        ~CCJGW_CSerialComPort(void);

        void EraseCliString(std::string& strRead);

        void SetEndString(const char* endStr = "\n") {mszEndString = endStr;}
        //! 打开COM端口 strComName = "COM12" mdwFlagsAndAttributes 异步或者同步
        bool OpenSerialComPort(std::wstring strComName,int nbaud_rate = CBR_9600,DWORD dwFlagsAndAttributes = FILE_FLAG_OVERLAPPED);
        //! 关闭COM端口
        void CloseSerialComPort();

        bool WriteCommand(std::string strCommand,long millis = 4 * 1000);
        //!
        bool WriteBinaryFileBufferToComPort(const char* szBuf,size_t len,long millis = 4 * 1000);

        bool ReadResponseCommandToComPort(std::string& strRead,long millis = 4 * 1000);
        //! 读取AT命令返回值
        bool ReadRespCommandToWriteCommand(const char* szFormat,std::string& strReturnValue,long millis = 4 * 1000);
        //! 获取端口写入错误
        const wchar_t* GetComPortLastErrorLog();
    private:
        //! 写AT命令
        int WriteAsyncCommand(const char* szATCommand,size_t len,long millis = 4 * 1000); 
        //! 读取AT命令
        int ReadAsyncCommand(std::string& strRead,long millis = 4 * 1000,size_t max_len = RX_QUEUE_COMPORT_BUFFER_LEN);
        //! 读取AT命令
        int ReadAsyncCommand(char* strRead,size_t len,long millis = 4 * 1000);
        //! 是否打开MODEM端口 true: 打开MODEM false :
        bool IsOpenComPort();  
    private:
        bool OpenPort(const wchar_t* szComName);

        bool SetControlCommState(int nbaud_rate);

        bool SetModemTimeOuts();

        bool CheckIsAsyncRWCoammnd(const char* szATCommand);

        void GetATCommandString(const char* szATCommand,size_t &len);
    private :
        //! 串口句柄
        DWORD               mdwFlagsAndAttributes;
        CCJGW_CSerialPort    mSerialPort;
        OVERLAPPED          mov;
        char*               mTxBuf;
        char*               mRxBuf;
        const char*         mszEndString;
    };
}

