#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace JGW
{
    typedef std::unordered_map<std::wstring,std::wstring> _mcommontestconfig;

    struct s_suite_single_item_test_config
    {
        std::wstring mParamName;
        std::wstring mParamValue;
        std::wstring mParamDescription;
    };

    class s_suite_item_test_config
    {
    public:
        s_suite_item_test_config():m_bFinalizeTest(false){}
        std::vector<s_suite_single_item_test_config> m_mTestParam;
        bool m_bFinalizeTest;
    public:
        std::vector<s_suite_single_item_test_config>::iterator find(const wchar_t* strKey)
        {
            for (auto it = m_mTestParam.begin();
                it != m_mTestParam.end();
                it ++)
            {
                if ( it[0].mParamName == strKey )
                {
                    return it;
                }
            }
            return m_mTestParam.end();
        }
    };

    typedef std::vector<std::shared_ptr<s_suite_item_test_config>> _vsuitetestconfig;
    typedef std::unordered_map<std::wstring,std::wstring> _mcommontestconfig;

    //! DLL 默认目录为当前EXE目录
    class s_suite_test_config
    {
    public:
        //! 参数数组 默认输出参数组
        _vsuitetestconfig m_vSuiteTestConfig;
        //! 通用参数组
        _mcommontestconfig m_mCommonTestParam;
    };
}