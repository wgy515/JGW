#pragma once
#include "CJGW_TestSuiteManage_Define.h"
#include <JGW_TestSuiteManagePlugin/CJGW_TestSuiteManageImpl.h>
#include <JGW_TestSuiteManagePlugin/CJGW_TestSuitePocoClassLoader.hpp>
#include <JGW_TestSuiteManagePlugin/CJGW_TestSuiteImpl.h>
namespace JGW
{
    class CCJGW_TestSuiteInterface
    {
    public:
        CCJGW_TestSuiteInterface(bool bMultiSuite,const int nIndex);
        ~CCJGW_TestSuiteInterface(void);
        //!
        void    SetSuiteTestLogFuncPtr(jgw_suite_test_log_func pSuiteTestLogFunc);
        //! 初始化序列接口
        bool	InitSuiteAssembly(HWND hMain,const std::string& strConfigFileBuf,LPCTSTR strConfigFilePath = NULL);
        //!
        size_t  GetSuiteClassPtrCount();

        //! 执行所有测试项目
        bool	ExecuteAllTest(bool bErrorStop = true);
        //! 执行测试序列清理动作
        bool	ExecuteTestFinalize();
        //! 执行单个测试 根据测试ID
        bool	ExecuteSingleTest(size_t nIndex);

        void    UpdateEnviromentParam();
    private:
        //! 加载DLL模块
        bool    LoadTestSuiteModule(std::wstring& strTestSuiteModuleName,std::string& strTestSuiteModuleFilePath);
        //!
        bool    SetTestSuiteGlobalResourcesInfo(const std::string& strTestSuiteModuleFilePath);
        //! 加载suite class 
        bool    LoadTestSuiteModuleClassPtr();
        //! Init Config Param
        void    InitTestSuiteClassConfigParam(std::shared_ptr<CCJGW_TestSuiteImpl> ptrTestSuiteImpl,std::shared_ptr<s_suite_item_test_config> ptrSuiteItemTestConfig);
    private:
        bool    mbMultiSuite;
        HWND	m_hMain;
        int		m_nAssemblyIndex;
        jgw_suite_test_log_func mpSuiteTestLogFunc;
        s_suite_test_config	m_s_suite_config;
        std::vector<size_t> m_vAlreadyRunSuiteIndex;
        std::vector<size_t>	m_vFinalizeTest;
        std::vector<std::shared_ptr<CCJGW_TestSuiteImpl>> m_vSuiteTestCommand;
        std::vector<std::wstring> mvSuiteTestName;
        CCJGW_TestSuitePocoClassLoader<CCJGW_TestSuiteImpl> mTestSuitePocoClassLoader;
    };
}


