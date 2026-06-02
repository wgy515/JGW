#include "StdAfx.h"
#include "CTSEThroughputTestThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
namespace JGW
{
    CCTSEThroughputTestThread::CCTSEThroughputTestThread(void):mpAsyncWndMessage(NULL),mpsTSETThroughputTestConfigParam(NULL)
    {
    }


    CCTSEThroughputTestThread::~CCTSEThroughputTestThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCTSEThroughputTestThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_AUTO_TEST,&CCTSEThroughputTestThread::OnAutoTSETest)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST,&CCTSEThroughputTestThread::OnExecuteConfigViewTest)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_INIT,&CCTSEThroughputTestThread::OnInitWnd)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCTSEThroughputTestThread::OnRunThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_CLEANUP,&CCTSEThroughputTestThread::OnExitThread)
    END_MYTHREAD_MESSAGE_MAP()


    void CCTSEThroughputTestThread::OnExitThread(WPARAM wParam,LPARAM lParam)
    {
        ExitTSE();
    }

    void CCTSEThroughputTestThread::OnInitWnd(WPARAM wParam,LPARAM lParam)
    {
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)wParam;
        mpsTSETThroughputTestConfigParam = (PS_TSE_THROUGHPUT_TEST_CONFIG_PARAM)lParam;
        if (!InitSequenceTest(mpsTSETThroughputTestConfigParam->mhMainWnd,JGW_W2A(mpsTSETThroughputTestConfigParam->mstrSuiteXmlConfigPath))) mpAsyncWndMessage->PutAsyncMessage(WM_HWND_INIT_TSE_TEST_FAIL_MSG);
    }

    void CCTSEThroughputTestThread::OnExecuteConfigViewTest(WPARAM wParam,LPARAM lParam)
    {
        if (!IsInitTSEConfigOk()) return;
        mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEConfigTest();
    }
#if 1
    void CCTSEThroughputTestThread::UpdateCopyDataMessage(const std::string& strMessage)
    {
        mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutAnsiCopyDataMessageQueue(strMessage);
    }
#else
    void CCTSEThroughputTestThread::UpdateQIAWindowHandle(int hanlde)
    {
        mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutInt(TSE_QIA_WINDOW_HANLDE,hanlde);
    }

    void CCTSEThroughputTestThread::UpdateCheckQIAIsRunningFlag(int flag)
    {
        mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutInt(TSE_QIA_IS_RUNNING_FLAG,flag);
    }

    void CCTSEThroughputTestThread::AddHtmlTestLogging(const std::wstring& strHtmlFilePath)
    {
        std::wstring htmlFile = mpSequenceManageImpl->GetGlobalEnvironmentImpl()->GetString(TSE_QIA_HTML_TEST_FILE);
        if (htmlFile.empty())
        {
            mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutString(TSE_QIA_HTML_TEST_FILE,htmlFile);
        }
        else
        {
            htmlFile += L"&&&&&&";
            htmlFile += strHtmlFilePath;
            mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutString(TSE_QIA_HTML_TEST_FILE,htmlFile);
        }
    }


    void CCTSEThroughputTestThread::UpdateQiaTestRessult(bool result)
    {
        mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutBool(TSE_QIA_TEST_RESULT,result);
        mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutInt(TSE_QIA_TEST_DONE,1);
    }
#endif
    void CCTSEThroughputTestThread::OnRunThread(WPARAM wParam,LPARAM lParam)
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

    void CCTSEThroughputTestThread::OnAutoTSETest(WPARAM wParam,LPARAM lParam)
    {
        if (!mbInitTSEConfig || !mpSequenceManageImpl->GetSequenceTestImpl()->TSEAutoTestNotEmpty()) return ;
        if (mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEAutoTest()) { mpAsyncWndMessage->PutAsyncMessage(WM_HWND_TEST_SATRT_MSG);mbExecAutoTSETest = true;}
    }

    void CCTSEThroughputTestThread::OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus)
    {
        switch (eTestStatus)
        {
        case E_INIT_TEST_STATUS:
			mstrErrorMsg = L"";
            SendListMsgToWnd(strTestName,BALCK);
            break;
        case E_RUN_TEST_STATUS:
			mstrErrorMsg = L"";
            SendListMsgToWnd(strTestName,BALCK);
            break;
        case E_RUNING_TEST_STATUS:
			mstrErrorMsg = L"";
            SendListMsgToWnd(strTestName,BULE);
            break;
        case E_PASS_TEST_STATUS:
            SendListMsgToWnd(strTestName,GREEN);
            break;
        case E_FAIL_TEST_STATUS:
            SendListMsgToWnd(strTestName,RED);
			if (!mstrErrorMsg.empty()) mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,mstrErrorMsg.c_str());
            break;
        }
    }

    void CCTSEThroughputTestThread::OnErrorLog(const wchar_t* strErrorLog)
    {
        PELOG4WW(strErrorLog);
		mstrErrorMsg += strErrorLog;
		mstrErrorMsg += L"\r\n";
        //mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strErrorLog);
    }

    void CCTSEThroughputTestThread::OnInfoLog(const wchar_t* strInfoLog)
    {
        PDLOG4WW(strInfoLog);
        if (NULL != JGW_WStrCaseWStr(strInfoLog,L" fail "))
        {
            OnErrorLog(strInfoLog);
        }    
        //mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strInfoLog);
    }

    void CCTSEThroughputTestThread::OnDebugLog(const wchar_t* strDebugLog)
    {
        Log4WD(strDebugLog);
    }

    void CCTSEThroughputTestThread::SendListMsgToWnd(const wchar_t* strMsg,DWORD dwRGB)
    {
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_TEST_LIST_MSG,strMsg,LPARAM(dwRGB));
    }

    void CCTSEThroughputTestThread::OnUserDefinedLog(int id,const wchar_t* strCustomLog)
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
        case E_COPYDATA_TSE_SHOW_IDEL_BUSY_LIST_TEST_STATUS:
            SendListMsgToWnd(strCustomLog,BALCK);
            SendListMsgToWnd(strCustomLog,BULE);
            //OnTestListMsg(WPARAM(testLog.Message.c_str()),BALCK);
            //OnTestListMsg(WPARAM(testLog.Message.c_str()),BULE);
            break;
        case E_COPYDATA_TSE_SHOW_FAIL_LIST_TEST_STATUS:
            SendListMsgToWnd(strCustomLog,RED);
            //OnTestListMsg(WPARAM(testLog.Message.c_str()),RED);
            break;
        case E_COPYDATA_TSE_SHOW_PASS_LIST_TEST_STATUS:
            SendListMsgToWnd(strCustomLog,GREEN);
            //OnTestListMsg(WPARAM(testLog.Message.c_str()),GREEN);
            break;
        }
    }
}