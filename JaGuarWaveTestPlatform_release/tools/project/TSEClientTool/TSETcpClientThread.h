#pragma once
#include <string>
#include "ITSEControlCmdForward.h"
#include "IWriteInfoServerInterface.h"
#include <queue>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace JGW
{
    class CTSETcpClientThread : public IWriteInfoServerInterface
    {
    public:
        CTSETcpClientThread(ITSEControlCmdForward& itseControlCmdForward,const std::string& strSocketIP,const size_t socketPort,const std::string& strClientFlags);
        ~CTSETcpClientThread(void);
    public:
        //! 
        void AsyncConnnectSocket(size_t timeOutSec);
        //!
        bool SocketIsConnected();
    private:
        void ConnectedSocket(const boost::system::error_code& ec);
        //! 
        void ReadControlCmdCompleted(const boost::system::error_code& ec,size_t rx_len);
        //!
        void WriteDataCmdCompleted(const boost::system::error_code& ec,size_t txLenCompleted,std::shared_ptr<std::string> pstrData,size_t txLen);
        //! 写入相关信息到TCP SERVER端
        void WriteInfoToTcpServer(std::queue<std::string>* pWriteMsgQueue);
        //! 
        void CloseSocket();
        //! 检查读写是否超时
        void CheckDeadline(const boost::system::error_code e);
        //!
        void TimeOut(const char* strErrorMsg);
    private:
        char* mszRecvBuf;
        std::queue<std::string>* mpWriteMsgQueue;
        size_t mnSocketPort;
        ITSEControlCmdForward& mITSEControlCmdForward;
        std::string mstrSocketIP;
        std::string mstrClientFlags;
        std::string mstrClientFlagsJson;
        boost::asio::io_service mios;
        boost::asio::ip::tcp::socket mClientSocket;
        boost::asio::deadline_timer mdeadline_;
    };
}


