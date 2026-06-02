#include "StdAfx.h"
#include "TSEUITestThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
namespace JGW
{
    CTSEUITestThread::CTSEUITestThread(void):mpAsyncWndMessage(NULL),mpsTSETTestConfigParam(NULL),mbRunThreaded(false)
    {
    }


    CTSEUITestThread::~CTSEUITestThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CTSEUITestThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_AUTO_TEST,&CTSEUITestThread::OnAutoTSETest)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST,&CTSEUITestThread::OnExecuteConfigViewTest)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_INIT,&CTSEUITestThread::OnInitWnd)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CTSEUITestThread::OnRunThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_CLEANUP,&CTSEUITestThread::OnExitThread)
    END_MYTHREAD_MESSAGE_MAP()


    void CTSEUITestThread::OnExitThread(WPARAM wParam,LPARAM lParam)
    {
        ExitTSE();
    }

    void CTSEUITestThread::OnInitWnd(WPARAM wParam,LPARAM lParam)
    {
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)wParam;
        mpsTSETTestConfigParam = (PS_TSE_TEST_CONFIG_PARAM)lParam;
        if (!InitSequenceTest(mpsTSETTestConfigParam->mhMainWnd,JGW_W2A(mpsTSETTestConfigParam->mstrSuiteXmlConfigPath))) mpAsyncWndMessage->PutAsyncMessage(WM_HWND_INIT_TSE_TEST_FAIL_MSG);
    }

    void CTSEUITestThread::OnExecuteConfigViewTest(WPARAM wParam,LPARAM lParam)
    {
        if (!IsInitTSEConfigOk()) return;
        mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEConfigTest();
    }

    void CTSEUITestThread::OnRunThread(WPARAM wParam,LPARAM lParam)
    {
        mbRunThreaded = true;
        if (!IsInitTSEConfigOk() || !ExecSequenceTest())
        {
            mbExecAutoTSETest = false;
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
            return ;
        }
        mbExecAutoTSETest = false;
        mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
    }

    void CTSEUITestThread::OnAutoTSETest(WPARAM wParam,LPARAM lParam)
    {
        if (!mbInitTSEConfig || !mpSequenceManageImpl->GetSequenceTestImpl()->TSEAutoTestNotEmpty()) return ;
        if (mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEAutoTest()) { mpAsyncWndMessage->PutAsyncMessage(WM_HWND_TEST_SATRT_MSG);mbExecAutoTSETest = true;}
    }

    void CTSEUITestThread::OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus)
    {
        std::wstring strTemp;
        switch (eTestStatus)
        {
        case E_INIT_TEST_STATUS:
            SendListMsgToWnd(strTestName,BALCK);
            break;
        case E_RUN_TEST_STATUS:
            SendListMsgToWnd(strTestName,BALCK);
            /*JGW_FormatWString(strTemp,L"%s => Runing",strTestName);
            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strTemp.c_str());*/
            break;
        case E_RUNING_TEST_STATUS:
            //SendListMsgToWnd(strTestName,BULE);
            JGW_FormatWString(strTemp,L"%s => Runing",strTestName);
            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strTemp.c_str());
            break;
        case E_PASS_TEST_STATUS:
            SendListMsgToWnd(strTestName,GREEN);
            JGW_FormatWString(strTemp,L"%s => PASS",strTestName);
            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strTemp.c_str());
            break;
        case E_FAIL_TEST_STATUS:
            SendListMsgToWnd(strTestName,RED);
            JGW_FormatWString(strTemp,L"%s => FAIL",strTestName);
            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strTemp.c_str());
            break;
        }
    }

    void CTSEUITestThread::OnErrorLog(const wchar_t* strErrorLog)
    {
        /*PELOG4WW(strErrorLog);
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strErrorLog);*/
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strErrorLog);
    }

    void CTSEUITestThread::OnInfoLog(const wchar_t* strInfoLog)
    {
        /*PDLOG4WW(strInfoLog);
        if (NULL != JGW_WStrCaseWStr(strInfoLog,L" fail "))  PELOG4WW(strInfoLog);*/
        //if (mbRunThreaded) mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strInfoLog);
    }

    void CTSEUITestThread::OnDebugLog(const wchar_t* strDebugLog)
    {
        //Log4WD(strDebugLog);
        //mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strDebugLog);
    }

    void CTSEUITestThread::SendListMsgToWnd(const wchar_t* strMsg,DWORD dwRGB)
    {
        //mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_TEST_LIST_MSG,strMsg,LPARAM(dwRGB));
    }

    void CTSEUITestThread::OnUserDefinedLog(int id,const wchar_t* strCustomLog)
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
