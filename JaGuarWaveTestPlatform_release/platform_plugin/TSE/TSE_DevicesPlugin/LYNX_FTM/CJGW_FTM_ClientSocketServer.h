#pragma once
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <string>

namespace JGW
{
    enum E_SOCKET_MSG
    {

    };

    struct S_CLIENT_SOCKET_SERVER_MSG 
    {

    };
    class CCJGW_FTM_ClientSocketServer
    {
    public:
        CCJGW_FTM_ClientSocketServer(void);
        ~CCJGW_FTM_ClientSocketServer(void);

    public:
        bool IsStartClientSocketServer() { return mbClientSocketServerStatusIsStart;}

        bool ConnectClientSocketServer(const std::string& strSocketIP,const size_t socketPort,size_t timeOutSec);
    private:
        //! 
        void StartClientSocketServer();
        //! 连接Socket
        void ConnectedSocket(const boost::system::error_code& ec);
        //! 检查读写是否超时
        void OnTimerServer(const boost::system::error_code e);
        //!
        void HandleClientSocketServerMsg();
    private:
        bool mbClientSocketServerStatusIsStart;
        bool mbResult;
        size_t mSocketPort;
        size_t mTimeOutSec;
        std::string mstrSocketIP;
        boost::asio::io_service mios;
        boost::asio::ip::tcp::socket mClientSocket;
        boost::asio::deadline_timer mdeadline_;
    };
}