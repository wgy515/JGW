#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include "PrsVendorRoamingDataDefine.h"
#include "CJGW_PrsVendorRoamingToolDlgController.h"
#include "CJGW_Iperf3Command.h"
#include "CJGW_PingTestThread.h"
#include "CJGW_PrsVendorRoamingServer.h"
#include "CJGW_PrsRoamingLogDebugSocket.h"
#include "CJGW_PrsVendorRoamingDataHandleThread.h"
#include "CJGW_IPQSshThread.h"
#include "CJGW_PrsRoamingGraphUDPServer.h"
namespace JGW
{
    class CCJGW_PrsRoamingTestThread : public CCMessageThread
    {
    public:
        CCJGW_PrsRoamingTestThread(S_TestParams* psTestParams,CCJGW_AsyncWndMessage* pAsyncWndMessage);
        ~CCJGW_PrsRoamingTestThread(void);
    public:
        //! 停止吞吐率测试线程
        void StopThroughputTestThread();
        //!
        void SendCEF3DataUDPMessage(const std::string& strMsg);
    private:
        void OnRunTestThread(WPARAM wParam,LPARAM lParam);
    private:
        //! iperf3 原始测试速率回调
        static void Iperf3RawTestLogCallbackFunc(const std::wstring& strRawLog,void* pContext);
        //! iperf server 流程处理
        void IperfServerFlowProcessing();
        //! iperf client 流程处理
        void IperfClientFlowProcessing();
        //! 发送测试消息到窗口
        void SendWindowMsg(const std::wstring& strMessage);
        void SendWindowMsg(const wchar_t* szFormat,...);
    private:
        Poco::Net::SocketAddress _senderSocket;
        Poco::Net::DatagramSocket _socket;
    private:
        DECLARE_MYTHREAD_MESSAGE_MAP();
    private:
        bool mbRunTestThread;
        bool mbStopIperf3Server;
        S_TestParams* mpsTestParams;
        CCJGW_AsyncWndMessage*  mpAsyncWndMessage;
        CCJGW_Iperf3Command mcIperf3Command;
        CCJGW_PingTestThread mcPingTestThread;
        CCJGW_PrsVendorRoamingServer mcPrsVendorRoamingServer;
        CCJGW_PrsRoamingLogDebugSocket mcPrsRoamingLogDebugSocket;
        CCJGW_PrsVendorRoamingDataHandleThread mcPrsVendorRoamingDataHandleThread;
        CCJGW_IPQSshThread mcIPQSshThread;
        CCJGW_PrsRoamingGraphUDPServer mcPrsRoamingGraphUDPServer;
        //Poco::Event m11ADConnectEventt;
        //CCJGW_MobilePhoneClientSocketThread mcMobilePhoneClientSocketThread;
        //CCJGW_IPQSshThread mcIPQSshThread;

    };
}
