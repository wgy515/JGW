#include "StdAfx.h"
#include "CJGW_TcpStatsImplThread.h"
#include <UiLib/CAsyncMsgDialog.h>
#include "..\..\..\platform_include/JGW_MSG_ID_Define.h"
namespace JGW
{
    CCJGW_TcpStatsImplThread::CCJGW_TcpStatsImplThread(const S_ThroughputTest_Confige& sThroughputTest) : msThroughputTestConfig(sThroughputTest),mbExitComplete(true),mbStartExit(false)
    {
    }


    CCJGW_TcpStatsImplThread::~CCJGW_TcpStatsImplThread(void)
    {
    }

    void CCJGW_TcpStatsImplThread::InitStartTest()
    {
        mbStartExit = false;
        mbExitComplete = false;
        ClearRecvSentCount();
    }

    void CCJGW_TcpStatsImplThread::ExitTestThread()
    {
        mbStartExit = true;
    }

    void CCJGW_TcpStatsImplThread::CheckExitComplete()
    {
        while (!mbExitComplete) Sleep(10);
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_TcpStatsImplThread,CCMessageThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_TcpStatsImplThread::ClearRecvSentCount()
    {
#if (_WIN32_WINNT <= _WIN32_WINNT_WS03)
        ::InterlockedExchange((volatile LONG*)&m_llTotalReceived, 0);
#else
        ::InterlockedExchange64(&m_llTotalReceived, 0);
#endif
#if (_WIN32_WINNT <= _WIN32_WINNT_WS03)
        ::InterlockedExchange((volatile LONG*)&m_llTotalSent, 0);
#else
        ::InterlockedExchange64(&m_llTotalSent, 0);
#endif
    }


    LONGLONG CCJGW_TcpStatsImplThread::GetTotalReceived()
    {
#if (_WIN32_WINNT <= _WIN32_WINNT_WS03)
        return  ::InterlockedExchange((volatile LONG*)&m_llTotalReceived, 0);
#else
        return ::InterlockedExchange64(&m_llTotalReceived, 0);
#endif
    }

    LONGLONG CCJGW_TcpStatsImplThread::GetTotalSent()
    {
#if (_WIN32_WINNT <= _WIN32_WINNT_WS03)
        return  ::InterlockedExchange((volatile LONG*)&m_llTotalSent, 0);
#else
        return ::InterlockedExchange64(&m_llTotalSent, 0);
#endif
    }

    void CCJGW_TcpStatsImplThread::SendMessageToHwnd(const wchar_t* strMsg)
    {
        msThroughputTestConfig.mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strMsg);
    }
}
