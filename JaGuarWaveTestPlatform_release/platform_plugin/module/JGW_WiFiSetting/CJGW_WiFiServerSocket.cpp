#include "StdAfx.h"
#include "CJGW_WiFiServerSocket.h"

namespace JGW
{
#define PRINT_ERRORCODE(ec,msg)\
    if (ec) PELOG4WA_F(msg,ec.message().c_str());

    CCJGW_WiFiServerSocket::CCJGW_WiFiServerSocket() : mClientSocket(mios),mdeadline_(mios),mTimeOutSec(5),mbResult(true)
    {

    }


    CCJGW_WiFiServerSocket::~CCJGW_WiFiServerSocket(void)
    {
        CloseSocket();
    }

    bool CCJGW_WiFiServerSocket::AsyncConnnectSocket(const std::string& strSocketIP,const size_t socketPort,size_t timeOutSec)
    {
        if (SocketIsConnected()) CloseSocket();
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(strSocketIP),socketPort);
        boost::system::error_code ec;
        mios.reset();
        mTimeOutSec = timeOutSec;
        mbResult = true;
        mdeadline_.expires_from_now(boost::posix_time::seconds(timeOutSec),ec);
        PRINT_ERRORCODE(ec,"deadline_.expires_from_now (%s)");
        mdeadline_.async_wait(boost::bind(&CCJGW_WiFiServerSocket::CheckDeadline, this,_1));
        mClientSocket.async_connect(ep,boost::bind(&CCJGW_WiFiServerSocket::ConnectedSocket,this,_1));
        mios.run();
        return mbResult;
    }

    bool CCJGW_WiFiServerSocket::SocketIsConnected()
    {
        return mClientSocket.is_open();
    }


    void CCJGW_WiFiServerSocket::ConnectedSocket(const boost::system::error_code& ec)
    {
        if (ec) 
        {
            TimeOut(ec.message().c_str());
            return ;
        }
        boost::system::error_code ecinfin;
        mdeadline_.expires_at(boost::posix_time::pos_infin,ecinfin);
    }

    bool CCJGW_WiFiServerSocket::AsyncWriterSocket(const std::string& strBuf,size_t timeOutSec)
    {
        if (!SocketIsConnected()) return false;
        boost::system::error_code ec;
        mTimeOutSec = timeOutSec;
        mbResult = true;
        mios.reset();
        mdeadline_.expires_from_now(boost::posix_time::seconds(timeOutSec),ec);
        PRINT_ERRORCODE(ec,"deadline_.expires_from_now (%s)");
        mdeadline_.async_wait(boost::bind(&CCJGW_WiFiServerSocket::CheckDeadline, this,_1));
        mClientSocket.async_write_some(boost::asio::buffer(strBuf),boost::bind(&CCJGW_WiFiServerSocket::WriteredSocket,this,_1,_2,strBuf.length()));
        mios.run();
        return mbResult;
    }

    void CCJGW_WiFiServerSocket::WriteredSocket(const boost::system::error_code& ec,size_t txLenCompleted,size_t txLen)
    {
        if (ec) 
        {
            TimeOut(ec.message().c_str());
        }
        else
        {
            boost::system::error_code ecinfin;
            mdeadline_.expires_at(boost::posix_time::pos_infin,ecinfin);
        }  
    }

    bool CCJGW_WiFiServerSocket::AsyncReadSocket(char* rxBuf,size_t rxLen,size_t timeOutSec)
    {
        if (!SocketIsConnected()) return false;
        boost::system::error_code ec;
        mTimeOutSec = timeOutSec;
        mbResult = true;
        mios.reset();
        mdeadline_.expires_from_now(boost::posix_time::seconds(timeOutSec),ec);
        PRINT_ERRORCODE(ec,"deadline_.expires_from_now (%s)");
        mdeadline_.async_wait(boost::bind(&CCJGW_WiFiServerSocket::CheckDeadline, this,_1));
        mClientSocket.async_read_some(boost::asio::buffer(rxBuf,rxLen),boost::bind(&CCJGW_WiFiServerSocket::ReadSocket,this,_1,_2));
        mios.run();
        return mbResult;
    }

    void CCJGW_WiFiServerSocket::ReadSocket(const boost::system::error_code& ec,size_t rxLen)
    {
        if (ec) 
        {
            TimeOut(ec.message().c_str());
        }
        else
        {
            boost::system::error_code ecinfin;
            mdeadline_.expires_at(boost::posix_time::pos_infin,ecinfin);
        }  
    }

    void CCJGW_WiFiServerSocket::CheckDeadline(const boost::system::error_code e)
    {
        if (e) return;
        //! 真正的等待超时
        if (mdeadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())  
        {
            TimeOut("wait time out");	
            return;
        }  
    }

    void CCJGW_WiFiServerSocket::TimeOut(const char* strErrorMsg)
    {
        boost::system::error_code ec;
        mbResult = false;        
        mdeadline_.expires_at(boost::posix_time::pos_infin,ec);
        PRINT_ERRORCODE(ec,"TimeOut(%s)");
        PELOG4WA(strErrorMsg);
        CloseSocket();
    }

    void CCJGW_WiFiServerSocket::CloseSocket()
    {
        boost::system::error_code ec;
        if (mClientSocket.is_open()) mClientSocket.close(ec);
        PRINT_ERRORCODE(ec,"Close Socket(%s)");
    }

    bool CCJGW_WiFiServerSocket::CheckResult()
    {
        return mbResult;
    }
}