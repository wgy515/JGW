#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <map>
#include <vector>

namespace JGW
{
    struct S_TSE2_TEST;

    struct S_FOLDER_TEST  
    {
        //! 循环参数数量
        size_t mnParamCount;
        //! 参数名称
        std::wstring mstrParamHeader;
        //! 模块名称
        std::wstring mstrModuleName;
        //! 实例名称
        std::wstring mstrRealName;
        //! 测试名称
        std::wstring mstrTestName;
        //! 参数值
        std::vector<std::wstring> mvstrParamValues;
        //! 子测试项目
        std::vector<S_TSE2_TEST> mvTseTest;
    };

    struct S_TEST_BASE 
    {
        //! 失败重测次数
        size_t mnErrorRetryCount;
        //! 最后执行的测试项目
        bool mbIsFinalize;
        //! 取反测试结果
        bool mbReversedResult;
        //! 当前测试失败后是否继续执行子测试项目
        //! 1 : 如果测试结果为false 则运行子测试序列
        bool mbIfFalse;
        //! 模块名称
        std::wstring mstrModuleName;
        //! 实例名称
        std::wstring mstrRealName;
        //! 测试名称
        std::wstring mstrTestName;
        //! 参数名称以及参数值
        std::map<std::wstring,std::wstring> mmapParam;
        //! 
        std::shared_ptr<CTSE_TestBase> mptrTestBase;
        //! 子测试项目
        std::vector<S_TSE2_TEST> mvTseTest;
    };

    struct S_TSE2_TEST
    {
        //! 是否是测试目录
        bool mbIsFolderTest;
        //! 测试类
        S_TEST_BASE msTestBase;
        //! 测试目录测试项目
        S_FOLDER_TEST msFolderTest;
    };

    typedef struct  
    {
        std::vector<S_TSE2_TEST> msTse2Test;
    }S_ROOT_TEST_STRUCT;

    typedef struct
    {
        S_ROOT_TEST_STRUCT msUiConfigTestStruct;
        S_ROOT_TEST_STRUCT msAutoTestStruct;
        S_ROOT_TEST_STRUCT msRootTestStruct;
        //! GlobalVariableKey,GlobalVariableValue
        std::map<std::wstring,std::wstring> mmapGlobalVariables;
    }S_TSE2_STRUCT;
}