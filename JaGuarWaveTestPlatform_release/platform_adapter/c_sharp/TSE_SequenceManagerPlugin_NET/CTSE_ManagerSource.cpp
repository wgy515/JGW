#include "StdAfx.h"
#include "CTSE_ManagerSource.h"

namespace JGW
{
    enum EHTML_SHOW_LOG_TYPE
    {
        //! 0 : 不显示PASS和FAIL LOG
        E_NOT_SHOW_PASS_AND_FAIL_LOG = 0,
        //! 1 : TSET FAIL 显示FAIL LOG 但不显示PASS LOG
        E_TF_SHOW_FAIL_NOT_PASS_LOG = 1,
        //! 2 : TSET FAIL 显示所有LOG
        E_TF_SHOW_ALL_LOG = 2,
        //! 3 : TSET PASS 显示所有LOG
        E_TP_SHOW_ALL_LOG = 3
    };

    CCTSE_ManagerSource::CCTSE_ManagerSource(void) : mSequenceTestNet(mLogNet,mGlobalEnvironment)
    {
    }


    CCTSE_ManagerSource::~CCTSE_ManagerSource(void)
    {
    }

    bool CCTSE_ManagerSource::LoadTSEModule(const std::wstring& moduleName)
    {
        return mSequenceTestNet.LoadTSEModule(moduleName.c_str());
    }

    CTSE_TestBase* CCTSE_ManagerSource::CreateTSEInstance(const std::wstring& realName, const std::wstring& moduleName)
    {
        return mSequenceTestNet.CreateTSEInstance(realName.c_str(),moduleName.c_str());
    }

    bool CCTSE_ManagerSource::UpdateConfigParamToTSEInstance(CTSE_TestBase* instance, const std::wstring& paramName, const std::wstring& paramValue)
    {
        return mSequenceTestNet.UpdateConfigParamToTSEInstance(instance,paramName,paramValue);
        //! return instance->TSE_AddParam(paramName.c_str(),paramValue.c_str());
    }

    bool CCTSE_ManagerSource::InitTestToTSEInstance(CTSE_TestBase* instance)
    {
        return mSequenceTestNet.InitTestToTSEInstance(instance);
    }

    bool CCTSE_ManagerSource::ExecuteTSEInstance(CTSE_TestBase* instance)
    {
        return mSequenceTestNet.ExecuteTSEInstance(instance);
    }

    bool CCTSE_ManagerSource::ExitTestToTSEInstance(CTSE_TestBase* instance)
    {
        return mSequenceTestNet.ExitTestToTSEInstance(instance);
    }

    int CCTSE_ManagerSource::GetGlobalEnvironment(const std::wstring& key,wchar_t* val)
    {
        std::wstring value = mGlobalEnvironment.GetString(key);
        wcscpy_s(val,1023,value.c_str());

        return wcslen(val);
    }

    void CCTSE_ManagerSource::SetGlobalEnvironment(const std::wstring& key,const std::wstring& val)
    {
        mGlobalEnvironment.PutString(key,val);
    }

    void CCTSE_ManagerSource::UnLoadTSEModule(const std::wstring& moduleName)
    {
        mSequenceTestNet.UnLoadTSEModule(moduleName);
    }

    bool CCTSE_ManagerSource::LoadTSEMESConfig(const std::wstring& mesConfigPath)
    {
        JGW::CCJGW_MESConfig mesConfig(mLogNet);
        if (mesConfig.LoadMESIniConfig(mesConfigPath,&msMESInfo))
        {
            GetMESDataBaseAdo()->SetMESServiceSocketConnect(&msMESInfo);
            return true;
        }
        return false;
    }

    void CCTSE_ManagerSource::UpdateTSEMESOperation(const std::wstring& operationID)
    {
        msMESInfo.mstrOperationId = JGW_W2A(operationID);
    }

    void CCTSE_ManagerSource::UpdateTSEMESUserPassword(const std::wstring& userName,const std::wstring& password)
    {
        msMESInfo.msMesConnectInfo.mstrUsername = JGW_W2A(userName);
        msMESInfo.msMesConnectInfo.mstrUserpwd = JGW_W2A(password);
    }

    bool CCTSE_ManagerSource::LoadHtmlResolveRuleFilePath(const std::wstring& htmlResolveRuleFilePath)
    {
#if 1
        JGW::CCJGW_HtmlQdartResolveConfig htmlQdartResolveConfig;
        return htmlQdartResolveConfig.LoadHtmlQdartResolveConfig(htmlResolveRuleFilePath,msHtmlQdartResolveConfigInfo);
#else
        return true;
#endif
    }

    void CCTSE_ManagerSource::SaveHtmlLogging(bool bTestResult,const std::wstring& strLogFilePath,const std::wstring& strSN)
    {    
        mMoveTestLogging.MoveTestLogging(mGlobalEnvironment.GetString(TSE_TEST_PROJECT_NAME).c_str(),strSN.c_str(),strLogFilePath.c_str(),bTestResult);
        //!if (!mbca) return ;
        //! 保存_Log_ConsoleApp文件
        std::wstring strLogConsoleApp(strLogFilePath);
        JGW::JGW_ReplaceStringW(strLogConsoleApp,L".html",L"");
        strLogConsoleApp += L"_Log_ConsoleApp.txt";
        if (JGW::JGW_FileExistsToFilePath(strLogConsoleApp.c_str()))
        {
            mMoveTestLogging.MoveTestLogging(mGlobalEnvironment.GetString(TSE_TEST_PROJECT_NAME).c_str(),strSN.c_str(),strLogConsoleApp.c_str(),bTestResult);
        }    
    }

    const wchar_t* CCTSE_ManagerSource::ResolveHtmlFileInfo(const std::wstring& htmlFilePath,int showTestLogType,bool saveTestLogging,const std::wstring& sn,bool testResult)
    {
        if (saveTestLogging)
        {
            SaveHtmlLogging(testResult,htmlFilePath,sn);
        }
        bool mbTestResult = testResult;
        EHTML_SHOW_LOG_TYPE meShowTestLogType = (EHTML_SHOW_LOG_TYPE)showTestLogType;

        if (mbTestResult && meShowTestLogType != E_TP_SHOW_ALL_LOG)
        {
            return L"";
        }

        bool bIsParsePassProject = false;
        if (meShowTestLogType == E_TF_SHOW_ALL_LOG || meShowTestLogType == E_TP_SHOW_ALL_LOG)
        {
            bIsParsePassProject = true;
        }
#if 1
        JGW::CCJGW_ResolveQdartHtml resolveQdartHtml;
        resolveQdartHtml.LoadResolveQdartHtml(JGW::JGW_W2A(htmlFilePath),msHtmlQdartResolveConfigInfo,bIsParsePassProject);

        if (bIsParsePassProject)
        {
            mstrLog = JGW::JGW_A2W(resolveQdartHtml.GetTestResult(),CP_UTF8);
            //std::vector<std::string> vTemp;
            //JGW::JGW_ParserStrA(resolveQdartHtml.GetTestResult().c_str(),"\n",vTemp);
            //for (size_t i = 0;i < vTemp.size();i ++)
            //{
            //	LogI(JGW_A2W(vTemp[i],CP_UTF8).c_str());
            //}
        }
        if (!mbTestResult)
        {
            mstrLog = JGW::JGW_A2W(resolveQdartHtml.GetErrorTestResult(),CP_UTF8);
            //std::vector<std::string> vTemp;
            //JGW::JGW_ParserStrA(resolveQdartHtml.GetErrorTestResult().c_str(),"\n",vTemp);
            //for (size_t i = 0;i < vTemp.size();i ++)
            //{
            //	LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,JGW_A2W(vTemp[i],CP_UTF8).c_str());
            //}
        }
#endif

        return mstrLog.c_str();
    }
}