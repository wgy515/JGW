#pragma once
#include "CJGW_PrsVendorRoamingServer.h"
#include <Poco/Net/Net.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Thread.h>
#include <Poco/Event.h>
#include <string>
#include <Poco/NotificationQueue.h>

namespace JGW
{
    class CCJGW_PrsRoamingGraphUDPServer : public Poco::Runnable
    {
    public:
        CCJGW_PrsRoamingGraphUDPServer();
        /// Creates the UDPEchoServer.

        CCJGW_PrsRoamingGraphUDPServer(const Poco::Net::SocketAddress& sa);
        /// Creates the UDPEchoServer and binds it to
        /// the given address.

        ~CCJGW_PrsRoamingGraphUDPServer();
        /// Destroys the UDPEchoServer.

        Poco::UInt16 port() const;
        /// Returns the port the echo server is
        /// listening on.

        Poco::Net::SocketAddress address() const;
        /// Returns the address of the server.	

        std::wstring GetErrorMessage() const;
    public:
        bool StartPrsRoamingGraphUDPServer(Poco::NotificationQueue* pPrsRoamingMessageNotificationQueue);
        void StopPrsRoamingGraphUDPServer();
    private:
        void run();
        /// Does the work.

    private:
        Poco::NotificationQueue* mpPrsRoamingMessageNotificationQueue;
        char* mszReceiveBuffer;
        Poco::Net::DatagramSocket* _socket;
        Poco::Thread _thread;
        Poco::Event  _ready;
        bool         _stop;
        std::wstring mstrErrorMessage;
    };
}


