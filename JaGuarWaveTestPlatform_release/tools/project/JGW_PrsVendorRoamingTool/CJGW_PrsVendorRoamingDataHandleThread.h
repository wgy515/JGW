#pragma once
#include "CJGW_PrsVendorRoamingToolDlgController.h"

#include "CJGW_PrsVendorRoamingDataHandle.h"
#include "CJGW_AsyncWndMessageHelp.h"

#include <JGW_WindowsFuncPlugin/CJGW_SimpleThread.h>
#include <Poco/FileStream.h>
#include <Poco/NotificationQueue.h>
#include <poco/Net/DatagramSocket.h>


namespace JGW
{
    class CCJGW_PrsVendorRoamingDataHandleThread
    {
    public:
        CCJGW_PrsVendorRoamingDataHandleThread(void);
        ~CCJGW_PrsVendorRoamingDataHandleThread(void);
    public:
        //! 启动PRS Vendor Roming 数据处理线程
        bool StartPrsVendorRoamingDataHandleThread(const std::wstring& strLogFolder,Poco::NotificationQueue* pPrsRoamingMessageNotificationQueue,bool bIsSendRoamingGraphMsg,CCJGW_AsyncWndMessage* pAsyncWndMessage);
        //! 获取错误消息
        std::wstring GetErrorMessage() const;
        //! 停止PRS Vendor Roming 数据处理线程
        bool StopPrsVendorRoamingDataHandleThread();
    private:
        //! 
        static void OnRuningPrsVendorRoamingDataHandleThreadCallback(void *lpParam);
        //! 
        void OnRuningPrsVendorRoamingDataHandleThreadThread();
    private:
        void WritePrsVendorAppLog(const std::string& strLog);
    private:
        bool mbIsSendRoamingGraphMsg;
        bool mbRuningThread;
        std::shared_ptr<CCJGW_AsyncWndMessageHelp> mptrAsyncWndMessageHelp;
        Poco::NotificationQueue* mpPrsRoamingMessageNotificationQueue;
        Poco::Net::SocketAddress _senderSocket;
        Poco::Net::DatagramSocket _socket;
        Poco::Timespan _span;
        std::shared_ptr<Poco::FileOutputStream> mptrPrsVendorLogStream;
        std::wstring mstrErrorMessage;
        CCJGW_SimpleThread mcPrsVendorRoamingDataHandleThread;
        CCJGW_PrsVendorRoamingDataHandle mcPrsVendorRoamingDataHandle;
    };
}

