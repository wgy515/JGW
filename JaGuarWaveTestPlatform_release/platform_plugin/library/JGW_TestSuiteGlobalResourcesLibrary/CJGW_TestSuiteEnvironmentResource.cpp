#include "StdAfx.h"
#include <JGW_TestSuiteGlobalResourcesLibrary/CJGW_TestSuiteEnvironmentResource.h>

namespace JGW
{
    JGW_SetSuiteEnvironmentVarFunc CCJGW_TestSuiteEnvironmentResource::mpSetSuiteEnvFunc = NULL;
    std::wstring g_strTemp;
    JGW_GetSuiteEnvironmentVarFunc CCJGW_TestSuiteEnvironmentResource::mpGetSuiteEnvFunc = NULL;

    CCJGW_TestSuiteEnvironmentResource::CCJGW_TestSuiteEnvironmentResource(void)
    {
    }


    CCJGW_TestSuiteEnvironmentResource::~CCJGW_TestSuiteEnvironmentResource(void)
    {
    }

    void CCJGW_TestSuiteEnvironmentResource::SetSuiteEnvironmentFunc(JGW_SetSuiteEnvironmentVarFunc pset_suite_env_func,JGW_GetSuiteEnvironmentVarFunc pget_suite_env_func)
    {
        mpSetSuiteEnvFunc = pset_suite_env_func;
        mpGetSuiteEnvFunc = pget_suite_env_func;
    }
}
