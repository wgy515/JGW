#pragma once
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#define TSE_FTM_MAX_READ_BUF 1024
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
        //!
        void ReadSerialNoID(const boost::system::error_code& ec,size_t rxLen,char* rxBuf);
        //! 
        bool AsyncConnnectSocket(const std::string& strSocketIP,const size_t socketPort,size_t timeOutSec);
        void ConnectedSocket(const boost::system::error_code& ec);
        //!
        void WriteredSocket(const boost::system::error_code& ec,size_t txLenCompleted,size_t txLen);
        //!
        void ReadSocket(const boost::system::error_code& ec,size_t rxLen,char* rxBuf);
        //! ¼ì²é¶ÁÐ´ÊÇ·ñ³¬Ê±
        void CheckDeadline(const boost::system::error_code e);
        //!
        void TimeOut(const char* strErrorMsg);
    private:
        bool mbResult;
        size_t mSocketPort;
        size_t mTimeOutSec;
        char* mRxBuf;
        std::string mstrSocketIP;
        boost::asio::io_service mios; 
        boost::asio::deadline_timer mdeadline_;
        boost::asio::ip::tcp::socket mClientSocket;
    };
}