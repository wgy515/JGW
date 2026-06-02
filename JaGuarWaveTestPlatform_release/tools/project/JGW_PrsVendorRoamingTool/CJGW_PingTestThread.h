#pragma once
#include <JGW_WindowsFuncPlugin/CJGW_SimpleThread.h>
#include <JGW_WindowsFuncPlugin/CJGW_ICMPSocket.h>
#include "CJGW_PrsVendorRoamingToolDlgController.h"

namespace JGW
{
    class CCJGW_PingTestThread
    {
    public:
        CCJGW_PingTestThread(void);
        ~CCJGW_PingTestThread(void);
    public:
        //! 开启PING测试线程
        bool StartPingTestThread(const std::string& strPingAddr,CCJGW_AsyncWndMessage* pAsyncWndMessage);
        //! 停止开启PING测试线程
        void StopPingTestThread();
        //!
        bool GetPingSucessful();
    private:
        //! 发送测试消息到窗口
        void SendWindowMsg(const std::wstring& strMessage);
        void SendWindowMsg(const wchar_t* szFormat,...);
        //! 
        static void OnRuningPingTestThreadCallback(void *lpParam);
        //1
        void OnRuningPingTestThread();
    private:
        bool mbPingSucessful;
        bool mbRuningPingTestThread;
        CCJGW_AsyncWndMessage* mpAsyncWndMessage;
        CCJGW_ICMPSocket mIcmpSocket;
        CCJGW_SimpleThread mcPingTestThread;
        std::string mstrPingAddr;
    };
}

