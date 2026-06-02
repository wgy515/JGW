#include "StdAfx.h"
#include <JGW_TestSuiteGlobalResourcesLibrary\CJGW_TestSuiteLogResource.h>
#include  <JGW_FoundationFunc/JGW_StringFunc.h>
#include <string>

namespace JGW
{
    jgw_test_suite_log_resource_func CCJGW_TestSuiteLogResource::mpSuiteLog = 0;
    int	CCJGW_TestSuiteLogResource::mnIndex = 0;
    CCJGW_TestSuiteLogResource::CCJGW_TestSuiteLogResource(void)
    {
    }


    CCJGW_TestSuiteLogResource::~CCJGW_TestSuiteLogResource(void)
    {
    }

    void CCJGW_TestSuiteLogResource::RegisterSuiteLogCallback(const int nIndex,jgw_test_suite_log_resource_func& pSuiteLogResFunc)
    {
        mpSuiteLog = pSuiteLogResFunc;
        mnIndex = nIndex;
    }

    void CCJGW_TestSuiteLogResource::DispatchSuiteTestNameMsg(const wchar_t* strSuiteTestName)
    {
        DispatchSuiteTestLogMsg(E_TEST_SUITE_TEST_NAME,strSuiteTestName);
    }

    void CCJGW_TestSuiteLogResource::DispatchSuiteTestLogMsgFormat(E_TEST_SUITE_TYPE eTestSuiteType,const wchar_t* strSuiteTestLog,...)
    {
        if (mpSuiteLog)
        {
            std::wstring strLog(L"");
            FORMAT_WSTRING(strLog,strSuiteTestLog);
            mpSuiteLog(mnIndex,eTestSuiteType,strLog.c_str());
        }
    }

    void CCJGW_TestSuiteLogResource::DispatchSuiteTestLogMsg(E_TEST_SUITE_TYPE eTestSuiteType,const wchar_t* strSuiteTestName)
    {
        if (mpSuiteLog) mpSuiteLog(mnIndex,eTestSuiteType,strSuiteTestName);
    }
}

