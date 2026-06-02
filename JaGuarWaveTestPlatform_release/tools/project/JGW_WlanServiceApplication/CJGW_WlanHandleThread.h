#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JGW_Wlan/CJGW_WlanInterfaceImpl.h>
#include <JGW_WindowsFuncPlugin/CJGW_CSerialComPort.h>
#include <JGW_WindowsFuncPlugin/CJGW_DonglesComPort.h>
#include <JGW_WindowsFuncPlugin/CJGW_SyncSocket.h>
#include <JGW_WindowsFuncPlugin/CJGW_ICMPSocket.h>
#include "CJGW_ReqCheckWlanConnectJson.h"
#include "CJGW_ResponseCheckWlanConnectJson.h"

#include <map>
#include <string>

//! 分发处理WLAN SERVICE线程 收到的请求，然后将请求回送回去

namespace JGW
{
    class CCJGW_WlanHandleThread : public CCMessageThread
    {
    public:
        CCJGW_WlanHandleThread(void);
        ~CCJGW_WlanHandleThread(void);
    private:
        //! 开始分发请求
        void OnStartHandleRequest(WPARAM wParam,LPARAM lParam);
    private:
        std::string HandleRequest(const std::string& strRequest);
        std::string HandleResopnseScanWiFi(const std::string& strRequest);
        std::string HandleResopnseConnectWiFi(const std::string& strRequest);
        std::string HandleResponseDisConnectWiFi(const std::string& strRequest);
        std::string HandleRspCheckWlanConnectStatus(const std::string& strRequest);
        //! pumas
        std::string HandleRspConnectPumas(const std::string& strRequest);
        std::string HandleRspCommandPumas(const std::string& strRequest);
        std::string HandleRspDisConnectPumas(const std::string& strRequest);
        //! shell execute
        std::string HandleRspShellExecute(const std::string& strRequest);
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP();
    private:
        bool ConnectWlanToSSID(const std::string& strSSID,const std::string& strPassword,int nTimeOutSec = 30);
        std::string GetWlanIpAddress(const std::string& strWlanGateWay,const std::string& strLocalAddress);
        bool ConnectWlan(CCJGW_ReqCheckWlanConnectJson repJson,CCJGW_ResponseCheckWlanConnectJson rspJson,int nTimeOut);
    private:
        size_t mScanTime;
        std::wstring mstrConnectSSID;
        std::string mstrErrorMsg;
        std::map<std::string,int> mapBss;

        CCJGW_CSerialComPort mSerialComPort;
        CCJGW_DonglesComPort mDonglesComPort;
        CCJGW_SyncSocket msyncSocket;
        CCJGW_ICMPSocket mIcmpSocket;

       
    };
}