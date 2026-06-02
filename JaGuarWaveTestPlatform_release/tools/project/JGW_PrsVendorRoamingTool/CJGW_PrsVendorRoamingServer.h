#pragma once

#include "PrsVendorRoamingDataDefine.h"
#include "CJGW_PrsVendorRoamingDataHandle.h"

#include <Poco/Net/TCPServer.h>
#include <memory>
#include <Poco/FileStream.h>
#include <Poco/File.h>
#include <Poco/NotificationQueue.h>

namespace JGW
{
    typedef struct
    {
        std::shared_ptr<CCJGW_PrsVendorRoamingDataHandle> mptrPrsVendorRoamingDataHandle;
        //std::shared_ptr<Poco::FileOutputStream> mptrPrsVendorLogStream;
        //! prs_vendor_app.log
        //std::wstring mstrPrsVendorAppLog;
        //!
        Poco::NotificationQueue mPrsRoamingMessageNotificationQueue;
    }S_TCPServer_Params,*LPS_TCPServer_Params;

    class CCJGW_PrsRoamingMessageNotification : public Poco::Notification
    {
    public:
        CCJGW_PrsRoamingMessageNotification(const std::string& strPrsRoamingMessage);
        const std::string& GetPrsRoamingMessage() const;
    private:
        std::string mstrPrsRoamingMessage;
    };


	class CCJGW_PrsVendorRoamingServer
	{
	public:
		CCJGW_PrsVendorRoamingServer(void);
		~CCJGW_PrsVendorRoamingServer(void);
	public:
        //! 开启PRS VENDOR ROAMING TCP Server Socket
		bool StartPrsVendorRoamingServer(Poco::UInt16 nSocketPort = PRS_VENDOR_APP_TCP_SOCKET_PORT);
        //! 停止PRS VENDOR ROAMING TCP Server Socket
		bool StopPrsVendorRoamingServer();
        //! 获取异常消息
        std::wstring GetErrorMessage() const;

        Poco::NotificationQueue* GetNotificationQueue();
	private:
        std::wstring mstrErrorMessage;
		std::unique_ptr<Poco::Net::TCPServer> mptrRoamingServer;
        S_TCPServer_Params msTcpServerParams;
		//CCJGW_PrsVendorRoamingDataHandle mcPrsVendorRoamingDataHandle;
	};
}
