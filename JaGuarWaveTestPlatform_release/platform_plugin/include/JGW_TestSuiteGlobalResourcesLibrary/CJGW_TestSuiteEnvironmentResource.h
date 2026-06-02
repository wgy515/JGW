#pragma once
#include <JGW_TestSuiteGlobalResourcesLibrary/CJGW_TestSuiteEnvironmentResourceImpl.h>
namespace JGW
{
    class CCJGW_TestSuiteEnvironmentResource : public CCJGW_TestSuiteEnvironmentResourceImpl
    {
    public:
        CCJGW_TestSuiteEnvironmentResource(void);
        ~CCJGW_TestSuiteEnvironmentResource(void);

    public:
        void SetSuiteEnvironmentFunc(JGW_SetSuiteEnvironmentVarFunc pset_suite_env_func,JGW_GetSuiteEnvironmentVarFunc pget_suite_env_func);

    private:
        static JGW_SetSuiteEnvironmentVarFunc mpSetSuiteEnvFunc;
        static JGW_GetSuiteEnvironmentVarFunc mpGetSuiteEnvFunc;
    };
}


