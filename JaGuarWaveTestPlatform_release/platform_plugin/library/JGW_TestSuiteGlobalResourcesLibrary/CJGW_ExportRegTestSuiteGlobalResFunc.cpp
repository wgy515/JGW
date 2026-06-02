#include "stdafx.h"
#include <JGW_TestSuiteGlobalResourcesLibrary\CJGW_TestSuiteLogResource.h>
#include <JGW_TestSuiteGlobalResourcesLibrary\CJGW_TestSuiteEnvironmentResource.h>
#include "CJGW_TestSuiteGlobalResourcesLibrary_Define.h"
#include <JGW_TestSuiteGlobalResourcesLibrary/CJGW_ExportRegTestSuiteGlobalResFunc.h>

namespace JGW
{
    void RegisterSuiteLogCallback(const int nIndex,jgw_test_suite_log_resource_func& pSuiteLogResFunc)
    {
        CCJGW_TestSuiteLogResource testSuiteLogRes;
        testSuiteLogRes.RegisterSuiteLogCallback(nIndex,pSuiteLogResFunc);
    }

    void SetSuiteEnvironmentFunc(JGW_SetSuiteEnvironmentVarFunc pset_suite_env_func,JGW_GetSuiteEnvironmentVarFunc pget_suite_env_func)
    {
        CCJGW_TestSuiteEnvironmentResource testSuiteEnvRes;
        testSuiteEnvRes.SetSuiteEnvironmentFunc(pset_suite_env_func,pget_suite_env_func);
    }
}