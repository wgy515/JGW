#pragma once
#include <string>
namespace JGW
{
    //! 设置序列环境变量函数指针
    typedef void (_cdecl *JGW_SetSuiteEnvironmentVarFunc)(const wchar_t* envirname,const wchar_t* envirvalue);
    //! 获取序列环境变量函数指针
    typedef std::wstring (_cdecl *JGW_GetSuiteEnvironmentVarFunc)(const wchar_t* envirname);

    class CCJGW_TestSuiteEnvironmentResourceImpl
    {
    public:
        virtual ~CCJGW_TestSuiteEnvironmentResourceImpl(){}

        virtual void SetSuiteEnvironmentFunc(JGW_SetSuiteEnvironmentVarFunc pset_suite_env_func,JGW_GetSuiteEnvironmentVarFunc pget_suite_env_func) = 0;
    };
}