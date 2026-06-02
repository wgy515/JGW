#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <TSE_SequenceManagerPlugin/TSE_SequencePocoClassLoader.hpp>

#include "TSE_SequenceResourceManage.h"

namespace JGW
{
    class CTSE_SequenceTestInteropManager
    {
    public:
        CTSE_SequenceTestInteropManager(CTSE_SequenceResourceManage& sequenceResourceManage);
        ~CTSE_SequenceTestInteropManager(void);
    public:
        void UnLoadTSEModule();
        //! 加载TSE相应模块
        bool LoadTSEModuleToModuleName(const std::wstring& strModuleName);
        //!
        CTSE_TestBase* GetTestSuitePtrClassToModuleName(const std::wstring& strClassName,const std::wstring& strModuleName);
        //!
        void UpdateTestSuiteConfigParam(CTSE_TestBase* ptrTestBase,const std::wstring& strParamName,const std::wstring& strParamValue);
        //!
        bool ExecTestSuite(CTSE_TestBase* ptrTestBase,bool isReversedResult);
        //!
        void ReleaseTSEInstance(void* pSeqTestInstance);

        void UpdateTSEGlobalEnvironment(const std::wstring& strEnvName,const std::wstring& strEnvValue);
        //! bool InitTSETestSuiteConfig(CTSE_TestBase* ptrTestBase);
        const wchar_t* GetTSEGlobalEnvironment(const std::wstring& strEnvName);
        //!
        void ClearGlobalEnvironment();
    private:
        //! 设置TSE全局模块资源信息
        bool SetTSEModuleGlobalResourcesInfo(std::string strTSEModuleFilePath);
    private:
        CTSE_SequenceResourceManage& mSequenceResourceManage;
        std::vector<std::wstring> mvModuleName;
        std::vector<std::shared_ptr<CTSE_TestBase>> mvPtrSuiteTestBase;
        //! 共享的全局资源组
        std::vector<std::shared_ptr<CTSE_TestBase>> mvSuiteGlobalResources;
        CTSE_SequencePocoClassLoader<CTSE_TestBase> mTestSuitePocoClassLoader;
    };
}