#include "StdAfx.h"
#include "CJGW_Cef3UDPServer.h"
#include <Poco/Net/SocketAddress.h>
#include <Poco/Timespan.h>
#include <poco/InflatingStream.h>
#include <poco/StreamCopier.h>
#include <poco/MemoryStream.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

using Poco::Net::Socket;
using Poco::Net::DatagramSocket;
using Poco::Net::SocketAddress;
using Poco::Net::IPAddress;

namespace JGW
{
    CCJGW_Cef3UDPServer::CCJGW_Cef3UDPServer(void): _thread("Cef3UDPServer"), _stop(false),_socket(NULL),mpOnRecvBufferFunc(NULL)
    {
        mszReceiveBuffer = new char[TCP_RECEIVE_BUFFER_MAX_LENGTH + 1];
    }


    CCJGW_Cef3UDPServer::~CCJGW_Cef3UDPServer(void)
    {
        _stop = true;
        _thread.join();
        if (NULL != _socket)
        {
            _socket->close();
            _socket = NULL;
        }
        delete []mszReceiveBuffer;
    }


    bool CCJGW_Cef3UDPServer::StartCef3UDPServer(pOnRecvBuffer pOnRecvBufferFunc)
    {
        if (NULL != _socket)
        {
            _socket->close();
            delete _socket;
            _socket = NULL;
        }
        try
        {
            mpOnRecvBufferFunc = pOnRecvBufferFunc;
            _socket = new DatagramSocket;
            _socket->bind(SocketAddress("localhost",CEF3_UDP_SOCKET_PORT), true);
            _thread.start(*this);
            //_ready.wait();
            return true;
        }
        catch (Poco::Exception& ec)
        {
            JGW_FormatWString(mstrErrorMessage,L"Start Romming TCP Server Exception : %s",JGW_A2W(ec.displayText(),CP_UTF8).c_str());
        }
        return false;
    }

    void CCJGW_Cef3UDPServer::StopCef3UDPServer()
    {
        _stop = true;
        if (_thread.isRunning())  _thread.join();

        if (NULL != _socket)
        {
            _socket->close();
            delete _socket;
            _socket = NULL;
        }
    }


    Poco::UInt16 CCJGW_Cef3UDPServer::port() const
    {
        return _socket->address().port();
    }

    void CCJGW_Cef3UDPServer::run()
    {
        Poco::Timespan span(250000);
        SocketAddress senderSocket;

        while (!_stop)
        {
            //_ready.set();
            if (_socket->poll(span, Socket::SELECT_READ))
            {
                try
                {
                    SocketAddress sender;
                    int nRecvLen = _socket->receiveFrom(mszReceiveBuffer, TCP_RECEIVE_BUFFER_MAX_LENGTH, sender);
                    if (nRecvLen > 0)
                    {
                        mszReceiveBuffer[nRecvLen] = '\0';
                        std::string strRecvBuffer(mszReceiveBuffer,mszReceiveBuffer + nRecvLen);
                        if (NULL != mpOnRecvBufferFunc) mpOnRecvBufferFunc(strRecvBuffer);
                    }
                }
                catch (Poco::Exception& exc)
                {
                    JGW_FormatWString(mstrErrorMessage,L"Start Romming TCP Server Exception : %s",JGW_A2W(exc.displayText(),CP_UTF8).c_str());
                }
            }
        }
    }

    SocketAddress CCJGW_Cef3UDPServer::address() const
    {
        return _socket->address();
    }

    std::wstring CCJGW_Cef3UDPServer::GetErrorMessage() const
    {
        return mstrErrorMessage;
    }

}
