#pragma once
#include <TSE_SequenceManagerPlugin/TSE_GlobalEnvironment.h>
#include "TSE_LogNet.h"
#include "TSE_SequenceTestNet.h"
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_MESDataBase/CJGW_MESDataBaseImpl.h>
#include "CJGW_MESConfig.h"
#include <JGW_TestLoggingPlugin/CJGW_MoveTestLogging.h>
#include "CJGW_HtmlQdartResolveConfig.h"
#include "CJGW_ResolveQdartHtml.h"

namespace JGW
{
    class CCTSE_ManagerSource
    {
    public:
        CCTSE_ManagerSource(void);
        ~CCTSE_ManagerSource(void);
    public:
        //! 
        bool LoadTSEModule(const std::wstring& moduleName);
        //! 
        CTSE_TestBase* CreateTSEInstance(const std::wstring& realName, const std::wstring& moduleName);
        //! 
        bool UpdateConfigParamToTSEInstance(CTSE_TestBase* instance, const std::wstring& paramName, const std::wstring& paramValue);
        //! 
        bool InitTestToTSEInstance(CTSE_TestBase* instance);
        //! 
        bool ExecuteTSEInstance(CTSE_TestBase* instance);
        //!
        bool ExitTestToTSEInstance(CTSE_TestBase* instance);
        //! 
        int GetGlobalEnvironment(const std::wstring& key,wchar_t* val);
        //!
        void SetGlobalEnvironment(const std::wstring& key,const std::wstring& val);
        //!
        void UnLoadTSEModule(const std::wstring& moduleName);
        //! 
        bool LoadTSEMESConfig(const std::wstring& mesConfigPath);
        //! 
        void UpdateTSEMESOperation(const std::wstring& operationID);
        //! 
        void UpdateTSEMESUserPassword(const std::wstring& userName,const std::wstring& password);
        //! 
        bool LoadHtmlResolveRuleFilePath(const std::wstring& htmlResolveRuleFilePath);
        //! 
        const wchar_t* ResolveHtmlFileInfo(const std::wstring& htmlFilePath,int showTestLogType,bool saveTestLogging,const std::wstring& sn,bool testResult);
    private:
        void SaveHtmlLogging(bool bTestResult,const std::wstring& strLogFilePath,const std::wstring& strSN);
    private:
        S_MES_INFO msMESInfo;
        S_HTML_QDART_RESOLVE_CONFIG_INFO msHtmlQdartResolveConfigInfo;
        CTSE_GlobalEnvironment mGlobalEnvironment;
        CTSE_LogNet mLogNet;
        CTSE_SequenceTestNet mSequenceTestNet;
        CCJGW_MoveTestLogging mMoveTestLogging;
        std::wstring mstrLog;
    };
}


