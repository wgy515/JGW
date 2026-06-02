#pragma once
#include "CJGW_ThroughputTestTool2Controller.h"
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JGW_WindowsFuncPlugin/CJGW_ICMPSocket.h>

#include "CJGW_Iperf3Command.h"
#include <JGW_SSHPlugin/CJGW_SSHShell.h>
#include <poco/Event.h>

#include "CJGW_MobilePhoneClientSocketThread.h"
#include "CJGW_IPQSshThread.h"

namespace JGW
{
    class CCJGW_ThroughputTestThread : public CCMessageThread
    {
    public:
        CCJGW_ThroughputTestThread(S_TestParams* psTestParams,CCJGW_AsyncWndMessage* pAsyncWndMessage);
        ~CCJGW_ThroughputTestThread(void);
    public:
        //! 停止吞吐率测试线程
        void StopThroughputTestThread();
    private:
        //! 运行吞吐率测试
        void OnRunTestThread(WPARAM wParam,LPARAM lParam);
        //! iperf server 流程处理
        void IperfServerFlowProcessing();
        //! iperf client 流程处理
        void IperfClientFlowProcessing();
    private:
        //! iperf3 测试速率回调
       //static void Iperf3TestSpeedCallBack(float fSpeedMbitssec,void* pContext);
        //! iperf3 原始测试速率回调
        static void Iperf3RawTestLogCallbackFunc(const std::wstring& strRawLog,void* pContext);
        //! 检查网络连接
        bool CheckNetworkConnection(const std::wstring& strIPAddr,const std::wstring& strSourceAddr = L"",int nTestTimeSec = 30,int nTestContinuousCount = 2);
        //! 发送测试消息到窗口
        void SendWindowMsg(const std::wstring& strMessage);
        void SendWindowMsg(const wchar_t* szFormat,...);
    private:
        static void ReadWriteSSHThreadCallback(void* pThreadContext);
    private:
        DECLARE_MYTHREAD_MESSAGE_MAP();
    private:
        bool mbRunTestThread;
        bool mbStopIperf3Server;
        S_TestParams* mpsTestParams;
        CCJGW_AsyncWndMessage*  mpAsyncWndMessage;
        CCJGW_ICMPSocket mIcmpSocket;
        CCJGW_Iperf3Command mcIperf3Command;
		Poco::Event m11ADConnectEventt;
		CCJGW_MobilePhoneClientSocketThread mcMobilePhoneClientSocketThread;
        CCJGW_IPQSshThread mcIPQSshThread;
    };
}
