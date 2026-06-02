#include "StdAfx.h"
#include "CTSECalConfigTestThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include "CTSEGuCalBase.h"
namespace JGW
{
    CCTSECalConfigTestThread::CCTSECalConfigTestThread(void) : mpAsyncWndMessage(NULL),mpsTSECalConfigTestConfigParam(NULL)
    {
    }


    CCTSECalConfigTestThread::~CCTSECalConfigTestThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCTSECalConfigTestThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_AUTO_TEST,&CCTSECalConfigTestThread::OnAutoTSETest)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST,&CCTSECalConfigTestThread::OnExecuteConfigViewTest)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_INIT,&CCTSECalConfigTestThread::OnInitWnd)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCTSECalConfigTestThread::OnRunThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_CLEANUP,&CCTSECalConfigTestThread::OnExitThread)
        ON_MYTHREAD_MESSGAE(WM_THREAD_SAVE_GU_CAL_DATA,&CCTSECalConfigTestThread::OnSaveGuCalData)
    END_MYTHREAD_MESSAGE_MAP()


    void CCTSECalConfigTestThread::OnExitThread(WPARAM wParam,LPARAM lParam)
    {
        ExitTSE();
    }

    void CCTSECalConfigTestThread::OnInitWnd(WPARAM wParam,LPARAM lParam)
    {
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)wParam;
        mpsTSECalConfigTestConfigParam = (PS_TSE_CALCONFIG_TEST_CONFIG_PARAM)lParam;
        if (!InitSequenceTest(mpsTSECalConfigTestConfigParam->mhMainWnd,JGW_W2A(mpsTSECalConfigTestConfigParam->mstrSuiteXmlConfigPath))) mpAsyncWndMessage->PutAsyncMessage(WM_HWND_INIT_TSE_TEST_FAIL_MSG);
        mpsTSECalConfigTestConfigParam->mfGuStartLoss = mpSequenceManageImpl->GetGlobalEnvironmentImpl()->GetFloat(L"TSE_GU_LOSS");
    }

    void CCTSECalConfigTestThread::OnExecuteConfigViewTest(WPARAM wParam,LPARAM lParam)
    {
        if (!IsInitTSEConfigOk()) return;
        mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEConfigTest();
        //! TSE_GU_LOSS
        mpsTSECalConfigTestConfigParam->mfGuStartLoss = mpSequenceManageImpl->GetGlobalEnvironmentImpl()->GetFloat(L"TSE_GU_LOSS");
    }

    void CCTSECalConfigTestThread::UpdateCopyDataMessage(const std::string& strMessage)
    {
        mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutAnsiCopyDataMessageQueue(strMessage);
    }

    void CCTSECalConfigTestThread::OnRunThread(WPARAM wParam,LPARAM lParam)
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

    void CCTSECalConfigTestThread::OnAutoTSETest(WPARAM wParam,LPARAM lParam)
    {
        if (!mbInitTSEConfig || !mpSequenceManageImpl->GetSequenceTestImpl()->TSEAutoTestNotEmpty()) return ;
        if (mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEAutoTest()) { mpAsyncWndMessage->PutAsyncMessage(WM_HWND_TEST_SATRT_MSG);mbExecAutoTSETest = true;}
    }

    void CCTSECalConfigTestThread::OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus)
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

    void CCTSECalConfigTestThread::OnErrorLog(const wchar_t* strErrorLog)
    {
        PELOG4WW(strErrorLog);
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strErrorLog);
    }

    void CCTSECalConfigTestThread::OnInfoLog(const wchar_t* strInfoLog)
    {
        PDLOG4WW(strInfoLog);
        if (NULL != JGW_WStrCaseWStr(strInfoLog,L" fail "))  PELOG4WW(strInfoLog);
        //mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strInfoLog);
    }

    void CCTSECalConfigTestThread::OnDebugLog(const wchar_t* strDebugLog)
    {
        Log4WD(strDebugLog);
    }

    void CCTSECalConfigTestThread::SendListMsgToWnd(const wchar_t* strMsg,DWORD dwRGB)
    {
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_TEST_LIST_MSG,strMsg,LPARAM(dwRGB));
    }

    void CCTSECalConfigTestThread::OnUserDefinedLog(int id,const wchar_t* strCustomLog)
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

    void CCTSECalConfigTestThread::OnSaveGuCalData(WPARAM wParam,LPARAM lParam)
    {
        CCTSEGuCalBase* pGuCalBase = (CCTSEGuCalBase*)wParam;
        std::wstring strErrorMsg = L"Error Param";
        if (pGuCalBase)
        {
            strErrorMsg = pGuCalBase->SaveDutCalibrationLossFile(pGuCalBase->GetDutTestDataFilePath());
#ifndef _DEBUG
            /*if (result) */JGW_DeleteFile(pGuCalBase->GetDutTestDataFilePath());
#endif         
        }

        if (!strErrorMsg.empty())
        {
            MessageBox(mpAsyncWndMessage->GetMainHwnd(),strErrorMsg.c_str(),L"Error",MB_ICONERROR);
        }
        mpAsyncWndMessage->PutAsyncMessage(WM_HWND_SAVE_GU_CAL_DATA,strErrorMsg.empty()?1:0);
    }
}
