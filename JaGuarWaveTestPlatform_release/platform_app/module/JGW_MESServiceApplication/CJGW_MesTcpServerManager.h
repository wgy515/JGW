#pragma once
#include <string>
#include <UiLib/CAsyncMsgDialog.h>
#include "JGW_MESServiceApplication_Define.h"
#include <Poco/Net/TCPServer.h>
#include <memory>
#include <Poco/FileStream.h>
#include <Poco/File.h>
#include <Poco/NotificationQueue.h>

namespace JGW
{
	class CCJGW_MesTcpServerManager
	{
	public:
		CCJGW_MesTcpServerManager(void);
		~CCJGW_MesTcpServerManager(void);
	public:
		//! 开启PRS VENDOR ROAMING TCP Server Socket
		bool StartMesTcpServerManager(CCJGW_AsyncWndMessage* pAsyncWndMessage,LPS_MESSericeInfo psMesServiceInfo);
		//! 停止PRS VENDOR ROAMING TCP Server Socket
		bool StopMesTcpServerManager();
		//! 获取异常消息
		std::wstring GetErrorMessage() const;
	private:
		std::wstring mstrErrorMessage;
		std::unique_ptr<Poco::Net::TCPServer> mptrRoamingServer;
	};
}

