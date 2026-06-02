#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <UiLib/CAsyncMsgDialog.h>
#include <map>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#if 0
#include <JGW_Wlan/CJGW_WlanHandle.h>
#include <JGW_WindowsFuncPlugin/CJGW_CSerialComPort.h>
#include <JGW_WindowsFuncPlugin/CJGW_DonglesComPort.h>
#include <JGW_WindowsFuncPlugin/CJGW_SyncSocket.h>
#include <JGW_WindowsFuncPlugin/CJGW_ICMPSocket.h>
#include "CJGW_ReqCheckWlanConnectJson.h"
#include "CJGW_ResponseCheckWlanConnectJson.h"
#endif
#include "CJGW_WlanHandleThread.h"
namespace JGW
{
    //! 整个处理过程在另外一个线程中进行处理 service 只处理一个请求连接
    typedef struct
    {
        CCMessageThread* mpMessageThread;
        std::string mstrJson;
        boost::shared_ptr<boost::asio::ip::tcp::socket> mClientSock;
    }S_WLAN_SOCKET_PARAM,*PS_WLAN_SOCKET_PARAM;

	class CCJGW_WlanServiceThread : public CCMessageThread
	{
	public:
		CCJGW_WlanServiceThread(void);
		~CCJGW_WlanServiceThread(void);
    public:
        //! 停止SOCKET 
        void StopClientThread();
        //!
        void SendJsonCommand(const std::string& strJson,boost::shared_ptr<boost::asio::ip::tcp::socket> sock);
	private:
		void OnStartServiceThread(WPARAM wParam,LPARAM lParam);
	private:
		void StartAccept();
		void AcceptHandle(const boost::system::error_code& ec,boost::shared_ptr<boost::asio::ip::tcp::socket> sock);
		void ReadSocket(const boost::system::error_code& ec/*,boost::shared_ptr<vector<char>> str*/,size_t len,boost::shared_ptr<boost::asio::ip::tcp::socket> sock);
		void WriteSocket(const boost::system::error_code& ec,size_t txLenCompleted,boost::shared_ptr<boost::asio::ip::tcp::socket> sock);
        void WaitSocketReadWriteTimeOut(const boost::system::error_code e);
		//! close socket
		void CloseSocketService(const boost::system::error_code e);
	private:
#if 0
		std::string HandleRequest(const std::string& strRequest);
		std::string HandleResopnseScanWiFi(const std::string& strRequest);
		std::string HandleResopnseConnectWiFi(const std::string& strRequest);
		std::string HandleResponseDisConnectWiFi(const std::string& strRequest);
        std::string HandleRspCheckWlanConnectStatus(const std::string& strRequest);
        //! pumas
        std::string HandleRspConnectPumas(const std::string& strRequest);
        std::string HandleRspCommandPumas(const std::string& strRequest);
        std::string HandleRspDisConnectPumas(const std::string& strRequest);
        
    private:
        std::string GetWlanIpAddress(const std::string& strWlanGateWay,const std::string& strLocalAddress);
        bool ConnectWlan(CCJGW_ReqCheckWlanConnectJson repJson,CCJGW_ResponseCheckWlanConnectJson rspJson,int nTimeOut);
#endif
	protected:
		DECLARE_MYTHREAD_MESSAGE_MAP();
	private:
        bool mbConnected;
		CCJGW_AsyncWndMessage* mAsyncWndMessage;
		char* mTxBuf;
        char* mRxBuf;

        S_WLAN_SOCKET_PARAM msWlanSocketParam;

		boost::asio::io_service mios;
		boost::asio::ip::tcp::acceptor macceptorSocket;
        boost::asio::deadline_timer mdeadline_;
        CCJGW_WlanHandleThread mWlanHandleThread;

#if 0
		std::wstring mstrSSID;
		std::wstring mstrGUID;
		std::map<std::wstring,int> mapBss;


		CCJGW_WlanHandle wlan;
        CCJGW_CSerialComPort mSerialComPort;
        CCJGW_DonglesComPort mDonglesComPort;
        CCJGW_SyncSocket msyncSocket;
        CCJGW_ICMPSocket mIcmpSocket;
#endif
	};
}