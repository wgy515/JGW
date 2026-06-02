#pragma once
#include <Poco/Net/Net.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Thread.h>
#include <Poco/Event.h>
#include <string>
#include "JGW_Cef3MainProcesses.h"

#define TCP_RECEIVE_BUFFER_MAX_LENGTH (1024 * 10)
#define CEF3_UDP_SOCKET_PORT 9002

namespace JGW
{
    class CCJGW_Cef3UDPServer : public Poco::Runnable
    {
    public:
        CCJGW_Cef3UDPServer();
        /// Creates the UDPEchoServer.

        CCJGW_Cef3UDPServer(const Poco::Net::SocketAddress& sa);
        /// Creates the UDPEchoServer and binds it to
        /// the given address.

        ~CCJGW_Cef3UDPServer();
        /// Destroys the UDPEchoServer.

        Poco::UInt16 port() const;
        /// Returns the port the echo server is
        /// listening on.

        Poco::Net::SocketAddress address() const;
        /// Returns the address of the server.	

        std::wstring GetErrorMessage() const;
    public:
        bool StartCef3UDPServer(pOnRecvBuffer pOnRecvBufferFunc);
        void StopCef3UDPServer();
    private:
        void run();
        /// Does the work.

    private:
        char* mszReceiveBuffer;
        Poco::Net::DatagramSocket* _socket;
        Poco::Thread _thread;
        Poco::Event  _ready;
        bool         _stop;
        std::wstring mstrErrorMessage;
        pOnRecvBuffer mpOnRecvBufferFunc;
    };

}

