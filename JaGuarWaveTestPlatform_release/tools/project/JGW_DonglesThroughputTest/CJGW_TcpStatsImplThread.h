#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include "JGW_DonglesThroughputTest_Define.h"

namespace JGW
{
    class CCJGW_TcpStatsImplThread : public CCMessageThread
    {
    public:
        CCJGW_TcpStatsImplThread(const S_ThroughputTest_Confige& sThroughputTest);
        virtual ~CCJGW_TcpStatsImplThread(void);
    public:
        //! 初始化测试环境
        void InitStartTest();
        //! 获取接收
        LONGLONG GetTotalReceived();
        //! 获取发送
        LONGLONG GetTotalSent();
        //! 获取当前线程类型 - > 客户端或者服务端
        virtual E_SOCKET_TERIMALTYPE GetSocketThreadType() = 0;
        //! 清空发送和接收统计
        void ClearRecvSentCount();
        //! 发送消息到窗口
        void SendMessageToHwnd(const wchar_t* strMsg);
        //!
        void CheckExitComplete();
        //!
        void ExitTestThread();
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    protected:
        volatile LONGLONG m_llTotalReceived;
        volatile LONGLONG m_llTotalSent;
        bool mbStartExit;
        bool mbExitComplete;
        const S_ThroughputTest_Confige& msThroughputTestConfig;
    };
}


