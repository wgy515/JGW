#include "StdAfx.h"
#include "CJGW_FTM_ClientSocketServer.h"
#define CLIENT_SOCKET_SERVER_TIME 1000
namespace JGW
{
#define PRINT_ERRORCODE(ec,msg)\
    if (ec) LogE_F(msg,ec.message().c_str());

    CCJGW_FTM_ClientSocketServer::CCJGW_FTM_ClientSocketServer(void) : mdeadline_(mios)
        ,mClientSocket(mios),mstrSocketIP("127.0.0.1"),mSocketPort(5432),mbClientSocketServerStatusIsStart(false)
    {
    }


    CCJGW_FTM_ClientSocketServer::~CCJGW_FTM_ClientSocketServer(void)
    {
    }

    bool CCJGW_FTM_ClientSocketServer::ConnectClientSocketServer(const std::string& strSocketIP,const size_t socketPort,size_t timeOutSec)
    {
        try
        {
            boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(strSocketIP),socketPort);

            boost::system::error_code ec;
            mios.reset();
            mTimeOutSec = timeOutSec;
            mSocketPort = socketPort;
            mstrSocketIP = strSocketIP;
            mbResult = true;
            mdeadline_.expires_from_now(boost::posix_time::seconds(timeOutSec),ec);
            PRINT_ERRORCODE(ec,"deadline_.expires_from_now (%s)\n");
            mdeadline_.async_wait(boost::bind(&CCJGW_FTM_ClientSocketServer::OnTimerServer, this,_1));
            mClientSocket.async_connect(ep,boost::bind(&CCJGW_FTM_ClientSocketServer::ConnectedSocket,this,_1));
        } 
        catch (boost::system::system_error& ec)
        {
            LogE_F("(%s)\n",ec.what());
            return false;
        }
        mios.run();
        return mbResult;
    }

    void CCJGW_FTM_ClientSocketServer::ConnectedSocket(const boost::system::error_code& ec)
    {
        if (ec) 
        {
            //! TimeOut(ec.message().c_str());
            return ;
        }
        boost::system::error_code ecinfin;
        mdeadline_.expires_at(boost::posix_time::pos_infin,ecinfin);
    }

    void CCJGW_FTM_ClientSocketServer::OnTimerServer(const boost::system::error_code e)
    {
        if (e) return;
        //! 超时
        if (mdeadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())  
        {
            if (mbClientSocketServerStatusIsStart)
            {
                boost::system::error_code ec;
                mdeadline_.expires_from_now(boost::posix_time::seconds(CLIENT_SOCKET_SERVER_TIME),ec);
                PRINT_ERRORCODE(ec,"deadline_.expires_from_now (%s)\n");
                //! 开启定时器服务
                mdeadline_.async_wait(boost::bind(&CCJGW_FTM_ClientSocketServer::OnTimerServer, this,_1));
                HandleClientSocketServerMsg();
            }
        }  
    }

    void CCJGW_FTM_ClientSocketServer::StartClientSocketServer()
    {
        try
        {
            boost::system::error_code ec;
            mdeadline_.expires_from_now(boost::posix_time::seconds(CLIENT_SOCKET_SERVER_TIME),ec);
            PRINT_ERRORCODE(ec,"deadline_.expires_from_now (%s)\n");
            //! 开启定时器服务
            mdeadline_.async_wait(boost::bind(&CCJGW_FTM_ClientSocketServer::OnTimerServer, this,_1));
        } 
        catch (boost::system::system_error& ec)
        {
            LogE_F("(%s)\n",ec.what());
            return /*false*/;
        }
        mios.run();
        mbClientSocketServerStatusIsStart = false;
    }

    void CCJGW_FTM_ClientSocketServer::HandleClientSocketServerMsg()
    {

    }
}