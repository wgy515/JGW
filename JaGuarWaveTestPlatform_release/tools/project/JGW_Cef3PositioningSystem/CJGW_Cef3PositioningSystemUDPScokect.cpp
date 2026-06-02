#include "StdAfx.h"
#include "CJGW_Cef3PositioningSystemUDPScokect.h"
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
	CCJGW_Cef3PositioningSystemUDPScokect::CCJGW_Cef3PositioningSystemUDPScokect(void): _thread("Cef3UDPServer"), _stop(false),_socket(NULL),mpOnRecvBufferFunc(NULL),_sendSocket("127.0.0.1",DXCS_BLE_TX_UDP_SOCKET_PORT)
	{
		mszReceiveBuffer = new char[DXCS_RECEIVE_BUFFER_MAX_LENGTH + 1];
	}


	CCJGW_Cef3PositioningSystemUDPScokect::~CCJGW_Cef3PositioningSystemUDPScokect(void)
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

	Poco::UInt16 CCJGW_Cef3PositioningSystemUDPScokect::port() const
	{
		return _socket->address().port();
	}

	bool CCJGW_Cef3PositioningSystemUDPScokect::StartCef3UDPServer(pOnRecvBuffer pOnRecvBufferFunc)
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
			_socket->bind(SocketAddress("localhost",LOCAL_UDP_SOCKET_PORT), true);
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

	void CCJGW_Cef3PositioningSystemUDPScokect::StopCef3UDPServer()
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

	void CCJGW_Cef3PositioningSystemUDPScokect::run()
	{
		Poco::Timespan span(500);
		SocketAddress senderSocket;

		while (!_stop)
		{
			//_ready.set();
			if (_socket->poll(span, Socket::SELECT_READ))
			{
				try
				{
					SocketAddress sender;
					int nRecvLen = _socket->receiveFrom(mszReceiveBuffer, DXCS_RECEIVE_BUFFER_MAX_LENGTH, sender);
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
			if (_socket->poll(span, Socket::SELECT_WRITE))
			{
				while (!mqueueMsg.empty())
				{
					std::string strMsg;
					{
						CCJGW_CriticalSectionAutoLock autoLock(mcCriticalSectionLock);
						strMsg = mqueueMsg.front();
						mqueueMsg.pop();
					}
					try
					{
						_socket->sendTo(strMsg.c_str(),strMsg.length(),_sendSocket);
					}
					catch (Poco::Exception& exc)
					{
						JGW_FormatWString(mstrErrorMessage,L"SendTo Exception : %s",JGW_A2W(exc.displayText(),CP_UTF8).c_str());
					}
				}
			}
		}
	}

	void CCJGW_Cef3PositioningSystemUDPScokect::SendMsgToUdpSocket(const std::string& strMsg)
	{
		CCJGW_CriticalSectionAutoLock autoLock(mcCriticalSectionLock);
		mqueueMsg.push(strMsg);
	}

	SocketAddress CCJGW_Cef3PositioningSystemUDPScokect::address() const
	{
		return _socket->address();
	}

	std::wstring CCJGW_Cef3PositioningSystemUDPScokect::GetErrorMessage() const
	{
		return mstrErrorMessage;
	}
}
