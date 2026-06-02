#pragma once
#include <JGW_TestSuiteGlobalResourcesLibrary/CJGW_TestSuiteLogResourceImpl.h>
#include <JGW_TestSuiteManagePlugin/CJGW_TestSuiteManage_Enum_Define.h>
namespace JGW
{
    class CCJGW_TestSuiteLogResource : public CCJGW_TestSuiteLogResourceImpl
    {
    public:
        CCJGW_TestSuiteLogResource(void);
        ~CCJGW_TestSuiteLogResource(void);

        virtual void RegisterSuiteLogCallback(const int nIndex,jgw_test_suite_log_resource_func& pSuiteLogResFunc);

        static void DispatchSuiteTestNameMsg(const wchar_t* strSuiteTestName);

        static void DispatchSuiteTestLogMsg(E_TEST_SUITE_TYPE eTestSuiteType,const wchar_t* strSuiteTestName);

        static void DispatchSuiteTestLogMsgFormat(E_TEST_SUITE_TYPE eTestSuiteType,const wchar_t* strSuiteTestLog,...);
    private:
        static jgw_test_suite_log_resource_func	mpSuiteLog;
        static int						mnIndex;
    };
}


