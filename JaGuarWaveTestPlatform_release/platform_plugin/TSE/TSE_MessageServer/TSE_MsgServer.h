#pragma once
#include <Poco/Net/Net.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Thread.h>
#include <Poco/Event.h>
#include "TSE_MsgManage.h"

namespace JGW
{
    class CTSE_MsgServer : public Poco::Runnable
    {
    public:
        CTSE_MsgServer(const std::string& hostAddress = "localhost",Poco::UInt16 portNumber = 3003);
        /// Creates the UDPEchoServer and binds it to

        CTSE_MsgServer(const Poco::Net::SocketAddress& sa);
        /// Creates the UDPEchoServer and binds it to
        /// the given address.
        ~CTSE_MsgServer(void);
    public:
        Poco::UInt16 port() const;
        /// Returns the port the echo server is
        /// listening on.

        Poco::Net::SocketAddress address() const;
        /// Returns the address of the server.	

        void run();
        /// Does the work.

        void Stop();
        /// stop the work

        CTSE_MsgManage* GetMsgManage();
    private:
        CTSE_MsgManage mcMsgManage;
        Poco::Net::SocketAddress mSocketAddress;
        Poco::Net::DatagramSocket _socket;
        Poco::Thread _thread;
        Poco::Event  _ready;
        bool         _stop;
        Poco::UInt16 _portNumber;
        char* _recvBuf;
    };
}

