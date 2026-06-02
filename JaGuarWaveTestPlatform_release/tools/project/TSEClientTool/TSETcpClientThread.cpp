#include "StdAfx.h"
#include "TSETcpClientThread.h"
#include "json/TSEServerJsonCommand.h"
#include <JGW_FoundationFunc/CUtf8String.hpp>

namespace JGW
{
    CTSETcpClientThread::CTSETcpClientThread(ITSEControlCmdForward& itseControlCmdForward,const std::string& strSocketIP,const size_t socketPort,const std::string& strClientFlags) : mClientSocket(mios),mdeadline_(mios),mITSEControlCmdForward(itseControlCmdForward),mstrSocketIP(strSocketIP),mnSocketPort(socketPort),mpWriteMsgQueue(NULL),mstrClientFlags(strClientFlags)
    {
        mszRecvBuf = (char*)calloc(TCP_RECV_MAX_BUFFER_LEN + 1,1);
        memset(mszRecvBuf,0x00,TCP_RECV_MAX_BUFFER_LEN + 1);
    }


    CTSETcpClientThread::~CTSETcpClientThread(void)
    {
        free(mszRecvBuf);
        mszRecvBuf = NULL;
    }

    bool CTSETcpClientThread::SocketIsConnected()
    {
        return mClientSocket.is_open();
    }

    void CTSETcpClientThread::CloseSocket()
    {
        boost::system::error_code ec;
        if (mClientSocket.is_open()) mClientSocket.close(ec);
    }

    void CTSETcpClientThread::AsyncConnnectSocket(size_t timeOutSec)
    {
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(mstrSocketIP),mnSocketPort);
        boost::system::error_code ec;
        mdeadline_.expires_from_now(boost::posix_time::seconds(timeOutSec),ec);
        mdeadline_.async_wait(boost::bind(&CTSETcpClientThread::CheckDeadline, this,_1));
        mClientSocket.async_connect(ep,boost::bind(&CTSETcpClientThread::ConnectedSocket,this,_1));
        mios.run();
        exit(0);
    }

    void CTSETcpClientThread::ConnectedSocket(const boost::system::error_code& ec)
    {
        if (ec) 
        {
            TimeOut(ec.message().c_str());
        }
        else
        {
            boost::system::error_code ecinfin;
            mdeadline_.expires_at(boost::posix_time::pos_infin,ecinfin);

            mstrClientFlagsJson = U(CTSEServerJsonCommand::GetFlagsJsonCommand(mstrClientFlags).c_str());
            std::shared_ptr<std::string> pstrData(new std::string);
            *pstrData = mstrClientFlagsJson;
            mClientSocket.async_write_some(boost::asio::buffer(*pstrData),boost::bind(&CTSETcpClientThread::WriteDataCmdCompleted,this,_1,_2,pstrData,mstrClientFlagsJson.length()));

            mClientSocket.async_read_some(boost::asio::buffer(mszRecvBuf,TCP_RECV_MAX_BUFFER_LEN),boost::bind(&CTSETcpClientThread::ReadControlCmdCompleted,this,_1,_2));
        } 
    }

    void CTSETcpClientThread::CheckDeadline(const boost::system::error_code e)
    {
        if (e) return;
        //! 真正的等待超时
        if (mdeadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())  
        {
            TimeOut("wait time out\n");	
            return;
        }  
    }

    void CTSETcpClientThread::TimeOut(const char* strErrorMsg)
    {
        boost::system::error_code ec;
        mdeadline_.expires_at(boost::posix_time::pos_infin,ec);
        /*PRINT_ERRORCODE(ec,"TimeOut(%s)\n");
        PELOG4WA(strErrorMsg);*/
        CloseSocket();
    }

    void CTSETcpClientThread::ReadControlCmdCompleted(const boost::system::error_code& ec,size_t rx_len)
    {
        if (ec)
        {
#ifdef _DEBUG
            OutputDebugStringA("ReadControlCmdCompleted");
            OutputDebugStringA(ec.message().c_str());
            OutputDebugStringA("\r\n");
#endif
        }
        else
        {
            mITSEControlCmdForward.OnForwardRecvControlCmdInfo(mszRecvBuf);
            mClientSocket.async_read_some(boost::asio::buffer(mszRecvBuf,TCP_RECV_MAX_BUFFER_LEN),boost::bind(&CTSETcpClientThread::ReadControlCmdCompleted,this,_1,_2));
        }
    }

    void CTSETcpClientThread::WriteInfoToTcpServer(std::queue<std::string>* pWriteMsgQueue)
    {
        mpWriteMsgQueue = pWriteMsgQueue;
        std::shared_ptr<std::string> pstrData(new std::string);
        //! *pstrData = JSON_CONTENT_SEPARATOR;
        *pstrData = "{\"ActionEnumID\":-1}";
        *pstrData += JSON_CONTENT_SEPARATOR;
        mClientSocket.async_write_some(boost::asio::buffer(*pstrData),boost::bind(&CTSETcpClientThread::WriteDataCmdCompleted,this,_1,_2,pstrData,pstrData->length()));
    }

    void CTSETcpClientThread::WriteDataCmdCompleted(const boost::system::error_code& ec,size_t txLenCompleted,std::shared_ptr<std::string> pstrData,size_t txLen)
    {
        if (ec)
        {
#ifdef _DEBUG
            OutputDebugStringA("WriteDataCmdCompleted");
            OutputDebugStringA(ec.message().c_str());
            OutputDebugStringA("\r\n");
#endif
        }
        else
        {
            //OutputDebugStringA(W(pstrData->c_str()));
            //OutputDebugStringA("\r\n");
            if (NULL != mpWriteMsgQueue && !mpWriteMsgQueue->empty())
            {
                std::shared_ptr<std::string> pstrData(new std::string);
                //! *pstrData = JSON_CONTENT_SEPARATOR;
                *pstrData = U(mpWriteMsgQueue->front().c_str());
                *pstrData += JSON_CONTENT_SEPARATOR;
                mpWriteMsgQueue->pop();
                mClientSocket.async_write_some(boost::asio::buffer(*pstrData),boost::bind(&CTSETcpClientThread::WriteDataCmdCompleted,this,_1,_2,pstrData,mstrClientFlagsJson.length()));
            }
        }
    }
}