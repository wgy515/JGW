#pragma once
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include <JGW_WindowsFuncPlugin/CJGW_SimpleThread.h>


namespace JGW
{
	//! 通过roaming.log日志文件，模拟prs_vendor_app数据转发
	class CCJGW_PrsRoamingLogDebugSocket
	{
	public:
		CCJGW_PrsRoamingLogDebugSocket(void);
		~CCJGW_PrsRoamingLogDebugSocket(void);
	public:
		bool StartPrsRoamingLogDebugSocket(const std::wstring& strRoamingLogPath);
		void StopPrsRoamingLogDebugSocket();
		void OnHandlePrsRoamingLogDebugSocket();
	public:
		std::wstring GetErrorMessage();
	private:
		bool mbIsStop;
		char* mszLogBuffer;
		std::wstring mstrRoamingLogPath;
		std::wstring mstrErrorMessage;
		CCJGW_SimpleThread mcPrsRoamingLogDebugSocketThread;
		
	};
}


