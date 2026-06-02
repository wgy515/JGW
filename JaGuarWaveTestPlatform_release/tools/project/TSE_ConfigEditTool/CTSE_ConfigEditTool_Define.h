#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <tinyxml/tinyxml.h>

namespace JGW
{
/*    typedef std::unordered_map<std::wstring,std::wstring> _mcommontestconfig;*/

    enum E_PARAM_TYPE
    {
        E_PARAM_STRING = 0,
        E_PARAM_INT
    };
    //! TSE SUITE节点 单项配置
    struct s_suite_single_item_test_config
    {
        E_PARAM_TYPE meParamType;
        std::wstring mParamName;
        std::wstring mParamValue;
        std::wstring mParamDescription;
    };
    //! TSE 单个suite配置
    class s_suite_item_test_config
    {
    public:
        s_suite_item_test_config():m_bFinalizeTest(false){}
        bool mbIsTest; //! 是否测试
        bool m_bFinalizeTest; //! 测试完成后销毁测试动作
        TiXmlElement*	mpSuiteXmlNode;
        std::string mstrPriority; //! 优先级 1:先于当前测试项目 0：不属于先于当前测试项目
        std::string mstrGroupName; //! 测试组名称
        std::vector<s_suite_single_item_test_config> m_mTestParam;
    public:
        std::vector<s_suite_single_item_test_config>::iterator find(const wchar_t* strKey)
        {
            for (auto it = m_mTestParam.begin();
                it != m_mTestParam.end();
                it ++)
            {
                if (it[0].mParamName == strKey)
                {
                    return it;
                }
            }
            return m_mTestParam.end();
        }
    };

    typedef std::vector<std::shared_ptr<s_suite_item_test_config>> _vsuitetestconfig;
    //! typedef std::unordered_map<std::wstring,std::wstring> _mcommontestconfig;

    typedef struct
    {
        TiXmlElement*	mpCommonXmlNode;
        std::unordered_map<std::wstring,std::wstring> mEnvironment;
    }_mcommontestconfig;
    //! DLL 默认目录为当前EXE目录
    class S_TSE_SequenceConfig
    {
    public:
        //! 参数数组 默认输出参数组
        //! 正常测试
        _vsuitetestconfig m_vSuiteTestConfig;
        //! 自动化测试相关
        _vsuitetestconfig m_vAutoTestConfig;
        //! 测试视图相关 -> 重新构造测试suite文件
        _vsuitetestconfig m_vConfigViewTestConfig;
        //! 通用参数组
        _mcommontestconfig m_mCommonTestParam;
    };

    enum E_TREENODEUI_TYPE
    {
        E_PROJECT_TREE_TYPE = 0, //! 项目TREE
        E_ENVIRONMENT_TREE_TYPE = 1,//! 变量类型
        E_CONFIG_SUITE_TREE_TYPE,//! config view
        E_AUTO_SUITE_TREE_TYPE,//! auto view
        E_SUITE_TEST_TREE_TYPE //! suite view
    };

    typedef struct  
    {
        E_TREENODEUI_TYPE meTreeNodeUIType;
        union
        {
            s_suite_item_test_config* mpsSuiteItemConfig;
            _mcommontestconfig* mpEnvironmentConfig;
        };
    }S_TSE_TREE_NODE_DATA,*LPS_TSE_TREE_NODE_DATA;
}