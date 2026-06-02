#include "StdAfx.h"
#include "CSTSETestThread.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"

namespace JGW
{
    CCSTSETestThread::CCSTSETestThread(void) : mpsCSTSETestConfig(NULL),mpAsyncWndMessage(NULL),mbExecAutoTSETest(false),mbInitTSEConfig(false)
    {
        mServerBase.AddLogListener(this);
        mServerBase.AddTestStatusListener(this);
    }


    CCSTSETestThread::~CCSTSETestThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCSTSETestThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_AUTO_TEST,&CCSTSETestThread::OnAutoTSETest)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST,&CCSTSETestThread::OnExecuteConfigViewTest)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_INIT,&CCSTSETestThread::OnInitWnd)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCSTSETestThread::OnRunThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCSTSETestThread::OnAutoTSETest(WPARAM wParam,LPARAM lParam)
    {
        if (!mbInitTSEConfig) return ;
        if (mServerBase.ExecuteAutoTest())
        {
            mpAsyncWndMessage->PutAsyncMessage(WM_HWND_TEST_SATRT_MSG);
            mbExecAutoTSETest = true;
        }
    }

    void CCSTSETestThread::OnExecuteConfigViewTest(WPARAM wParam,LPARAM lParam)
    {
        if (mbInitTSEConfig && mServerBase.IsStartCSTSEClient())
        {
            mServerBase.ExecuteConfigViewTest();
        }
    }

    void CCSTSETestThread::OnInitWnd( WPARAM wParam,LPARAM lParam )
    {
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)wParam;
        mpsCSTSETestConfig = (PS_CSTSE_TEST_CONFIG)lParam;
        mbInitTSEConfig = true;
        if (!mServerBase.StartCSTSEClient(mpsCSTSETestConfig->mstrCSTSEClientExecPath,mpsCSTSETestConfig->mmapExecParam))
        {
            mbInitTSEConfig = false;
            mpAsyncWndMessage->PutAsyncMessage(WM_HWND_INIT_TSE_TEST_FAIL_MSG);
        }
    }

    void CCSTSETestThread::OnRunThread(WPARAM wParam,LPARAM lParam)
    {
        if (!mServerBase.IsStartCSTSEClient() || !mServerBase.ExecuteAllTest())
        {
            mbExecAutoTSETest = false;
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
            return ;
        }
        mbExecAutoTSETest = false;
        mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
    }


    void CCSTSETestThread::OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus)
    {
        switch (eTestStatus)
        {
        case E_INIT_TEST_STATUS:
            SendListMsgToWnd(strTestName,BALCK);
            break;
        case E_RUN_TEST_STATUS:
            SendListMsgToWnd(strTestName,BALCK);
            break;
        case E_RUNING_TEST_STATUS:
            SendListMsgToWnd(strTestName,BULE);
            break;
        case E_PASS_TEST_STATUS:
            SendListMsgToWnd(strTestName,GREEN);
            break;
        case E_FAIL_TEST_STATUS:
            SendListMsgToWnd(strTestName,RED);
            break;
        }
    }

    void CCSTSETestThread::OnErrorLog(const wchar_t* strErrorLog)
    {
        PELOG4WW(strErrorLog);
    }

    void CCSTSETestThread::OnInfoLog(const wchar_t* strInfoLog)
    {
        PDLOG4WW(strInfoLog);
        if (NULL != JGW_WStrCaseWStr(strInfoLog,L" fail "))  PELOG4WW(strInfoLog);
    }

    void CCSTSETestThread::OnDebugLog(const wchar_t* strDebugLog)
    {
        Log4WD(strDebugLog);
    }

    void CCSTSETestThread::SendListMsgToWnd(const wchar_t* strMsg,DWORD dwRGB)
    {
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_TEST_LIST_MSG,strMsg,LPARAM(dwRGB));
    }

    void CCSTSETestThread::OnUserDefinedLog(int id,const wchar_t* strCustomLog)
    {
        switch (id)
        {
        case E_USER_DEFINE_HTMLFILEPATH:
            PHLOG4WW(strCustomLog);
            break;
        case E_TSE_SHOW_CONTENT_INFO:
            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_TSE_SHOW_CONTENT,strCustomLog);
            break;
        case E_TSE_SHOW_TEST_INFO_TO_RICHEDIT:
            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strCustomLog);
            PDLOG4WW(strCustomLog);
            break;
        }
    }
}

