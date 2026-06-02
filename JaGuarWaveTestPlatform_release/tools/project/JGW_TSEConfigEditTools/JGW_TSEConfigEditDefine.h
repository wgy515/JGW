#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <map>

namespace JGW
{
    typedef enum 
    {
        E_PARAM_STRING = 0,
        E_PARAM_INT,
        E_PARAM_HEX
    }E_PARAM_TYPE;

    typedef struct 
    {
        E_PARAM_TYPE meParamType;
        std::wstring mParamName;
        std::wstring mParamValue;
        std::wstring mParamDescription;
    }s_tse_test_param;

    typedef struct 
    {
        bool mbIsTest; //! 是否测试
        bool mbReversedResult; //! 相反的结果 => 返回false 表示true
        bool mbIfFalse; //! 如果测试结果为false 则运行子测试序列
        bool mbFinalizeTest; //! 测试完成后销毁测试动作
        bool mbIsFolderTest; //! TSE2 FolderTest
        std::wstring mstrTestName;
        std::vector<s_tse_test_param> mvTestConfigs;
    } s_single_item_test_config;

    struct s_suite_test_config
    {
        // 当前测试项目
        s_single_item_test_config msItemTestConfig;
        // 子节点测试项目
        std::vector<std::shared_ptr<s_suite_test_config>> mvChildrenSuiteTest;
    };

    // environment key -  environment value
    typedef std::unordered_map<std::wstring,std::wstring> _mMapGlobalVariables;
    // suite item test config
    typedef std::vector<std::shared_ptr<s_suite_test_config>> _vsuitetestconfig;

    typedef struct  
    {
        //! 参数数组 默认输出参数组
        //! 正常测试
        _vsuitetestconfig m_vSuiteTestConfig;
        //! 自动化测试相关
        _vsuitetestconfig m_vAutoTestConfig;
        //! 测试视图相关 -> 重新构造测试suite文件
        _vsuitetestconfig m_vConfigViewTestConfig;
        //! 通用参数组
        _mMapGlobalVariables mmapGlobalVariables;
    } S_TSE_CONFIG_INFOS,*LPS_TSE_CONFIG_INFOS;
}
