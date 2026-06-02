#include "StdAfx.h"
#include "TSETestThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
namespace JGW
{
    CTSETestThread::CTSETestThread(void):mpAsyncWndMessage(NULL),mpsTSETTestConfigParam(NULL)
    {
    }


    CTSETestThread::~CTSETestThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CTSETestThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_AUTO_TEST,&CTSETestThread::OnAutoTSETest)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST,&CTSETestThread::OnExecuteConfigViewTest)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_INIT,&CTSETestThread::OnInitWnd)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CTSETestThread::OnRunThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_TSE_INDEX_MSG_RUN,&CTSETestThread::OnRunTSEIndexThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_RESET_TSE,&CTSETestThread::OnTSEResetMsg)
    END_MYTHREAD_MESSAGE_MAP()

    void CTSETestThread::OnInitWnd(WPARAM wParam,LPARAM lParam)
    {
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)wParam;
        mpsTSETTestConfigParam = (PS_TSE_TEST_CONFIG_PARAM)lParam;
        if (!InitSequenceTest(mpsTSETTestConfigParam->mhMainWnd,JGW_W2A(mpsTSETTestConfigParam->mstrSuiteXmlConfigPath))) mpAsyncWndMessage->PutAsyncMessage(WM_HWND_INIT_TSE_TEST_FAIL_MSG);
    }

    void CTSETestThread::OnExecuteConfigViewTest(WPARAM wParam,LPARAM lParam)
    {
        if (!IsInitTSEConfigOk()) return;
        mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEConfigTest();
    }

    void CTSETestThread::OnRunThread(WPARAM wParam,LPARAM lParam)
    {
        if (!IsInitTSEConfigOk() || !ExecSequenceTest())
        {
            mbExecAutoTSETest = false;
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
            return ;
        }
        mbExecAutoTSETest = false;
        mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
    }

    void CTSETestThread::OnAutoTSETest(WPARAM wParam,LPARAM lParam)
    {
        if (!mbInitTSEConfig || !mpSequenceManageImpl->GetSequenceTestImpl()->TSEAutoTestNotEmpty()) return ;
        if (mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEAutoTest()) { mpAsyncWndMessage->PutAsyncMessage(WM_HWND_TEST_SATRT_MSG);mbExecAutoTSETest = true;}
    }

    void CTSETestThread::OnTestIndexChange(size_t index,int subIndex)
    {
        mpAsyncWndMessage->PutAsyncMessage(WM_HWND_TSE_INDEX_MSG,WPARAM(index),LPARAM(subIndex));
    }

    void CTSETestThread::OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus)
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

    void CTSETestThread::OnErrorLog(const wchar_t* strErrorLog)
    {
        PELOG4WW(strErrorLog);
    }

    void CTSETestThread::OnInfoLog(const wchar_t* strInfoLog)
    {
        PDLOG4WW(strInfoLog);
        if (NULL != JGW_WStrCaseWStr(strInfoLog,L" fail "))  PELOG4WW(strInfoLog);
    }

    void CTSETestThread::OnDebugLog(const wchar_t* strDebugLog)
    {
        Log4WD(strDebugLog);
    }

    void CTSETestThread::SendListMsgToWnd(const wchar_t* strMsg,DWORD dwRGB)
    {
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_TEST_LIST_MSG,strMsg,LPARAM(dwRGB));
    }

    void CTSETestThread::OnUserDefinedLog(int id,const wchar_t* strCustomLog)
    {
        switch (id)
        {
        case E_USER_DEFINE_HTMLFILEPATH:
            PHLOG4WW(strCustomLog);
            break;
        case E_TSE_SHOW_CONTENT_INFO:
            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_TSE_SHOW_CONTENT,strCustomLog);
            break;
        default:
            PDLOG4WW(strCustomLog);
        }
    }

    void CTSETestThread::OnRunTSEIndexThread(WPARAM wParam,LPARAM lParam)
    {
        mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEGroupTestToIndex((size_t)wParam);
        mpAsyncWndMessage->PutAsyncMessage(WM_HWND_TSE_INDEX_MSG_RUN_COMPLETE);
    }

    void CTSETestThread::OnTSEResetMsg(WPARAM wParam,LPARAM lParam)
    {
        mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSETestFinalize();
        mpSequenceManageImpl->GetSequenceTestImpl()->ClearTSEExistedTestArrayIndex();
        mpAsyncWndMessage->PutAsyncMessage(WM_HWND_RESET_TSE_COMPLETE);
    }
}