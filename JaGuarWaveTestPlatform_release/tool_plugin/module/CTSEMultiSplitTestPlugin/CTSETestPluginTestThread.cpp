#include "StdAfx.h"
#include "CTSETestPluginTestThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"

namespace JGW
{
	LONG gnTokenID = 0;
    CCTSETestPluginTestThread::CCTSETestPluginTestThread(HWND hMainWnd) : mpAsyncWndMessage(NULL),mpsMultiSplitTestConfig(NULL),mhMainWnd(hMainWnd)
    {
		mnTokenID = ::InterlockedIncrement(&gnTokenID); 
		mbMultiTSETest = true;
		
    }


    CCTSETestPluginTestThread::~CCTSETestPluginTestThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCTSETestPluginTestThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_AUTO_TEST,&CCTSETestPluginTestThread::OnAutoTSETest)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST,&CCTSETestPluginTestThread::OnExecuteConfigViewTest)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_INIT,&CCTSETestPluginTestThread::OnInitWnd)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCTSETestPluginTestThread::OnRunThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_CLEANUP,&CCTSETestPluginTestThread::OnExitThread)
    END_MYTHREAD_MESSAGE_MAP()


    void CCTSETestPluginTestThread::OnExitThread(WPARAM wParam,LPARAM lParam)
    {
        ExitTSE();
    }

	std::wstring CCTSETestPluginTestThread::GetDebufMessage()
	{
		return mstrDebugTestLogStream.str();
	}

	std::wstring CCTSETestPluginTestThread::GetErrorMessage()
	{
		return mstrErrorTestLogStream.str();
	}

    void CCTSETestPluginTestThread::OnInitWnd(WPARAM wParam,LPARAM lParam)
    {
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)wParam;
        mpsMultiSplitTestConfig = (PS_MultiSplitTestConfig)lParam;
        if (!InitSequenceTest(mhMainWnd,JGW_W2A(mpsMultiSplitTestConfig->mstrSuiteXmlConfigPath))) mpAsyncWndMessage->PutAsyncMessage(WM_HWND_INIT_TSE_TEST_FAIL_MSG);
    }

    void CCTSETestPluginTestThread::OnExecuteConfigViewTest(WPARAM wParam,LPARAM lParam)
    {
        if (!IsInitTSEConfigOk()) return;
        mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEConfigTest();
    }
#if 1
    void CCTSETestPluginTestThread::UpdateCopyDataMessage(const std::string& strMessage)
    {
        mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutAnsiCopyDataMessageQueue(strMessage);
    }
#else
    void CCTSETestPluginTestThread::UpdateQIAWindowHandle(int hanlde)
    {
        mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutInt(TSE_QIA_WINDOW_HANLDE,hanlde);
    }

    void CCTSETestPluginTestThread::UpdateCheckQIAIsRunningFlag(int flag)
    {
        mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutInt(TSE_QIA_IS_RUNNING_FLAG,flag);
    }

    void CCTSETestPluginTestThread::AddHtmlTestLogging(const std::wstring& strHtmlFilePath)
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


    void CCTSETestPluginTestThread::UpdateQiaTestRessult(bool result)
    {
        mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutBool(TSE_QIA_TEST_RESULT,result);
        mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutInt(TSE_QIA_TEST_DONE,1);
    }
#endif
    void CCTSETestPluginTestThread::OnRunThread(WPARAM wParam,LPARAM lParam)
    {
		mstrErrorTestLogStream.str(L"");
		mstrErrorTestLogStream.clear();
		mstrDebugTestLogStream.str(L"");
		mstrDebugTestLogStream.clear();

        if (!IsInitTSEConfigOk() || !ExecSequenceTest())
        {
            mbExecAutoTSETest = false;
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
            return ;
        }
        mbExecAutoTSETest = false;
        mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
    }

    void CCTSETestPluginTestThread::OnAutoTSETest(WPARAM wParam,LPARAM lParam)
    {
        if (!mbInitTSEConfig || !mpSequenceManageImpl->GetSequenceTestImpl()->TSEAutoTestNotEmpty()) return ;
        if (mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEAutoTest()) { mpAsyncWndMessage->PutAsyncMessage(WM_HWND_TEST_SATRT_MSG);mbExecAutoTSETest = true;}
    }

    void CCTSETestPluginTestThread::OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus)
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

    void CCTSETestPluginTestThread::OnErrorLog(const wchar_t* strErrorLog)
    {
        PELOG4WW(strErrorLog);
        mstrErrorMsg += strErrorLog;
        mstrErrorMsg += L"\r\n";
		mstrErrorTestLogStream << strErrorLog << std::endl;

        //mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strErrorLog);
    }

    void CCTSETestPluginTestThread::OnInfoLog(const wchar_t* strInfoLog)
    {
        PDLOG4WW(strInfoLog);
        if (NULL != JGW_WStrCaseWStr(strInfoLog,L" fail "))
        {
            OnErrorLog(strInfoLog);
			mstrErrorTestLogStream << strInfoLog << std::endl;
        }    
		mstrDebugTestLogStream << strInfoLog << std::endl;
        //mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strInfoLog);
    }

    void CCTSETestPluginTestThread::OnDebugLog(const wchar_t* strDebugLog)
    {
        Log4WD(strDebugLog);
		mstrDebugTestLogStream << strDebugLog << std::endl;
    }

    void CCTSETestPluginTestThread::SendListMsgToWnd(const wchar_t* strMsg,DWORD dwRGB)
    {
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_TEST_LIST_MSG,strMsg,LPARAM(dwRGB));
    }

    void CCTSETestPluginTestThread::OnUserDefinedLog(int id,const wchar_t* strCustomLog)
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
