#include "StdAfx.h"
#include "TSE_SequenceTestInteropManager.h"
#include <JGW_FoundationFunc/vecfunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CTSE_SequenceTestInteropManager::CTSE_SequenceTestInteropManager(CTSE_SequenceResourceManage& sequenceResourceManage) : mSequenceResourceManage(sequenceResourceManage)
    {
    }


    CTSE_SequenceTestInteropManager::~CTSE_SequenceTestInteropManager(void)
    {
        //UnLoadTSEModule();
    }

    void CTSE_SequenceTestInteropManager::UnLoadTSEModule()
    {
        mvPtrSuiteTestBase.clear();
        mSequenceResourceManage.GetGlobalEnvironment().ClearGlobalEnvironment();
        //! 清空全局资源
        for (size_t i = 0;i < mvSuiteGlobalResources.size();i ++)
        {
            mvSuiteGlobalResources.at(i)->TSE_AddParam(TSE_LOG_SERVICES,NULL);
            mvSuiteGlobalResources.at(i)->TSE_AddParam(TSE_GLOBAL_ENVIRONMENT,NULL);
            mvSuiteGlobalResources.at(i)->TSE_AddParam(TSE_TESTSTATUS_SERVICES,NULL);
        }
        mvSuiteGlobalResources.clear();
        mTestSuitePocoClassLoader.UnloadTestSuiteModule();
    }

    bool CTSE_SequenceTestInteropManager::LoadTSEModuleToModuleName(const std::wstring& strModuleName)
    {
        std::wstring strModulePath = JGW_RealativePathToAbsPath(strModuleName.c_str());
        std::string strTestSuiteModuleFilePath = JGW_W2A(strModulePath,CP_UTF8);
        //! 如果已经加载
        for (std::vector<std::wstring>::iterator it = mvModuleName.begin();
            it != mvModuleName.end();
            ++ it)
        {
            if (0 == JGW_WStrComparenoCaseWStr(it->c_str(),strModulePath.c_str()))
            {
                mSequenceResourceManage.GetLogServices().DebugLogFormat(L"TSE Module Is Loaded(%s)",strModulePath.c_str());
                return true;
            }
        }

        mvModuleName.push_back(strModulePath);
        mSequenceResourceManage.GetLogServices().DebugLogFormat(L"Start Load TSE Test Module(%s)",strModulePath.c_str());
        if (!mTestSuitePocoClassLoader.LoadTestSuiteModule(strModulePath))
        {
            mSequenceResourceManage.GetLogServices().ErrorLogFormat(L"LoadTSETestModule(%s) Fail",strModulePath.c_str());
            return false;
        }

        //strTestSuiteModuleFilePath = U(strTestSuiteModuleFilePath.c_str());
        if (!SetTSEModuleGlobalResourcesInfo(strTestSuiteModuleFilePath))
        {
            mSequenceResourceManage.GetLogServices().ErrorLogFormat(L"Set TSE Module Global Resources Info Fail(%s)",strModulePath.c_str());
            return false;
        }

        return true;
    }

    bool CTSE_SequenceTestInteropManager::SetTSEModuleGlobalResourcesInfo(std::string strTSEModuleFilePath)
    {
        const Poco::Manifest<CTSE_TestBase>* pManif = mTestSuitePocoClassLoader.GetClassLoaderModule().findManifest(strTSEModuleFilePath);
        if (!pManif)
        {
            mSequenceResourceManage.GetLogServices().ErrorLogFormat(L"%s Not Find Manifest",JGW_A2W(strTSEModuleFilePath,CP_UTF8).c_str());
            return false;
        }

        Poco::Manifest<CTSE_TestBase>::Iterator itm = pManif->find(TSE_MODULE_GLOBAL_RESOURCES_CLSSNAM);
        if (pManif->end() == itm)
        {
             mSequenceResourceManage.GetLogServices().ErrorLogFormat("%s Not Find Class Name %s",JGW_A2W(strTSEModuleFilePath,CP_UTF8).c_str(),TSE_MODULE_GLOBAL_RESOURCES_CLSSNAM);
             return true;
        }

        std::shared_ptr<CTSE_TestBase> ptrTestBase (itm->create());
        //! 将service下发下去
        if (ptrTestBase)
        {
            ptrTestBase->TSE_AddParam(TSE_LOG_SERVICES,(const wchar_t*)&mSequenceResourceManage.GetLogServices());
            ptrTestBase->TSE_AddParam(TSE_GLOBAL_ENVIRONMENT,(const wchar_t*)&mSequenceResourceManage.GetGlobalEnvironment());
            ptrTestBase->TSE_AddParam(TSE_TESTSTATUS_SERVICES,(const wchar_t*)&mSequenceResourceManage.GetTestStatusService());
            mvSuiteGlobalResources.push_back(ptrTestBase);
        }
        return true;
    }

    CTSE_TestBase* CTSE_SequenceTestInteropManager::GetTestSuitePtrClassToModuleName(const std::wstring& strClassName,const std::wstring& strModuleName)
    {
        std::string className = JGW_W2A(strClassName),moduleName = JGW_W2A(JGW_RealativePathToAbsPath(strModuleName.c_str()),CP_UTF8);
        std::shared_ptr<CTSE_TestBase> ptrTestBase ;
        if (!strModuleName.empty())
        {
            //moduleName = U(moduleName.c_str());
            mSequenceResourceManage.GetLogServices().DebugLogFormat("GetTestSuitePtrClassToModuleName TSE Module (%s),Class Name %s",moduleName.c_str(),className.c_str());
            ptrTestBase.reset(mTestSuitePocoClassLoader.CreateTestSuitePtrClassToModuleName(className,moduleName));
        }
        else
        {
            mSequenceResourceManage.GetLogServices().DebugLogFormat("GetTestSuitePtrClassToModuleName TSE Class Name %s",className.c_str());
            ptrTestBase.reset(mTestSuitePocoClassLoader.CreateTestSuitePtrClassNotAutoDelete(className));
        }
        mSequenceResourceManage.GetLogServices().DebugLogFormat("TSE Class Ptr %d",ptrTestBase.get());
        mvPtrSuiteTestBase.push_back(ptrTestBase);
        return ptrTestBase.get();
    }

    void CTSE_SequenceTestInteropManager::UpdateTestSuiteConfigParam(CTSE_TestBase* ptrTestBase,const std::wstring& strParamName,const std::wstring& strParamValue)
    {
        if (!ptrTestBase || NULL == strParamName.compare(TSE_CONFIG_MODULENAME) 
            || NULL == strParamName.compare(TSE_CONFIG_REALNAME)) return;

        ptrTestBase->TSE_AddParam(strParamName.c_str(),strParamValue.c_str());
    }

    bool CTSE_SequenceTestInteropManager::ExecTestSuite(CTSE_TestBase* ptrTestBase,bool isReversedResult)
    {
        bool bSigleResult = false;
        try
        {
            mSequenceResourceManage.GetTestStatusService().TestStatusChange(ptrTestBase->TSE_GetTestName().c_str(),E_RUN_TEST_STATUS);
            //! 错误重测次数
            for (size_t i = 0;i < 1/*ptrTestBase->TSE_GetErrorRetryTestCount()*/;i ++)
            {           
                if (0 != i) mSequenceResourceManage.GetTestStatusService().TestStatusChange(ptrTestBase->TSE_GetTestName().c_str(),E_RUNING_TEST_STATUS);
                bSigleResult = true;
                mSequenceResourceManage.GetLogServices().InfoLogFormat(L"Start Execute %s Test",ptrTestBase->TSE_GetTestName().c_str());
                bSigleResult &= ptrTestBase->TSE_Init();
                bSigleResult &= ptrTestBase->TSE_Run();
                bSigleResult &= ptrTestBase->TSE_Exit();
                if (isReversedResult)
                {
                    bSigleResult = !bSigleResult;
                }
                mSequenceResourceManage.GetTestStatusService().TestStatusChange(ptrTestBase->TSE_GetTestName().c_str(),bSigleResult?E_PASS_TEST_STATUS:E_FAIL_TEST_STATUS);
                if (bSigleResult) break;
            }
        }
        catch(...)
        {
            mSequenceResourceManage.GetLogServices().ErrorLog(L"Unknown error exception");
            return false;
        }

        return bSigleResult;
    }

    void CTSE_SequenceTestInteropManager::ReleaseTSEInstance(void* pSeqTestInstance)
    {
        std::vector<std::shared_ptr<CTSE_TestBase>>::iterator it = mvPtrSuiteTestBase.begin();
        while (it != mvPtrSuiteTestBase.end())
        {
            if (it->get() == pSeqTestInstance)
            {
                it = mvPtrSuiteTestBase.erase(it);
            }
            else
            {
                ++ it;
            }
        }
    }

    void CTSE_SequenceTestInteropManager::UpdateTSEGlobalEnvironment(const std::wstring& strEnvName,const std::wstring& strEnvValue)
    {
        mSequenceResourceManage.GetLogServices().InfoLogFormat(L"Update GlobalEnvironment %s \t %s",strEnvName.c_str(),strEnvValue.c_str());
        mSequenceResourceManage.GetGlobalEnvironment().PutString(strEnvName,strEnvValue);
    }

    const wchar_t* CTSE_SequenceTestInteropManager::GetTSEGlobalEnvironment(const std::wstring& strEnvName)
    {
        static std::wstring gstrEnvironmentValue;
        gstrEnvironmentValue = mSequenceResourceManage.GetGlobalEnvironment().GetString(strEnvName);
        return gstrEnvironmentValue.c_str();
    }

    void CTSE_SequenceTestInteropManager::ClearGlobalEnvironment()
    {
        mSequenceResourceManage.GetLogServices().InfoLog(L"Clear GlobalEnvironment");
        return mSequenceResourceManage.GetGlobalEnvironment().ClearGlobalEnvironment();
    }
}
