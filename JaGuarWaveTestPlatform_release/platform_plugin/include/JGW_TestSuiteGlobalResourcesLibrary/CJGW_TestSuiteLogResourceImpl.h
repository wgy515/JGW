#pragma once

namespace JGW
{
    typedef void (_cdecl *jgw_test_suite_log_resource_func)(int nIndex,int type,const wchar_t* strmsg);
    class CCJGW_TestSuiteLogResourceImpl
    {
    public:
        virtual ~CCJGW_TestSuiteLogResourceImpl(){}

        virtual void RegisterSuiteLogCallback(const int nIndex,jgw_test_suite_log_resource_func& pSuiteLogResFunc) = 0;
    };
}