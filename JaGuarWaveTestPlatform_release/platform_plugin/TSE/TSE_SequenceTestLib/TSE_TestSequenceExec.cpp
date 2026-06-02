#include "StdAfx.h"
#include <TSE_SequenceTestLib/TSE_TestSequenceExec.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <ObjBase.h>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
namespace JGW
{
    CTSE_TestSequenceExec::CTSE_TestSequenceExec(void) : mpSequenceManageImpl(NULL),mptrTestSuiteManageImpl(NULL),mbExecAutoTSETest(false),mbIsLuaTSEConfig(false),mnTokenID(0),mbMultiTSETest(false)
    {
		//mnTokenID = (unsigned long)this;
    }


    CTSE_TestSequenceExec::~CTSE_TestSequenceExec(void)
    {
        ExitTSE();
    }

    void CTSE_TestSequenceExec::ExitTSE()
    {
        if (mpSequenceManageImpl)
        {
            if (mptrTestSuiteManageImpl) mptrTestSuiteManageImpl->ReleaseSequenceManageImpl(mpSequenceManageImpl);
            mpSequenceManageImpl = NULL;
            CoUninitialize();
        }
    }

    bool CTSE_TestSequenceExec::IsInitTSEConfigOk()
    {
        return mbInitTSEConfig;
    }

    void CTSE_TestSequenceExec::OnDebugLog(const wchar_t* strDebugLog)
    {

    }

    void CTSE_TestSequenceExec::OnErrorLog(const wchar_t* strErrorLog)
    {

    }

    void CTSE_TestSequenceExec::OnInfoLog(const wchar_t* strInfoLog)
    {

    }

    void CTSE_TestSequenceExec::OnUserDefinedLog(int id,const wchar_t* strCustomLog)
    {

    }

    void CTSE_TestSequenceExec::OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus)
    {

    }

    void CTSE_TestSequenceExec::OnTestIndexChange(size_t index,int subIndex)
    {

    }

    bool CTSE_TestSequenceExec::LoadTestSuiteManageModule(const std::string& strConfigXmlPath)
    {
#if 0
        if (mptrTestSuiteManageImpl) return true;
        std::wstring strTestSuiteManageModule(JGW_GetApplicationFolder());
        strTestSuiteManageModule += _T(TEST_SUITE_MANAGE_PLUGIN_NAME);
        return mTestSuitePocoClassLoader.LoadTestSuiteModule(strTestSuiteManageModule);
#else
		if (mptrTestSuiteManageImpl) return true;
		std::wstring strTestSuiteManageModule(JGW_GetApplicationFolder());

		std::string strConfigXmlFileName = JGW_GetFileNameToFilePathA(strConfigXmlPath.c_str());
		if (NULL == JGW_StrCaseStr(strConfigXmlPath.c_str(),".lua"))
		{
			mbIsLuaTSEConfig = false;
			strTestSuiteManageModule += _T(TEST_SUITE_MANAGE_PLUGIN_NAME);
			return mTestSuitePocoClassLoader.LoadTestSuiteModule(strTestSuiteManageModule);
		}
		else
		{
			mbIsLuaTSEConfig = true;
			strTestSuiteManageModule += _T("TSE_SequenceManagerLuaPlugin.dll");
			return mTestSuitePocoClassLoader.LoadTestSuiteModule(strTestSuiteManageModule);
		}
#endif
    }

    bool CTSE_TestSequenceExec::GetTestSuiteManageClassPtr()
    {
#if 0
        if (mptrTestSuiteManageImpl) return true;
        mptrTestSuiteManageImpl = (mTestSuitePocoClassLoader.CreateTestSuitePtrClass(std::string("CTSE_SequenceManageExport")));
        return (NULL != mptrTestSuiteManageImpl);
#else
		if (mptrTestSuiteManageImpl) return true;
		if (!mbIsLuaTSEConfig)
		{
			mptrTestSuiteManageImpl = (mTestSuitePocoClassLoader.CreateTestSuitePtrClass(std::string("CTSE_SequenceManageExport")));
		}
		else
		{
			mptrTestSuiteManageImpl = (mTestSuitePocoClassLoader.CreateTestSuitePtrClass(std::string("CTSE_SequenceManageLuaExport")));
		}
		return (NULL != mptrTestSuiteManageImpl);
#endif
    }

    bool CTSE_TestSequenceExec::InitSequenceTest(HWND hMainWnd,const std::string& strConfigXmlPath)
    {
        CoInitialize(NULL);
        if (!LoadTestSuiteManageModule(strConfigXmlPath) 
            || !GetTestSuiteManageClassPtr())
        {
            MessageBox(hMainWnd,GetString(E_TSE_LOAD_MODULE_FAIL),_T("Error"),MB_ICONERROR);
            PostMessage(hMainWnd,WM_HWND_CLOSE_SUB_PLUGIN_WINDOWS,NULL,NULL);
            return false;
        }
        if (mpSequenceManageImpl) mptrTestSuiteManageImpl->ReleaseSequenceManageImpl(mpSequenceManageImpl);
        mpSequenceManageImpl = mptrTestSuiteManageImpl->GetSequenceManageImplToTokenID(mnTokenID);
        mpSequenceManageImpl->SetOnLogListener(this);
        mpSequenceManageImpl->SetOnTestStatusListener(this);
		if (NULL != mpSequenceManageImpl->GetGlobalEnvironmentImpl()) mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutBool(MULTIUNIT_TSE_TEST,mbMultiTSETest);

        return InitTestSuiteManageConfig(hMainWnd,strConfigXmlPath);
    }

    bool CTSE_TestSequenceExec::InitTestSuiteManageConfig(HWND hMainWnd,const std::string& strConfigXmlPath)
    {
        mbInitTSEConfig = true;
        mbInitTSEConfig = mpSequenceManageImpl->GetSequenceTestImpl()->LoadTSESuiteConfig(strConfigXmlPath);
        if (mbInitTSEConfig)
        {
            mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutInt(TSE_GLOBAL_MAIN_HWND,(int)hMainWnd);
			//! MULTIUNIT_TSE_TEST
            mpSequenceManageImpl->GetSequenceTestImpl()->InitTSETestConfig();
            PostMessage(hMainWnd,WM_HWND_IS_SHOW_TEST_STATUS_WINDOW_MSG,WPARAM(mpSequenceManageImpl->GetSequenceTestImpl()->TSEAutoTestNotEmpty()?0:1),NULL);
        }
        return mbInitTSEConfig;
    }

    bool CTSE_TestSequenceExec::ExecSequenceTest()
    {
        if (!mbInitTSEConfig) return false;
        //! 如果已经初始化AUTO TSE测试则忽略，否则重新初始化
        if (!mbExecAutoTSETest && mpSequenceManageImpl->GetSequenceTestImpl()->TSEAutoTestNotEmpty() && !mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEAutoTest()) return false;
        return mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEAllSuiteTest(true);
    }
// 
//     void CTSE_TestSequenceExec::OnAutoTSETest(WPARAM wParam,LPARAM lParam)
//     {
//        
//     }
}