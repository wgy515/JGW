#include "StdAfx.h"
#include "TSE_SequenceTestNet.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>

namespace JGW
{
    CTSE_SequenceTestNet::CTSE_SequenceTestNet(CTSE_LogNet& logServices,CTSE_GlobalEnvironment& globalEnvironment) : mLogServices(logServices),mGlobalEnvironment(globalEnvironment)
    {
    }


    CTSE_SequenceTestNet::~CTSE_SequenceTestNet(void)
    {
    }

    bool CTSE_SequenceTestNet::LoadTSEModule(const wchar_t* moduleName)
    {
        std::wstring wstrSrcModuleName = JGW_RealativePathToAbsPath(moduleName);

        if(!mTestSuitePocoClassLoader.LoadTestSuiteModule(wstrSrcModuleName))
        {
            mLogServices.ErrorLogFormat(L"Load TSE Module(%s) Fail(%s)",wstrSrcModuleName.c_str(),JGW_GetSystemErrorString(GetLastError()).c_str());
            return false;
        }

        if (!SetTSEModuleGlobalResourcesInfo(wstrSrcModuleName))
        {
            mLogServices.ErrorLog(L"SetTSEModuleGlobalResourcesInfo Fail");
            return false;
        }

        return true;
    }

    bool CTSE_SequenceTestNet::SetTSEModuleGlobalResourcesInfo(const std::wstring& strTSEModuleFilePath)
    {
        std::string strTestSuiteModuleFilePath = JGW_W2A(strTSEModuleFilePath.c_str(),CP_UTF8);
        const Poco::Manifest<CTSE_TestBase>* pManif = mTestSuitePocoClassLoader.GetClassLoaderModule().findManifest(strTestSuiteModuleFilePath);
        if (!pManif) return false;
        Poco::Manifest<CTSE_TestBase>::Iterator itm = pManif->find(TSE_MODULE_GLOBAL_RESOURCES_CLSSNAM);
        if (pManif->end() == itm) return true;
        CTSE_TestBase* pTestBase  = itm->create();
        //! 将service下发下去
        if (NULL != pTestBase)
        {
            pTestBase->TSE_AddParam(TSE_LOG_SERVICES,(const wchar_t*)&mLogServices);
            pTestBase->TSE_AddParam(TSE_GLOBAL_ENVIRONMENT,(const wchar_t*)&mGlobalEnvironment);
            itm->destroy(pTestBase);
            //ptrTestBase->TSE_AddParam(TSE_TESTSTATUS_SERVICES,(const wchar_t*)&mTestStatusService);
            //mvSuiteGlobalResources.push_back(ptrTestBase);
        }
          return true;
    }

    CTSE_TestBase* CTSE_SequenceTestNet::CreateTSEInstance(const wchar_t* realName, const wchar_t* moduleName)
    {
        std::wstring wstrSrcModuleName = JGW_RealativePathToAbsPath(moduleName);
        std::string strTestSuiteModuleFilePath = JGW_W2A(wstrSrcModuleName,CP_UTF8);
        std::string realClassName = JGW_W2A_W(realName);
        CTSE_TestBase* pTestBase = mTestSuitePocoClassLoader.CreateAutoDeleteTestSuitePtrClassToModuleName(realClassName,strTestSuiteModuleFilePath);
        /*std::shared_ptr<CTSE_TestBase> ptrTestBase (pTestBase);
        if (ptrTestBase)
        {
        mvAllSuiteTestBase.push_back(ptrTestBase);
        return pTestBase;
        }*/
        return pTestBase;
    }

    bool CTSE_SequenceTestNet::UpdateConfigParamToTSEInstance(CTSE_TestBase* pTestBase,const std::wstring& paramName,const std::wstring& paramValue)
    {
        if (pTestBase)
        {
            return pTestBase->TSE_AddParam(paramName.c_str(),paramValue.c_str());
        }
        return false;
    }

    bool CTSE_SequenceTestNet::InitTestToTSEInstance(CTSE_TestBase* pTestBase)
    {
        if (pTestBase)
        {
            return pTestBase->TSE_Init();
        }
        return false;
    }

    bool CTSE_SequenceTestNet::ExecuteTSEInstance(CTSE_TestBase* pTestBase)
    {
        if (pTestBase)
        {
            return pTestBase->TSE_Run();
        }
        return false;
    }

    bool CTSE_SequenceTestNet::ExitTestToTSEInstance(CTSE_TestBase* pTestBase)
    {
        if (pTestBase)
        {
            return pTestBase->TSE_Exit();
        }
        return false;
    }

    bool CTSE_SequenceTestNet::UnLoadTSEModule(const std::wstring& moduleName)
    {
        std::wstring wstrSrcModuleName = JGW_RealativePathToAbsPath(moduleName.c_str());
        std::string strTestSuiteModuleFilePath = JGW_W2A(wstrSrcModuleName,CP_UTF8);

        if (mTestSuitePocoClassLoader.GetClassLoaderModule().isLibraryLoaded(strTestSuiteModuleFilePath))
        {
             mTestSuitePocoClassLoader.GetClassLoaderModule().unloadLibrary(strTestSuiteModuleFilePath);
        }
        return true;
    }
}

