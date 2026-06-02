#include "StdAfx.h"
#include "CJGW_PrsVendorRoamingServer.h"
#include <poco/Net/ServerSocket.h>
#include <poco/Net/TCPServerConnectionFactory.h>
#include <poco/Net/TCPServerConnection.h>
#include <poco/Net/StreamSocket.h>
#include <poco/Net/DatagramSocket.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <regex>

#include "PrsVendorRoamingDataDefine.h"
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>


namespace JGW
{
    //void WritePrsVendorAppLog(Poco::FileOutputStream& ptrFileOutputStram,const std::string& strLog);

    CCJGW_PrsRoamingMessageNotification::CCJGW_PrsRoamingMessageNotification(const std::string& strPrsRoamingMessage) : mstrPrsRoamingMessage(strPrsRoamingMessage)
    {

    }

    const std::string& CCJGW_PrsRoamingMessageNotification::GetPrsRoamingMessage() const
    {
        return mstrPrsRoamingMessage;
    }



	class CustomConnection : public Poco::Net::TCPServerConnection
	{
	private:
		char* mszReceiveBuffer;
        LPS_TCPServer_Params mpsTcpServerParams;
		Poco::Net::DatagramSocket _socket;
		//Poco::Net::SocketAddress _senderSocket;
		Poco::Timespan _span;
	public:
		CustomConnection(LPS_TCPServer_Params psTcpServerParams,const Poco::Net::StreamSocket& socket) : TCPServerConnection(socket)/*,_senderSocket("127.0.0.1",PRS_ROAMING_GRAPH_UDP_SOCKET_PORT)*/,_span(250000),mpsTcpServerParams(psTcpServerParams)
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
					strBuffer += std::string(mszReceiveBuffer,mszReceiveBuffer + nRecvLen);

					int nPos = strBuffer.find("\n");
					if (std::string::npos == nPos) nPos = strBuffer.find("\r");
					if (std::string::npos != nPos)
					{
						Poco::AutoPtr<CCJGW_PrsRoamingMessageNotification> pNf = new CCJGW_PrsRoamingMessageNotification(strBuffer.substr(0,nPos));
						mpsTcpServerParams->mPrsRoamingMessageNotificationQueue.enqueueNotification(pNf);	
						strBuffer = strBuffer.substr(nPos + 1);
					} 
				}
				catch (Poco::Exception ec)
				{
                    OutputDebugStringA(ec.displayText().c_str());
                    OutputDebugStringA("\n");
				}
				//! send msg to roaming_graph.py
				//try
				//{
				//	if (_socket.poll(_span, Poco::Net::Socket::SELECT_WRITE))
				//	{
				//		_socket.sendTo(mszReceiveBuffer,nRecvLen > PYTHON_ROAMING_GRAPH_PY_RECV_BUFFER_MAX_LENGTH ? PYTHON_ROAMING_GRAPH_PY_RECV_BUFFER_MAX_LENGTH : nRecvLen,_senderSocket);
				//	}
				//}
				//catch (Poco::Exception ec)
				//{
    //                OutputDebugStringA(ec.message().c_str());
    //                OutputDebugStringA("\n");
				//}
#if 0
				//! "classId": 13, "serviceId": 4, 
				if (0 == JGW_StrComparenoCaseStr(CONNECT_MSG,mszReceiveBuffer) || 0 == JGW_StrComparenoCaseStr(DISCONNECT_MSG,mszReceiveBuffer))
				{
					continue;
				}
				int nClassID = 0,nServiceID = 0;
				//! "{field}": (-?\d+)
				const char* pClassID = JGW_StrCaseStr(mszReceiveBuffer,"\"classId\":");
				if (NULL == pClassID)
				{
					continue ;
				}
				nClassID = atoi(pClassID + strlen("\"classId\":"));

				const char* pServiceID = JGW_StrCaseStr(mszReceiveBuffer,"\"serviceId\":");
				if (NULL == pServiceID)
				{
					continue ;
				}
				nServiceID = atoi(pServiceID + strlen("\"serviceId\":"));

				try
				{
					HandleNotificationHelper(nClassID,nServiceID,std::string(mszReceiveBuffer,mszReceiveBuffer + nRecvLen));
				}
				catch (std::string& strError)
				{
					OutputDebugStringA(strError.c_str());
					OutputDebugStringA("\n");
				}
				catch (...)
				{
					OutputDebugStringA("unknown exception\n");
				}		
#endif
			}
		}
#if 0
		//! handle_notification_helper
		void HandleNotificationHelper(int nClassID,int nServiceID,const std::string& strData)
		{
			if (DEVICE_HOST_CLASS_ID == nClassID)
			{
				mpsTcpServerParams->mptrPrsVendorRoamingDataHandle->OnHandleDeviceHostClass(nServiceID,strData);
			}
			else if (DRIVER_CLASS_ID == nClassID)
			{
				mpsTcpServerParams->mptrPrsVendorRoamingDataHandle->OnHandleDriverClass(nServiceID,strData);
			}
			else if (ROAMING_CLASS_ID == nClassID)
			{
				mpsTcpServerParams->mptrPrsVendorRoamingDataHandle->OnHandleRoamingClass(nServiceID,strData);
			}
			else
			{
				printf("{Colors.ERROR}[ERROR] Unknown classId: {class_id}{Colors.END}");
			}
		}
#endif
	};


    class CustomTCPServerConnection : public Poco::Net::TCPServerConnectionFactory
    {
    public:
        CustomTCPServerConnection(LPS_TCPServer_Params psTcpServerParams) : mpsTcpServerParams(psTcpServerParams){}
    public:
        Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket) override {
            return new CustomConnection(mpsTcpServerParams,socket);
        }
    private:
        LPS_TCPServer_Params mpsTcpServerParams;
    };

	CCJGW_PrsVendorRoamingServer::CCJGW_PrsVendorRoamingServer(void)
	{
	}


	CCJGW_PrsVendorRoamingServer::~CCJGW_PrsVendorRoamingServer(void)
	{
	}

    std::wstring CCJGW_PrsVendorRoamingServer::GetErrorMessage() const
    {
        return mstrErrorMessage;
    }
#if 0
    void WritePrsVendorAppLog(Poco::FileOutputStream& ptrFileOutputStram,const std::string& strLog)
    {
        static CCJGW_CriticalSectionLock gPrsLogCriticalSectionLock;
        try
        {
            if (ptrFileOutputStram.good())
            {
                CCJGW_CriticalSectionAutoLock autoLock(gPrsLogCriticalSectionLock);
                ptrFileOutputStram << strLog << std::endl;
                OutputDebugStringA(strLog.c_str());
                OutputDebugStringA("\n");
            }
        }
        catch (Poco::Exception& ec)
        {
        	OutputDebugStringW(JGW_GetFormatWString(L"Write Prs Vendor App Log Exception : %s\n",JGW_A2W(ec.displayText(),CP_UTF8)).c_str());
        }
    }
#endif

	bool CCJGW_PrsVendorRoamingServer::StartPrsVendorRoamingServer(Poco::UInt16 nSocketPort /* = PRS_VENDOR_APP_TCP_SOCKET_PORT */)
	{
        //! log folder
        //JGW_FormatWString(msTcpServerParams.mstrPrsVendorAppLog,L"%s\\prs_vendor_app_%u.log",mstrLogFolder.c_str(),JGW_GetTimeOfDay());
		try
		{
            //msTcpServerParams.mptrPrsVendorLogStream.reset(new Poco::FileOutputStream(JGW_W2A(msTcpServerParams.mstrPrsVendorAppLog,CP_UTF8)));
            //if (!msTcpServerParams.mptrPrsVendorLogStream->good()) return false;
            msTcpServerParams.mPrsRoamingMessageNotificationQueue.clear();
            msTcpServerParams.mptrPrsVendorRoamingDataHandle.reset(new CCJGW_PrsVendorRoamingDataHandle);
			mptrRoamingServer.reset(new Poco::Net::TCPServer(new CustomTCPServerConnection(&msTcpServerParams), Poco::Net::ServerSocket(nSocketPort)));
			mptrRoamingServer->start();
			return true;
		}
		catch (Poco::Exception& ec)
		{
            JGW_FormatWString(mstrErrorMessage,L"Start Romming TCP Server Exception : %s",JGW_A2W(ec.displayText(),CP_UTF8).c_str());
		}
		return false;
	}

    bool CCJGW_PrsVendorRoamingServer::StopPrsVendorRoamingServer()
    {
        //msTcpServerParams.mptrPrsVendorLogStream.reset();
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
            JGW_FormatWString(mstrErrorMessage,L"Stop Romming TCP Server Exception : %s",JGW_A2W(ec.displayText(),CP_UTF8).c_str());
        }
        return true;
    }

    Poco::NotificationQueue* CCJGW_PrsVendorRoamingServer::GetNotificationQueue()
    {
        return &msTcpServerParams.mPrsRoamingMessageNotificationQueue;
    }
}

