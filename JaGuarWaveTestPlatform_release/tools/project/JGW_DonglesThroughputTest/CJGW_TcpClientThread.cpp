#include "StdAfx.h"
#include "CJGW_TcpClientThread.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <UiLib/CAsyncMsgDialog.h>
#include "..\..\..\platform_include/JGW_MSG_ID_Define.h"
namespace JGW
{
    CCJGW_TcpClientThread::CCJGW_TcpClientThread(const S_ThroughputTest_Confige& sThroughputTest) : CCJGW_TcpStatsImplThread(sThroughputTest)
    {
    }


    CCJGW_TcpClientThread::~CCJGW_TcpClientThread(void)
    {
        //! if (!mbExitComplete) StopClient(NULL,NULL);
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_TcpClientThread,CCJGW_TcpStatsImplThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCJGW_TcpClientThread::StartClient)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_CLEANUP,&CCJGW_TcpClientThread::StopClient) 
    END_MYTHREAD_MESSAGE_MAP()


    void CCJGW_TcpClientThread::StopClient(WPARAM wParam,LPARAM lParam)
    {
        mbStartExit = true;
        for(size_t i = 0; i < m_vtClients.Size(); i++)
        {
            CTcpClientPtr* pSocket = m_vtClients[i];
            if ((*pSocket)->HasStarted()) (*pSocket)->Stop();
        }
        //! 
        ::WaitWithMessageLoop(100L);
        //! 清空
        m_vtClients.Clear();
        //! 退出完成的标记
        mbExitComplete = true;
    }

    void CCJGW_TcpClientThread::StartClient(WPARAM wParam,LPARAM lParam)
    {
        InitStartTest();
        m_sendBuffer.Malloc(msThroughputTestConfig.mnDataLength, true);

        //! 连接Server Socket
        for(size_t i = 0; i < msThroughputTestConfig.mnSocketCount; i++)
        {
            smart_simple_ptr<CTcpClientPtr> pSocket = new CTcpClientPtr(this);
            if(!(*pSocket)->Start(msThroughputTestConfig.mstrIPAddress.c_str(), msThroughputTestConfig.mnSocketPort, FALSE)) 
            {
                SendMessageToHwnd(JGW_GetFormatWString(L"Connect Server Socket Fail %s",JGW_GetSystemErrorString(GetLastError()).c_str()).c_str());
                msThroughputTestConfig.mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
                return ;
            }
            m_vtClients->push_back(pSocket.release());
        }
        //! 等待三秒钟后连接
        ::WaitWithMessageLoop(3 * 1000);
        //! 首先是客户端发送给服务端，告知服务现在开始
        SendBufferToServer();
        //! 
        msThroughputTestConfig.mpAsyncWndMessage->PutAsyncMessage(WM_HWND_TEST_SATRT_MSG);
        ClearRecvSentCount();
    }

    void CCJGW_TcpClientThread::SendBufferToServer()
    {
        for(size_t j = 0; j < msThroughputTestConfig.mnSocketCount; j++)
        {
            CTcpClientPtr* pSocket = m_vtClients[j];
            if(!(*pSocket)->Send(m_sendBuffer, (int)m_sendBuffer.Size()) || mbStartExit)
            {
                SendMessageToHwnd(L"Socket Send Fail !!!\n");
                break;
            }
        }
    }

    EnHandleResult CCJGW_TcpClientThread::OnPrepareConnect(ITcpClient* pSender, CONNID dwConnID, SOCKET socket)
    {
        return HR_OK;
    }

    EnHandleResult CCJGW_TcpClientThread::OnConnect(ITcpClient* pSender, CONNID dwConnID)
    {
/*#ifdef 1/ *_DEBUG* /*/
        std::wstring strTemp;
        JGW_FormatWString(strTemp,_T("OnConnect: %d\n"), dwConnID);
        SendMessageToHwnd(strTemp.c_str());
/*#endif*/
        return HR_OK;
    }

    EnHandleResult CCJGW_TcpClientThread::OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
    {
/*#ifdef 1/ *_DEBUG* /*/
        std::wstring strTemp;
        JGW_FormatWString(strTemp,_T("OnClose: %d\n"), dwConnID);
        SendMessageToHwnd(strTemp.c_str());
/*#endif*/
        return HR_OK;
    }

    EnHandleResult CCJGW_TcpClientThread::OnHandShake(ITcpClient* pSender, CONNID dwConnID)
    {
        return HR_OK;
    }

    EnHandleResult CCJGW_TcpClientThread::OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
    {
#if (_WIN32_WINNT <= _WIN32_WINNT_WS03)
        ::InterlockedExchangeAdd((volatile LONG*)&m_llTotalReceived, iLength);
#else
        ::InterlockedExchangeAdd64(&m_llTotalReceived, iLength);
#endif
        return HR_OK;
    }

    EnHandleResult CCJGW_TcpClientThread::OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
    {
#if (_WIN32_WINNT <= _WIN32_WINNT_WS03)
        ::InterlockedExchangeAdd((volatile LONG*)&m_llTotalSent, iLength);
#else
        ::InterlockedExchangeAdd64(&m_llTotalSent, iLength);
#endif
        if (E_SEND_TYPE == msThroughputTestConfig.meTranservicesType && !mbStartExit)
        {
            pSender->Send(pData,iLength);
            if (msThroughputTestConfig.mnIntervTime > 0) ::WaitWithMessageLoop(msThroughputTestConfig.mnIntervTime);
        }

        return HR_OK;
    }
}