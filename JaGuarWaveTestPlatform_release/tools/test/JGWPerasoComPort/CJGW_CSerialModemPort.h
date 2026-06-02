#pragma once

#include <string>
#include "CJGW_CSerialPort.h"
#define TX_QUEUE_BUFFER_LEN 1024
#define RX_QUEUE_BUFFER_LEN 1024
namespace JGW
{
    class  CCJGW_CSerialModemPort
    {
    public:
        CCJGW_CSerialModemPort(void);
        ~CCJGW_CSerialModemPort(void);
        //! 打开MODEM端口 strComName = "COM12" mdwFlagsAndAttributes 异步或者同步
        bool OpenModemPort(std::wstring strComName,int nbaud_rate = CBR_9600,DWORD dwFlagsAndAttributes = FILE_FLAG_OVERLAPPED);
        //! 关闭MODEM端口
        void CloseModemPort();
        //! szFormat = AT+PID?
        bool SendATCommandOK(const char* szFormat,const char* szValue,long millis = 4 * 1000);
        //! 读取AT命令返回值
        bool ReadATCommandValue(const char* szFormat,const char* szValue,std::string& strReturnValue,long millis = 4 * 1000);
        //! 获取端口写入错误
        const wchar_t* GetModemLastErrorLog();
    private:
        //! 写AT命令
        int WriteAsyncATModem(const char* szATCommand,size_t len,long millis = 4 * 1000); 
        //! 读取AT命令
        int ReadATAsyncModem(std::string& strRead,long millis = 4 * 1000,size_t max_len = RX_QUEUE_BUFFER_LEN);
        //! 读取AT命令
        int ReadATAsyncModem(char* strRead,size_t len,long millis = 4 * 1000);
        //! 是否打开MODEM端口 true: 打开MODEM false :
        bool IsOpenModemPort();  
    private:
        bool OpenPort(const wchar_t* szComName);

        bool SetControlCommState(int nbaud_rate);

        bool SetModemTimeOuts();

        bool CheckIsAsyncRWATModem(const char* szATCommand);

        void GetATCommandString(const char* szATCommand,size_t &len);
    private :
        //! 串口句柄
        DWORD               mdwFlagsAndAttributes;
        CCJGW_CSerialPort    mSerialPort;
        OVERLAPPED          mov;
        char*               mTxBuf;
        char*               mRxBuf;
    };
}

