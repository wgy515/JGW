#include "StdAfx.h"
#include "CJGW_TcpServerThread.h"

#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <UiLib/CAsyncMsgDialog.h>

#include "Common/Src/WaitFor.h"
#include "..\..\..\platform_include/JGW_MSG_ID_Define.h"
namespace JGW
{
    //! 当服务端为发送端，但是第一个包一定是客户端先发送
    CCJGW_TcpServerThread::CCJGW_TcpServerThread(const S_ThroughputTest_Confige& sThroughputTest) : m_Server(this),CCJGW_TcpStatsImplThread(sThroughputTest)
    {
    }


    CCJGW_TcpServerThread::~CCJGW_TcpServerThread(void)
    {
        //! StopTcpServer(NULL,NULL);
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_TcpServerThread,CCJGW_TcpStatsImplThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCJGW_TcpServerThread::StartTcpServer)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_CLEANUP,&CCJGW_TcpServerThread::StopTcpServer)      
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_TcpServerThread::StartTcpServer(WPARAM wParam,LPARAM lParam)
    {
        mbStartExit = false;
        mbExitComplete = false;
        ClearRecvSentCount();
        m_Server->SetSendPolicy(msThroughputTestConfig.meSendPolicy);
        m_Server->SetWorkerThreadCount(msThroughputTestConfig.mnServerThreadCount);
        m_Server->SetMaxConnectionCount(msThroughputTestConfig.mnServerMaxConnSocketCount);
        if (m_Server->Start(msThroughputTestConfig.mstrIPAddress.c_str(),msThroughputTestConfig.mnSocketPort))
        {
            SendMessageToHwnd(L"start server ok\n");
            msThroughputTestConfig.mpAsyncWndMessage->PutAsyncMessage(WM_HWND_TEST_SATRT_MSG);
            ClearRecvSentCount();
        }
        else
        {
            SendMessageToHwnd(L"start server fail\n");
            msThroughputTestConfig.mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
        }
    }

    void CCJGW_TcpServerThread::StopTcpServer(WPARAM wParam,LPARAM lParam)
    {     
        mbStartExit = true;
        if (m_Server->HasStarted()) m_Server->Stop();
        mbExitComplete = true;
    }

    EnHandleResult CCJGW_TcpServerThread::OnPrepareListen(ITcpServer* pSender, SOCKET soListen)
    {
        std::wstring strTemp;
        JGW_FormatWString(strTemp,_T("bind address: %s#%d\n"), msThroughputTestConfig.mstrIPAddress.c_str(),msThroughputTestConfig.mnSocketPort);
        SendMessageToHwnd(strTemp.c_str());
        return HR_OK;
    }
    
    EnHandleResult CCJGW_TcpServerThread::OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient)
    {
        std::wstring strTemp;
        JGW_FormatWString(strTemp,_T("OnAccept: %d\n"), dwConnID);
        SendMessageToHwnd(strTemp.c_str());
        return HR_OK;
    }

    EnHandleResult CCJGW_TcpServerThread::OnHandShake(ITcpServer* pSender, CONNID dwConnID)
    {
        return HR_OK;
    }
    //! += 20 + 12 +4
    EnHandleResult CCJGW_TcpServerThread::OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
    {
      //  mdwRx += iLength/* + 12 + 20 + 4*/;
#if (_WIN32_WINNT <= _WIN32_WINNT_WS03)
        ::InterlockedExchangeAdd((volatile LONG*)&m_llTotalReceived, iLength);
#else
        ::InterlockedExchangeAdd64(&m_llTotalReceived, iLength);
#endif
        if (E_SEND_TYPE == msThroughputTestConfig.meTranservicesType && !mbStartExit)
        {
            pSender->Send(dwConnID,pData,iLength);
            if (msThroughputTestConfig.mnIntervTime > 0) ::WaitWithMessageLoop(msThroughputTestConfig.mnIntervTime);
        }

        return HR_OK;
    }

    EnHandleResult CCJGW_TcpServerThread::OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
    {
       // mdwTx += iLength /*+ 12 + 20 + 4*/;
#if (_WIN32_WINNT <= _WIN32_WINNT_WS03)
        ::InterlockedExchangeAdd((volatile LONG*)&m_llTotalSent, iLength);
#else
        ::InterlockedExchangeAdd64(&m_llTotalSent, iLength);
#endif
        if (E_SEND_TYPE == msThroughputTestConfig.meTranservicesType && !mbStartExit)
        {
            pSender->Send(dwConnID,pData,iLength);
            if (msThroughputTestConfig.mnIntervTime > 0) ::WaitWithMessageLoop(msThroughputTestConfig.mnIntervTime);
        }

        return HR_OK;
    }

    EnHandleResult CCJGW_TcpServerThread::OnShutdown(ITcpServer* pSender)
    {
        return HR_OK;
    }

    EnHandleResult CCJGW_TcpServerThread::OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
    {
        std::wstring strTemp;
        JGW_FormatWString(strTemp,_T("OnClose: %d\n"), dwConnID);
        SendMessageToHwnd(strTemp.c_str());
        return HR_OK;
    }
}