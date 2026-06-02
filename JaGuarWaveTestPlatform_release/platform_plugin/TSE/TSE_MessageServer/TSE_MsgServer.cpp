#include "StdAfx.h"
#include "TSE_MsgServer.h"
#include <TSE_MSFoundationLib/CTSE_MSFoundationDefine.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Timespan.h>
#include <poco/InflatingStream.h>
#include <poco/StreamCopier.h>
#include <poco/MemoryStream.h>
#include <iostream>
#include <sstream>

#include <JGW_FoundationFunc/JGW_StringFunc.h>
using Poco::Net::Socket;
using Poco::Net::DatagramSocket;
using Poco::Net::SocketAddress;
using Poco::Net::IPAddress;
extern void UpdateTSEMSConfigSocketPort(int port);
namespace JGW
{
    CTSE_MsgServer::CTSE_MsgServer(const std::string& hostAddress /* = "localhost" */,Poco::UInt16 portNumber /* = 3003 */) : mSocketAddress(hostAddress,portNumber),_stop(false),_portNumber(portNumber)
    {
        _recvBuf = (char*)calloc(MAX_MSG_SIZE + 1,1);
        Poco::UInt16 socketPort = portNumber;
        std::string strTemp;

        while (1)
        {
            try
            {
                _socket.bind(mSocketAddress, false);
                break;
            }
            catch (Poco::Exception& ec)
            {
                JGW_FormatString(strTemp,"Bind(%s:%u) Error:%s",hostAddress.c_str(),socketPort,ec.message().c_str());
            	PELOG4WW_F(JGW_A2W(strTemp).c_str());
            }
            socketPort++;
            UpdateTSEMSConfigSocketPort(socketPort);
            mSocketAddress = SocketAddress(hostAddress,socketPort);
        }
        _thread.start(*this);
        _ready.wait();
    }

    CTSE_MsgServer::CTSE_MsgServer(const Poco::Net::SocketAddress& sa) : mSocketAddress(sa),_stop(false)
    {
        //_socket.bind(mSocketAddress, false);
        _portNumber = sa.port();
        Poco::UInt16 socketPort = _portNumber;
        std::string strTemp;

        while (1)
        {
            try
            {
                _socket.bind(mSocketAddress, false);
                break;
            }
            catch (Poco::Exception& ec)
            {
                JGW_FormatString(strTemp,"Bind(%s:%u) Error:%s",sa.host().toString().c_str(),socketPort,ec.message().c_str());
                PELOG4WW_F(JGW_A2W(strTemp).c_str());
            }
            socketPort++;
            UpdateTSEMSConfigSocketPort(socketPort);
            mSocketAddress = SocketAddress(sa.host(),socketPort);
        }
        _thread.start(*this);
        _ready.wait();
        _recvBuf = (char*)calloc(MAX_MSG_SIZE + 1,1); 
    }


    CTSE_MsgServer::~CTSE_MsgServer(void)
    {
        Stop();
    }

    CTSE_MsgManage* CTSE_MsgServer::GetMsgManage()
    {
        return &mcMsgManage;
    }

    void CTSE_MsgServer::Stop()
    {
        if (_recvBuf != NULL)
        {
            mcMsgManage.CloseMsgManage();
            UpdateTSEMSConfigSocketPort(_portNumber);
            _stop = true;
            _thread.join();
            free(_recvBuf);
            _recvBuf = NULL;   
        }
    }

    Poco::UInt16 CTSE_MsgServer::port() const
    {
        return _socket.address().port();
    }

    void CTSE_MsgServer::run()
    {
        Poco::Timespan span(250000);
        while (!_stop)
        {
            _ready.set();
            if (_socket.poll(span, Socket::SELECT_READ))
            {
                try
                {
                    SocketAddress sender;
                    int n = _socket.receiveFrom(_recvBuf, MAX_MSG_SIZE, sender);
                    if (n > 0)
                    {
                        _recvBuf[n] = '\0';
#if 1
                        //! 解压接收到的字符串
                        Poco::MemoryInputStream istr(_recvBuf,n);
                        Poco::InflatingInputStream inflater(istr);
                        //! StreamCopier::copyStream(istr, deflater);
                        std::ostringstream buffer1;
                        Poco::StreamCopier::copyStream(inflater, buffer1);
#ifdef _DEBUG
                        //PDLOG4WW_F(JGW_A2W(JGW_GetFormatString("ReceiveFrom: %s",sender.toString().c_str())).c_str());
                        std::string strLog = "ReceiveFrom:";
                        strLog += buffer1.str();
                        PDLOG4WW(JGW_A2W(strLog).c_str());
#endif
                        mcMsgManage.HandleMsg(sender,buffer1.str());

#endif
                    }
                }
                catch (Poco::Exception& exc)
                {
                    PELOG4WW_F(JGW_A2W(JGW_GetFormatString("UDPEchoServer: %s",exc.displayText().c_str())).c_str());
                }
            }
        }
    }

    SocketAddress CTSE_MsgServer::address() const
    {
        return _socket.address();
    }

}
