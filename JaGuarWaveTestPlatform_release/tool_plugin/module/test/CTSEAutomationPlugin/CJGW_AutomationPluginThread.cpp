#include "StdAfx.h"
#include "CJGW_AutomationPluginThread.h"
#include "../../../../platform_include/JGW_MSG_ID_Define.h"

namespace JGW
{
    CCJGW_AutomationPluginThread::CCJGW_AutomationPluginThread(void) : mpAsyncWndMessage(NULL),mpsTSETTestConfigParam(NULL)
    {
    }


    CCJGW_AutomationPluginThread::~CCJGW_AutomationPluginThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_AutomationPluginThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_AUTO_TEST,&CCJGW_AutomationPluginThread::OnAutoTSETest)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST,&CCJGW_AutomationPluginThread::OnExecuteConfigViewTest)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_INIT,&CCJGW_AutomationPluginThread::OnInitWnd)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCJGW_AutomationPluginThread::OnRunThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_TSE_INDEX_MSG_RUN,&CCJGW_AutomationPluginThread::OnRunTSEIndexThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_RESET_TSE,&CCJGW_AutomationPluginThread::OnTSEResetMsg)
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_AutomationPluginThread::OnInitWnd(WPARAM wParam,LPARAM lParam)
    {
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)wParam;
        mpsTSETTestConfigParam = (PS_TSE_TEST_CONFIG_PARAM)lParam;
        if (!InitSequenceTest(mpsTSETTestConfigParam->mhMainWnd,JGW_W2A(mpsTSETTestConfigParam->mstrSuiteXmlConfigPath))) mpAsyncWndMessage->PutAsyncMessage(WM_HWND_INIT_TSE_TEST_FAIL_MSG);
        else 
            mpsTSETTestConfigParam->mnTestCount = mpSequenceManageImpl->GetGlobalEnvironmentImpl()->GetInt(L"TSE_TEST_COUNT");
    }

    void CCJGW_AutomationPluginThread::OnExecuteConfigViewTest(WPARAM wParam,LPARAM lParam)
    {
        if (!IsInitTSEConfigOk()) return;
        mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEConfigTest();
        mpsTSETTestConfigParam->mnTestCount = mpSequenceManageImpl->GetGlobalEnvironmentImpl()->GetInt(L"TSE_TEST_COUNT");
    }

    void CCJGW_AutomationPluginThread::OnRunThread(WPARAM wParam,LPARAM lParam)
    {
        if (!IsInitTSEConfigOk() || !ExecSequenceTest())
        {
            mbExecAutoTSETest = false;
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
            Log4WE(L"Current Test Fail");
            return ;
        }
        mbExecAutoTSETest = false;
        mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
        Log4WI(L"Current Test Pass");
    }

    void CCJGW_AutomationPluginThread::OnAutoTSETest(WPARAM wParam,LPARAM lParam)
    {
        if (!mbInitTSEConfig || !mpSequenceManageImpl->GetSequenceTestImpl()->TSEAutoTestNotEmpty()) return ;
        if (mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEAutoTest()) { mpAsyncWndMessage->PutAsyncMessage(WM_HWND_TEST_SATRT_MSG);mbExecAutoTSETest = true;}
    }

    void CCJGW_AutomationPluginThread::OnTestIndexChange(size_t index,int subIndex)
    {
        mpAsyncWndMessage->PutAsyncMessage(WM_HWND_TSE_INDEX_MSG,WPARAM(index),LPARAM(subIndex));
    }

    void CCJGW_AutomationPluginThread::OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus)
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

    void CCJGW_AutomationPluginThread::OnErrorLog(const wchar_t* strErrorLog)
    {
        PELOG4WW(strErrorLog);
    }

    void CCJGW_AutomationPluginThread::OnInfoLog(const wchar_t* strInfoLog)
    {
        PDLOG4WW(strInfoLog);
        if (NULL != JGW_WStrCaseWStr(strInfoLog,L" fail "))  PELOG4WW(strInfoLog);
    }

    void CCJGW_AutomationPluginThread::OnDebugLog(const wchar_t* strDebugLog)
    {
        Log4WD(strDebugLog);
    }

    void CCJGW_AutomationPluginThread::SendListMsgToWnd(const wchar_t* strMsg,DWORD dwRGB)
    {
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_TEST_LIST_MSG,strMsg,LPARAM(dwRGB));
    }

    void CCJGW_AutomationPluginThread::OnUserDefinedLog(int id,const wchar_t* strCustomLog)
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

    void CCJGW_AutomationPluginThread::OnRunTSEIndexThread(WPARAM wParam,LPARAM lParam)
    {
        mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEGroupTestToIndex((size_t)wParam);
        mpAsyncWndMessage->PutAsyncMessage(WM_HWND_TSE_INDEX_MSG_RUN_COMPLETE);
    }

    void CCJGW_AutomationPluginThread::OnTSEResetMsg(WPARAM wParam,LPARAM lParam)
    {
        mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSETestFinalize();
        mpSequenceManageImpl->GetSequenceTestImpl()->ClearTSEExistedTestArrayIndex();
        mpAsyncWndMessage->PutAsyncMessage(WM_HWND_RESET_TSE_COMPLETE);
    }
}