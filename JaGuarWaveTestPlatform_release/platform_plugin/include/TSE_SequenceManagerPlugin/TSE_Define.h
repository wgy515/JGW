#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <map>

namespace JGW
{
    typedef std::unordered_map<std::wstring,std::wstring> _mcommontestconfig;
    //! TSE SUITE节点 单项配置
    struct s_suite_single_item_test_config
    {
        std::wstring mParamName;
        std::wstring mParamValue;
        std::wstring mParamDescription;
    };
    //! TSE 单个suite配置
    class s_suite_item_test_config
    {
    public:
        s_suite_item_test_config():m_bFinalizeTest(false),mbReversedResult(false){}
        bool mbIsTest; //! 是否测试
        bool mbReversedResult; //! 相反的结果 => 返回false 表示true
        bool mbIfFalse; //! 如果测试结果为false 则运行子测试序列
        bool m_bFinalizeTest; //! 测试完成后销毁测试动作
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
    typedef std::unordered_map<std::wstring,std::wstring> _mcommontestconfig;

	struct S_Test_SuiteProjectConfig
	{
		s_suite_item_test_config msSuiteItemTestConfig;
		std::vector<std::shared_ptr<S_Test_SuiteProjectConfig>> m_vChildTestSuiteProjectConfig;
	};
	typedef std::vector<std::shared_ptr<S_Test_SuiteProjectConfig>> _vs_suite_projectconfig;
    //! DLL 默认目录为当前EXE目录
    class S_TSE_SequenceConfig
    {
    public:
        //! 参数数组 默认输出参数组
        //! 正常测试
        _vs_suite_projectconfig mvsSuiteProjectConfig;
        //! 自动化测试相关
        _vsuitetestconfig m_vAutoTestConfig;
        //! 测试视图相关 -> 重新构造测试suite文件
        _vsuitetestconfig m_vConfigViewTestConfig;
        //! 通用参数组
        _mcommontestconfig m_mCommonTestParam;
    };

    struct S_TSE_TEST_GROUP
    {
        bool m_bFinalizeTest; //! 是否是销毁动作执行档
        int mnPriority; //! 1:先于当前测试项目 0：不属于先于当前测试项目
        size_t mnTestIndex; //! 当前测试索引
    };

    class CTSE_TestBase;
    typedef struct 
    {
        CTSE_TestBase* mpTSETestBase;
        std::vector<std::vector<S_TSE_TEST_GROUP>*> mvpsTSETestIndexGroups;
    }S_TSE_SUITE_GROUPS,*PS_TSE_TEST_INDEX_GROUPS;

    typedef struct  
    {
        //! 正常测试
        std::vector<CTSE_TestBase*> m_vSuiteTestCommand;
        //! 自动化测试相关 预测试命令
        std::vector<CTSE_TestBase*> m_vAutoTSETestCommand;
        //! 配置视图的测试命令
        std::vector<CTSE_TestBase*> m_vConfigViewTestCommand;
        //! TSE 测试组
        std::map<std::string,std::vector<S_TSE_TEST_GROUP>> mmapTSETestGroup;
        //! TSE ID -> 匹配测试组信息
        std::vector<S_TSE_SUITE_GROUPS> mvsTSETestIndexGroups;
        //! 共享的全局资源组
        std::vector<std::shared_ptr<CTSE_TestBase>> mvSuiteGlobalResources;
    }S_TSE_PARAM,*PS_TSE_PARAM;
}