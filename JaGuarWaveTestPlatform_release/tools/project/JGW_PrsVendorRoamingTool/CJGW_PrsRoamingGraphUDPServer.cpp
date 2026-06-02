#include "StdAfx.h"
#include "CJGW_PrsRoamingGraphUDPServer.h"
#include <Poco/Net/SocketAddress.h>
#include <Poco/Timespan.h>
#include <poco/InflatingStream.h>
#include <poco/StreamCopier.h>
#include <poco/MemoryStream.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#include "PrsVendorRoamingDataDefine.h"
using Poco::Net::Socket;
using Poco::Net::DatagramSocket;
using Poco::Net::SocketAddress;
using Poco::Net::IPAddress;

namespace JGW
{
    CCJGW_PrsRoamingGraphUDPServer::CCJGW_PrsRoamingGraphUDPServer() : _thread("PrsRoamingGraphUDPServer"), _stop(false),_socket(NULL)
    {
        mszReceiveBuffer = new char[TCP_RECEIVE_BUFFER_MAX_LENGTH + 1];

    }

    CCJGW_PrsRoamingGraphUDPServer::CCJGW_PrsRoamingGraphUDPServer(const SocketAddress& sa) : _thread("PrsRoamingGraphUDPServer"),_stop(false)
    {
        //_socket.bind(sa, true);
        //_thread.start(*this);
    }

    CCJGW_PrsRoamingGraphUDPServer::~CCJGW_PrsRoamingGraphUDPServer()
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

    bool CCJGW_PrsRoamingGraphUDPServer::StartPrsRoamingGraphUDPServer(Poco::NotificationQueue* pPrsRoamingMessageNotificationQueue)
    {
        if (NULL != _socket)
        {
            _socket->close();
            delete _socket;
            _socket = NULL;
        }
        try
        {
            mpPrsRoamingMessageNotificationQueue = pPrsRoamingMessageNotificationQueue;
            _socket = new DatagramSocket;
            _socket->bind(SocketAddress("localhost",PRS_ROAMING_GRAPH_UDP_SOCKET_PORT), true);
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

    void CCJGW_PrsRoamingGraphUDPServer::StopPrsRoamingGraphUDPServer()
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


    Poco::UInt16 CCJGW_PrsRoamingGraphUDPServer::port() const
    {
        return _socket->address().port();
    }

    void CCJGW_PrsRoamingGraphUDPServer::run()
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
                        Poco::AutoPtr<CCJGW_PrsRoamingMessageNotification> pNf = new CCJGW_PrsRoamingMessageNotification(std::string(mszReceiveBuffer,mszReceiveBuffer + nRecvLen));
                        mpPrsRoamingMessageNotificationQueue->enqueueNotification(pNf);	
                    }
                }
                catch (Poco::Exception& exc)
                {
                    JGW_FormatWString(mstrErrorMessage,L"Start Romming TCP Server Exception : %s",JGW_A2W(exc.displayText(),CP_UTF8).c_str());
                }
            }
        }
    }

    SocketAddress CCJGW_PrsRoamingGraphUDPServer::address() const
    {
        return _socket->address();
    }

    std::wstring CCJGW_PrsRoamingGraphUDPServer::GetErrorMessage() const
    {
        return mstrErrorMessage;
    }

}
