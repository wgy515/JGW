#pragma once
#include <JGW_WindowsFuncPlugin/CJGW_AsyncSocket.h>
namespace JGW
{
    class CCJGW_FTM_ClientSocket
    {
    public:
        CCJGW_FTM_ClientSocket();
        ~CCJGW_FTM_ClientSocket(void);
        //!
        bool AsyncAdbTcpIPConnnectSocket(const std::string& strSocketIP,const size_t socketPort,size_t timeOutSec);
       
        //! 
        bool AsyncWriterSocket(const std::string& strBuf,size_t timeOutSec);
        //!
        bool AsyncReadSocket(char* rxBuf,size_t rxLen,size_t timeOutSec);
        //! 
        bool CheckResult();
        //! 
        void CloseSocket();
    private:
        bool IsAdbTcpIP();
    private:
        bool mbResult;
        size_t mSocketPort;
        size_t mTimeOutSec;
        std::string mstrSocketIP;
        CCJGW_AsyncSocket mAsyncSocket;
    };
}