#include "StdAfx.h"
#include "TSE_SequenceManager.h"
#include "CJGW_SuiteEnvironment.h"
//! Sequence
namespace JGW
{
    TSE_SequenceManager::TSE_SequenceManager(bool bMultiSuite /* = false */,const int nIndex /* = 0 */):mTestSuiteInterface(bMultiSuite,nIndex)
    {
    }


    TSE_SequenceManager::~TSE_SequenceManager(void)
    {
    }

    bool TSE_SequenceManager::InitTestSuiteManageInterface(HWND hMain,LPCTSTR strConfigFilePath)
    {
        std::string strTemp;
        return mTestSuiteInterface.InitSuiteAssembly(hMain,strTemp,strConfigFilePath);
    }

    bool TSE_SequenceManager::InitTestSuiteManageInterfaceToXmlBuf(HWND hMain,std::string& strConfigFileBuf)
    {
        return mTestSuiteInterface.InitSuiteAssembly(hMain,strConfigFileBuf);
    }

    bool TSE_SequenceManager::ExecuteAllSuiteTest(bool bErrorStop /* = true */)
    {
        return mTestSuiteInterface.ExecuteAllTest(bErrorStop);
    }

    bool TSE_SequenceManager::ExecuteSingleSuiteTestToTestID(int nIndex)
    {
        return mTestSuiteInterface.ExecuteSingleTest(nIndex);
    }

    bool TSE_SequenceManager::ExecuteSuiteTestFinalize()
    {
        return mTestSuiteInterface.ExecuteTestFinalize();
    }

    void TSE_SequenceManager::Register_System_Event_Handler_Fn(jgw_suite_test_log_func evtcb)
    {
        mTestSuiteInterface.SetSuiteTestLogFuncPtr(evtcb);
    }

    void TSE_SequenceManager::SetSuiteEnvironmentVar(const wchar_t* envirname,const wchar_t* envirvalue)
    {
        CCJGW_SuiteEnvironment::SetSuiteEnvironmentVar(envirname,envirvalue);
    }

    std::wstring TSE_SequenceManager::GetSuiteEnvironmentVar(const wchar_t* envirname)
    {
        return CCJGW_SuiteEnvironment::GetSuiteEnvironmentVar(envirname);
    }

    size_t TSE_SequenceManager::GetSuiteTestCounts(void)
    {
        return mTestSuiteInterface.GetSuiteClassPtrCount();
    }

    void TSE_SequenceManager::UpdateEnviromentParam(void)
    {
        mTestSuiteInterface.UpdateEnviromentParam();
    }
}

