#pragma once
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <string>
namespace JGW
{
    class CCJGW_WiFiServerSocket
    {
    public:
        CCJGW_WiFiServerSocket();
        ~CCJGW_WiFiServerSocket(void);
        //! 
        bool AsyncConnnectSocket(const std::string& strSocketIP,const size_t socketPort,size_t timeOutSec);
        //! 
        bool AsyncWriterSocket(const std::string& strBuf,size_t timeOutSec);
        //!
        bool AsyncReadSocket(char* rxBuf,size_t rxLen,size_t timeOutSec);
        //!
        bool SocketIsConnected();
        //! 
        bool CheckResult();
        //! 
        void CloseSocket();
    private:
        void ConnectedSocket(const boost::system::error_code& ec);
        //!
        void WriteredSocket(const boost::system::error_code& ec,size_t txLenCompleted,size_t txLen);
        //!
        void ReadSocket(const boost::system::error_code& ec,size_t rxLen);
        //! ¼ì²é¶ÁÐ´ÊÇ·ñ³¬Ê±
        void CheckDeadline(const boost::system::error_code e);
        //!
        void TimeOut(const char* strErrorMsg);
    private:
        bool mbResult;
        size_t mTimeOutSec;
        boost::asio::io_service mios;
        boost::asio::ip::tcp::socket mClientSocket;
        boost::asio::deadline_timer mdeadline_;
    };
}