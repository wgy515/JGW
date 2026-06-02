#pragma once
#include <Poco/Net/Net.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Thread.h>
#include <Poco/Event.h>
#include <string>
#include <queue>
#include "JGW_Cef3PositioningSystem.h"
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>
namespace JGW
{
	class CCJGW_Cef3PositioningSystemUDPScokect : public Poco::Runnable
	{
	public:
		CCJGW_Cef3PositioningSystemUDPScokect();
		/// Creates the UDPEchoServer.

		//CCJGW_Cef3PositioningSystemUDPScokect(const Poco::Net::SocketAddress& sa);
		/// Creates the UDPEchoServer and binds it to
		/// the given address.

		~CCJGW_Cef3PositioningSystemUDPScokect();
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
	public:
		void SendMsgToUdpSocket(const std::string& strMsg);
	private:
		void run();
		/// Does the work.

	private:
		char* mszReceiveBuffer;
		Poco::Net::DatagramSocket* _socket;
		Poco::Net::SocketAddress _sendSocket;
		Poco::Thread _thread;
		Poco::Event  _ready;
		std::queue<std::string> mqueueMsg;
		bool         _stop;
		std::wstring mstrErrorMessage;
		pOnRecvBuffer mpOnRecvBufferFunc;
		CCJGW_CriticalSectionLock mcCriticalSectionLock;
	};
}
