#pragma once
namespace JGW
{
    extern "C"
    {
        __declspec(dllexport) void RegisterSuiteLogCallback(const int nIndex,jgw_test_suite_log_resource_func& pSuiteLogResFunc);
        __declspec(dllexport) void SetSuiteEnvironmentFunc(JGW_SetSuiteEnvironmentVarFunc pset_suite_env_func,JGW_GetSuiteEnvironmentVarFunc pget_suite_env_func);
    }
}