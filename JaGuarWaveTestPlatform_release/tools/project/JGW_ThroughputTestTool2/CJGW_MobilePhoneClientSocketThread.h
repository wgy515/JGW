#pragma once
#include <JGW_WindowsFuncPlugin/CJGW_SimpleThread.h>
#include "CJGW_ThroughputTestTool2Controller.h"
#include <JGW_WindowsFuncPlugin/CJGW_AsyncSocket.h>
#include <JGW_WindowsFuncPlugin/CJGW_ICMPSocket.h>
#include <poco/Event.h>

namespace JGW
{
	class CCJGW_MobilePhoneClientSocketThread
	{
	public:
		CCJGW_MobilePhoneClientSocketThread(bool& bRunTestThread,S_TestParams* psTestParams,CCJGW_AsyncWndMessage* pAsyncWndMessage);
		~CCJGW_MobilePhoneClientSocketThread(void);
	public:
		bool StartMobilePhoneClientSocketThread();
		void StopMobilePhoneClientSocketThread();
        void TerminateMobilePhoneClientSocketThread();
	private:
		//! 读取GPS以及速率回调请求
		static void OnReadWriteClientSocketCallback(void *lpParam);
		//! 读写客户端SOCKET线程回调
		void ReadWriteClientSocketThreadCallback();
		//! 读SOCKET内容
		bool ReadClientSocket(std::string& strMessage);
		//! 检查网络连接
		bool CheckNetworkConnection(const std::wstring& strIPAddr,const std::wstring& strSourceAddr = L"",int nTestTimeSec = 30,int nTestContinuousCount = 2);
		//! 处理SOCKET消息
		void HandleClientSocketMessage(const std::string& strMsg);
		//! 
		void ParseClientSocketMsgJson(const std::string& strJsonValue);
		//!
		void HandleGspInfoJson(const std::string& strJsonValue);
		//!
		void HandleMessageInfoJson(const std::string& strJsonValue);
	private:
		//! 发送测试消息到窗口
		void SendWindowMsg(const std::wstring& strMessage);
		void SendWindowMsg(const wchar_t* szFormat,...);
	private:
		char* mszReadBuffer;
		bool& mbRunTestThread;
		std::string mstrReadContectMsg;
		S_TestParams* mpsTestParams;
		CCJGW_AsyncWndMessage* mpAsyncWndMessage;
		CCJGW_ICMPSocket mIcmpSocket;
		Poco::Event mStopEvent;
		CCJGW_AsyncSocket mcAsyncSocket;
		CCJGW_SimpleThread mcClientSocketThread;
	};
}


