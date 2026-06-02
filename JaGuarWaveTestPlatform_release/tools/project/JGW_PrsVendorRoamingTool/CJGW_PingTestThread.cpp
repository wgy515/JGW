#include "StdAfx.h"
#include "CJGW_PingTestThread.h"
#include "..\..\..\platform_include/JGW_MSG_ID_Define.h"

#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_PingTestThread::CCJGW_PingTestThread(void) : mbPingSucessful(false)
    {
    }


    CCJGW_PingTestThread::~CCJGW_PingTestThread(void)
    {
    }

    bool CCJGW_PingTestThread::StartPingTestThread(const std::string& strPingAddr,CCJGW_AsyncWndMessage* pAsyncWndMessage)
    {
        mstrPingAddr = strPingAddr;
        mpAsyncWndMessage = pAsyncWndMessage;
        mbRuningPingTestThread = true;

        SendWindowMsg(L"Start Ping(%s) Test Thread...",JGW_A2W(strPingAddr).c_str());
        if (!mcPingTestThread.CreateMessageThread(OnRuningPingTestThreadCallback,this,true))
        {
            SendWindowMsg(L"Error : Start Ping Test Thread ");
            return false;
        }

        return true;
    }

    void CCJGW_PingTestThread::StopPingTestThread()
    {
        mbRuningPingTestThread = false;
        while (mcPingTestThread.IsThreadRun()) Sleep(100);
    }

    void CCJGW_PingTestThread::OnRuningPingTestThreadCallback(void *lpParam)
    {
        CCJGW_PingTestThread* pPingTestThread = (CCJGW_PingTestThread*)lpParam;
        if (NULL != pPingTestThread)
        {
            pPingTestThread->OnRuningPingTestThread();
        }
    }

    void CCJGW_PingTestThread::OnRuningPingTestThread()
    {
        while (mbRuningPingTestThread)
        {
            if (mIcmpSocket.Ping(mstrPingAddr))
            {
                mbPingSucessful = true;
                mpAsyncWndMessage->PutAsyncStringMessage(WM_TIME_DELAY_MSG,JGW_GetFormatWString(L"%.3lf",mIcmpSocket.GetPingTimeMs()).c_str());
                SendWindowMsg(L"ping %s %s",JGW_A2W(mstrPingAddr).c_str(),JGW_A2W(mIcmpSocket.GetPingResult()).c_str());
                Sleep(1000 - ((int)mIcmpSocket.GetPingTimeMs() % 1000) - 1);
            }
            else
            {
                mbPingSucessful = false;
                mpAsyncWndMessage->PutAsyncStringMessage(WM_TIME_DELAY_MSG,L"");
            }
        }
        mbPingSucessful = false;
        mbRuningPingTestThread = false;
    }

    bool CCJGW_PingTestThread::GetPingSucessful()
    {
        return mbPingSucessful;
    }

    void CCJGW_PingTestThread::SendWindowMsg(const std::wstring& strMessage)
    {
        Log4WI(strMessage.c_str());
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strMessage.c_str());
    }

    void CCJGW_PingTestThread::SendWindowMsg(const wchar_t* szFormat,...)
    {
        std::wstring strLog;
        FORMAT_WSTRING(strLog,szFormat);
        Log4WI(strLog.c_str());
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strLog.c_str());
    }
}

