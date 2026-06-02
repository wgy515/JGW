#include "StdAfx.h"
#include "CRFCalVerTestThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
namespace JGW
{
    CCRFCalVerTestThread::CCRFCalVerTestThread(void):mpAsyncWndMessage(NULL)
        ,mptrTestSuiteManageImpl(NULL)
        ,mpSequenceManageImpl(NULL)
		,mbInitTSEConfig(false)
        ,mbInitPreTSETest(false)
    {
        
    }

    CCRFCalVerTestThread::~CCRFCalVerTestThread(void)
    {
        CoUninitialize();
    }

    void CCRFCalVerTestThread::OnErrorLog(const wchar_t* strErrorLog)
    {
        PELOG4WW(strErrorLog);
    }

    void CCRFCalVerTestThread::OnDebugLog(const wchar_t* strDebugLog)
    {
        Log4WD(strDebugLog);
    }

    void CCRFCalVerTestThread::OnInfoLog(const wchar_t* strInfoLog)
    {
        PDLOG4WW(strInfoLog);
        if (NULL != JGW_WStrCaseWStr(strInfoLog,L" fail "))  PELOG4WW(strInfoLog);
    }

    void CCRFCalVerTestThread::OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus)
    {
        switch (eTestStatus)
        {
        case E_INIT_TEST_STATUS:
            SendListMsgToWnd(strTestName,BALCK);
            break;
        case E_RUN_TEST_STATUS:
            SendListMsgToWnd(strTestName,BALCK);
            break;
        case E_PASS_TEST_STATUS:
            SendListMsgToWnd(strTestName,GREEN);
            break;
        case E_FAIL_TEST_STATUS:
            SendListMsgToWnd(strTestName,RED);
            break;
        }
    }

    void CCRFCalVerTestThread::OnUserDefinedLog(int id,const wchar_t* strCustomLog)
    {
		switch (id)
		{
		case E_USER_DEFINE_HTMLFILEPATH:
			PHLOG4WW(strCustomLog);
			break;
		case E_TSE_SHOW_CONTENT_INFO:
			mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_TSE_SHOW_CONTENT,strCustomLog);
			break;
		}
    }
#if 0
    void _cdecl SuiteTestLog(int nIndex,int type,const wchar_t* strmsg)
    {
        if (!gpRFCalVerTestThread || !strmsg) return ;

        switch (type)
        {
        case E_TEST_SUITE_TEST_NAME:
            if ( NULL != _tcsstr(strmsg,L" runing\n"))
            {
                std::wstring str(strmsg);
                JGW_ReplaceStringW(str,L" runing\n",L"");
                gpRFCalVerTestThread->SendListMsgToWnd(str.c_str(),BALCK);
            }
            else if( NULL != _tcsstr(strmsg,L" pass\n"))
            {
                std::wstring str(strmsg);
                JGW_ReplaceStringW(str,L" pass\n",L"");
                gpRFCalVerTestThread->SendListMsgToWnd(str.c_str(),GREEN);
            }
            else if( NULL != _tcsstr(strmsg,L" fail\n"))
            {
                std::wstring str(strmsg);
                JGW_ReplaceStringW(str,L" fail\n",L"");
                gpRFCalVerTestThread->SendListMsgToWnd(str.c_str(),RED);
            }
            else
            {
                gstrTestSuiteName = strmsg;
                gpRFCalVerTestThread->SendListMsgToWnd(gstrTestSuiteName.c_str(),BALCK);
            }
            break;
        default:
            break;
        }
    }
#endif
    BEGIN_MYTHREAD_MESSAGE_MAP(CCRFCalVerTestThread, CCMessageThread)
		ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_AUTO_TEST,&CCRFCalVerTestThread::OnAutoTSETest)
		ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST,&CCRFCalVerTestThread::OnExecuteConfigViewTest)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_INIT,&CCRFCalVerTestThread::OnInitWnd)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCRFCalVerTestThread::OnRunThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCRFCalVerTestThread::OnInitWnd(WPARAM wParam,LPARAM lParam)
    {
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)wParam;
        mpsRFCalVerTestParam = (PS_RFCALVER_TEST_PARAM)lParam;
        CoInitialize(NULL);
        if (!LoadTestSuiteManageModule() 
            || !GetTestSuiteManageClassPtr())
        {
            MessageBox(mpsRFCalVerTestParam->mhMainWnd/*FindWindow(NULL,NULL)*/,GetString(E_TSE_LOAD_MODULE_FAIL),_T("Error"),MB_ICONERROR);
            PostMessage(mpsRFCalVerTestParam->mhMainWnd,WM_HWND_CLOSE_SUB_PLUGIN_WINDOWS,NULL,NULL);
            return ;
        }
        mpSequenceManageImpl = mptrTestSuiteManageImpl->GetSequenceManageImpl();
        mpSequenceManageImpl->SetOnLogListener(this);
        mpSequenceManageImpl->SetOnTestStatusListener(this);
        InitTestSuiteManageConfig();
		PostMessage(mpsRFCalVerTestParam->mhMainWnd,WM_HWND_IS_SHOW_TEST_STATUS_WINDOW_MSG,WPARAM(mpSequenceManageImpl->GetSequenceTestImpl()->TSEAutoTestNotEmpty()?0:1),NULL);
    }

    void CCRFCalVerTestThread::OnRunThread(WPARAM wParam,LPARAM lParam)
    {  
        if (!mbInitTSEConfig || !OnStartSuiteTest())
        {
            mbInitPreTSETest = false;
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
            return ;
        }
        mbInitPreTSETest = false;
        mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
    }

	void CCRFCalVerTestThread::OnAutoTSETest(WPARAM wParam,LPARAM lParam)
	{
		if (!mbInitTSEConfig || !mpSequenceManageImpl->GetSequenceTestImpl()->TSEAutoTestNotEmpty()) return ;
        if (mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEAutoTest()) { mpAsyncWndMessage->PutAsyncMessage(WM_HWND_TEST_SATRT_MSG);mbInitPreTSETest = true;}
	}

    void CCRFCalVerTestThread::SendListMsgToWnd(const wchar_t* strMsg,DWORD dwRGB)
    {
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_TEST_LIST_MSG,strMsg,LPARAM(dwRGB));
    }

	void CCRFCalVerTestThread::OnExecuteConfigViewTest(WPARAM wParam,LPARAM lParam)
	{
		if (!mbInitTSEConfig) return;
		mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEConfigTest();
	}

    bool CCRFCalVerTestThread::InitTestSuiteManageConfig()
    {
        if (mbInitTSEConfig) return true;
        mbInitTSEConfig = mpSequenceManageImpl->GetSequenceTestImpl()->LoadTSESuiteConfig(JGW_W2A(mpsRFCalVerTestParam->mstrConfigXmlPath));
        if (!mbInitTSEConfig) {mpAsyncWndMessage->PutAsyncMessage(WM_HWND_INIT_TSE_TEST_FAIL_MSG); return false;}
        mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutInt(TSE_GLOBAL_MAIN_HWND,(int)mpsRFCalVerTestParam->mhMainWnd);
        mpSequenceManageImpl->GetSequenceTestImpl()->InitTSETestConfig();
        if (!mbInitTSEConfig) mpAsyncWndMessage->PutAsyncMessage(WM_HWND_INIT_TSE_TEST_FAIL_MSG);
        return mbInitTSEConfig;
    }

    bool CCRFCalVerTestThread::LoadTestSuiteManageModule()
    {
        if (mptrTestSuiteManageImpl) return true;
        std::wstring strTestSuiteManageModule(JGW_GetApplicationFolder());
        strTestSuiteManageModule += _T(TEST_SUITE_MANAGE_PLUGIN_NAME);

        return mTestSuitePocoClassLoader.LoadTestSuiteModule(strTestSuiteManageModule);
    }

    bool CCRFCalVerTestThread::GetTestSuiteManageClassPtr()
    {
        if (mptrTestSuiteManageImpl) return true;
        mptrTestSuiteManageImpl = (mTestSuitePocoClassLoader.CreateTestSuitePtrClass(std::string("CTSE_SequenceManageExport")));
        return (NULL != mptrTestSuiteManageImpl);
    }

    bool CCRFCalVerTestThread::OnStartSuiteTest()
    {
        //! 如果已经初始化AUTO TSE测试则忽略，否则重新初始化
        if (!mbInitPreTSETest && mpSequenceManageImpl->GetSequenceTestImpl()->TSEAutoTestNotEmpty() && !mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEAutoTest()) return false;
        return mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEAllSuiteTest(true);
    }
}