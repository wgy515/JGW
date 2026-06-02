#include "StdAfx.h"


#include "CJGW_MesTcpServerManager.h"

#include <poco/Net/ServerSocket.h>
#include <poco/Net/TCPServerConnectionFactory.h>
#include <poco/Net/TCPServerConnection.h>
#include <poco/Net/StreamSocket.h>
#include <poco/Net/DatagramSocket.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include "CJGW_MesTcpServiceDataHandle.h"

namespace JGW
{
	CCJGW_AsyncWndMessage* gpAsyncWndMessage = NULL;

	void SendWindowMsg(const std::wstring& strMessage)
	{
		gpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strMessage.c_str());
	}

	void SendWindowMsgFormat(const wchar_t* szFormat,...)
	{
		std::wstring strLog;
		FORMAT_WSTRING(strLog,szFormat);
		gpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strLog.c_str());
	}

	class CustomConnection : public Poco::Net::TCPServerConnection
	{
	private:
		char* mszReceiveBuffer;
		Poco::Net::DatagramSocket _socket;
		//Poco::Net::SocketAddress _senderSocket;
		Poco::Timespan _span;
		static CCJGW_MesTcpServiceDataHandle mcMesTcpServiceDataHandle;

	public:
		CustomConnection(const Poco::Net::StreamSocket& socket) : TCPServerConnection(socket)/*,_senderSocket("127.0.0.1",PRS_ROAMING_GRAPH_UDP_SOCKET_PORT)*/,_span(250000)
		{
			mszReceiveBuffer = new char[TCP_RECEIVE_BUFFER_MAX_LENGTH];
			//_socket.bind(Poco::Net::SocketAddress(), true);
		}

		~CustomConnection()
		{
			delete []mszReceiveBuffer;
		}

		void run() override 
		{
			Poco::Net::StreamSocket& socket = this->socket();
			int nRecvLen = 0;
			std::string strBuffer;

			while (true)
			{
				//! receive msg
				try
				{
					nRecvLen = socket.receiveBytes(mszReceiveBuffer, TCP_RECEIVE_BUFFER_MAX_LENGTH);
					if (nRecvLen <= 0) break;
					strBuffer = std::string(mszReceiveBuffer,mszReceiveBuffer + nRecvLen);
					SendWindowMsgFormat(L"recv : %s",JGW_A2W(strBuffer,CP_UTF8).c_str());
					std::string strRecv = mcMesTcpServiceDataHandle.OnHandleMsg(strBuffer);
					if (!strRecv.empty()) socket.sendBytes(strRecv.c_str(),strRecv.length());
#if 0
					int nPos = strBuffer.find("\n");
					if (std::string::npos == nPos) nPos = strBuffer.find("\r");
					if (std::string::npos != nPos)
					{
						Poco::AutoPtr<CCJGW_PrsRoamingMessageNotification> pNf = new CCJGW_PrsRoamingMessageNotification(strBuffer.substr(0,nPos));
						mpsTcpServerParams->mPrsRoamingMessageNotificationQueue.enqueueNotification(pNf);	
						strBuffer = strBuffer.substr(nPos + 1);
					} 
#endif
				}
				catch (Poco::Exception ec)
				{
					OutputDebugStringA(ec.displayText().c_str());
					OutputDebugStringA("\n");
				}
			}
		}
	};

#if 0
	class CustomTCPServerConnection : public Poco::Net::TCPServerConnectionFactory
	{
	public:
		CustomTCPServerConnection()
		{

		}
	public:
		Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket) override {
			return new CustomConnection(socket);
		}
	};
#endif

	CCJGW_MesTcpServiceDataHandle CustomConnection::mcMesTcpServiceDataHandle;


	CCJGW_MesTcpServerManager::CCJGW_MesTcpServerManager(void)
	{
	}


	CCJGW_MesTcpServerManager::~CCJGW_MesTcpServerManager(void)
	{
	}

	bool CCJGW_MesTcpServerManager::StartMesTcpServerManager(CCJGW_AsyncWndMessage* pAsyncWndMessage,LPS_MESSericeInfo psMesServiceInfo)
	{
		try
		{
			gpAsyncWndMessage = pAsyncWndMessage;
			SendWindowMsgFormat(L"Start Mes Tcp Server Manager %s - %s",JGW_A2W(psMesServiceInfo->mstrMESServiceIP).c_str(),JGW_A2W(psMesServiceInfo->mstrMESServiceSocketPort).c_str());
			//msTcpServerParams.mptrPrsVendorLogStream.reset(new Poco::FileOutputStream(JGW_W2A(msTcpServerParams.mstrPrsVendorAppLog,CP_UTF8)));
			//if (!msTcpServerParams.mptrPrsVendorLogStream->good()) return false;
			//msTcpServerParams.mPrsRoamingMessageNotificationQueue.clear();
			//msTcpServerParams.mptrPrsVendorRoamingDataHandle.reset(new CCJGW_PrsVendorRoamingDataHandle);
#if 0
			mptrRoamingServer.reset(new Poco::Net::TCPServer(new CustomTCPServerConnection(), Poco::Net::ServerSocket(nSocketPort)));
#else
			gpAsyncWndMessage = pAsyncWndMessage;
			mptrRoamingServer.reset(new Poco::Net::TCPServer(new Poco::Net::TCPServerConnectionFactoryImpl<CustomConnection>(), Poco::Net::ServerSocket(Poco::Net::SocketAddress(psMesServiceInfo->mstrMESServiceIP,atoi(psMesServiceInfo->mstrMESServiceSocketPort.c_str())))));	
#endif
			mptrRoamingServer->start();
			return true;
		}
		catch (Poco::Exception& ec)
		{
			JGW_FormatWString(mstrErrorMessage,L"Start Mes Tcp Server Exception : %s",JGW_A2W(ec.displayText(),CP_UTF8).c_str());
			pAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,mstrErrorMessage.c_str());
		}
		return false;
	}

	bool CCJGW_MesTcpServerManager::StopMesTcpServerManager()
	{
		try
		{
			if (!!mptrRoamingServer)
			{
				mptrRoamingServer->stop();
				mptrRoamingServer.reset();
			}
		}
		catch (Poco::Exception& ec)
		{
			JGW_FormatWString(mstrErrorMessage,L"Stop Mes Tcp Server Exception : %s",JGW_A2W(ec.displayText(),CP_UTF8).c_str());
		}
		return true;
	}

	std::wstring CCJGW_MesTcpServerManager::GetErrorMessage() const
	{
		return mstrErrorMessage;
	}
}

