#pragma once
#include <TSE_SequenceManagerPlugin/TSE_SequenceManageImpl.h>
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
#include <TSE_SequenceManagerPlugin/TSE_SequencePocoClassLoader.hpp>
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "TSE_LogServices.h"
#include "TSE_TestStatusService.h"
#include "TSE_GlobalEnvironment.h"
#include "TSE_SequenceTest.h"
#include <TSE_SequenceManagerPlugin/TSE_Define.h>
#define TDO_TSE_GROUP_CODE

namespace JGW
{
	//! CTSE_TestBase
	struct S_SuiteProject_TestBase
	{
        //! 如果测试结果为false 则运行子测试序列
        bool mbIfFalse;
        //! 测试结果取反
        bool mbReversedResult;
		size_t mnTestIndex;
		CTSE_TestBase* mpsTestBase;
		std::vector<std::shared_ptr<S_SuiteProject_TestBase>> mvChildTestBase;
	};

    class CTSE_SequenceTest : public CTSE_SequenceTestImpl
    {
    public:
        CTSE_SequenceTest(CTSE_LogServices& logServices,CTSE_TestStatusService& testStatusService,CTSE_GlobalEnvironment& globalEnvironment,int token_id);
        ~CTSE_SequenceTest(void);
    private:
        //! 
        bool LoadTSESuiteConfig(const std::string& strResourceConfig);
        //! 
        bool ExecTSEAllSuiteTest(bool bErrorStop = true);
		//!
		bool ExecTSEConfigTest();
		//! 
		bool ExecTSEAutoTest();
        //!
        bool TSEAutoTestNotEmpty();
        //! 
        bool ExecTSETestToIndex(size_t index);
        //! 
        bool ExecTSEGroupTestToIndex(size_t index);
        //!
        bool ExecTSETestFinalize();
        //! 
        bool InitTSETestConfig();
        //!
        void ClearTSEExistedTestArrayIndex();
        //!
        const wchar_t* GetTSETestNameToIndex(size_t nIndex);
        //!
        void UnLoadTSETestPlugin();
    private:
        bool LoadTestSuiteModuleClassPtr();
		//!
		bool LoadSingleTSEPlugin(s_suite_item_test_config& suiteItemTestConfig);
		//! 加载TSE模块所需的DLL
		bool LoadTSEPlugin(_vsuitetestconfig& vsuiteTestConfig,bool bSuiteProject = false);
		//! 
		bool LoadSuiteProjectTSEPlugin(_vs_suite_projectconfig& vsSuiteProjectConfig);
		//! 加载所需要插件测试序列实例指针
		bool LoadTSEPtrClassPlugin(_vsuitetestconfig& vsuiteTestConfig,std::vector<CTSE_TestBase*>& vTSETestBase,bool bSuiteProject = false);
		//! 
		CTSE_TestBase* LoadSingeTSEPtrClassPlugin(s_suite_item_test_config& sSuiteItemTestConfig,size_t index);
		//!
		bool LoadSuiteProjectTSEPtrClassPlugin(_vs_suite_projectconfig &vsSuiteProjectConfig,std::vector<std::shared_ptr<S_SuiteProject_TestBase>>& vsSuiteProjectTestBase,size_t& testIndex);
        //! 
        void LoadTSESuiteTestCommandGroupInfo();
        //! 加载TSE 测试DLL模块
        bool LoadTSETestModule(std::wstring& strTestSuiteModuleName,std::wstring& strTestSuiteModuleFilePath);
        //! 设置TSE全局模块资源信息
        bool SetTSEModuleGlobalResourcesInfo(std::wstring strTSEModuleFilePath);
        //! Init Config Param
        void InitTestSuiteClassConfigParam(CTSE_TestBase* ptrTestSuiteImpl,std::shared_ptr<s_suite_item_test_config> ptrSuiteItemTestConfig);
		void InitTestSuiteClassConfigParam(CTSE_TestBase* ptrTestSuiteImpl,s_suite_item_test_config& sSuiteItemTestConfig);
        //! 执行TSE测试组To索引
      //  bool ExecTSEGroupToIndex(size_t index,std::vector<S_TSE_TEST_GROUP>* pvsTSEGroup);
		//! S_SuiteProject_TestBase
		bool ExecuteSuiteProjectTestBase(std::shared_ptr<S_SuiteProject_TestBase> ptrSuiteProjectTestBase);
		//!
		bool ExecuteSingleSuiteProject(size_t testIndex,std::shared_ptr<S_SuiteProject_TestBase> ptrSuiteProjectTestBase);
		//!
		CTSE_TestBase* GetTestBaseToIndex(size_t index);
        //!
        std::shared_ptr<S_SuiteProject_TestBase> GetSuiteProjectTestBaseToIndex(size_t nIndex,std::vector<std::shared_ptr<S_SuiteProject_TestBase>>& vsSuiteProjectTestBase);
    private:
        //! 多路测试令牌ID
        int mnTokenID;
        //! 测试失败后最终测试项目Finalize
        bool mbIsFinalizeTest;
        //! TSE 注册的类指针供 TSE DLL回调使用
        CTSE_LogServices& mLogServices;
        CTSE_TestStatusService& mTestStatusService;
        CTSE_GlobalEnvironment& mGlobalEnvironment;
        //! TSE SUITE 配置
        S_TSE_SequenceConfig msSTESequenceConfig;
        //! 已运行测试序列
        std::vector<size_t> m_vAlreadyRunSuiteIndex;
        //! 当前TSE序列运行后执行的销毁动作
        std::vector<size_t>	m_vFinalizeTest;
		//! 正常测试
		std::vector<std::shared_ptr<S_SuiteProject_TestBase>> mvSuiteProjectTestBase;
		//!
		std::vector<CTSE_TestBase*> mvTSETestBase;
        //std::vector<CTSE_TestBase*> m_vSuiteTestCommand;
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
        //! TSE DLL加载实例
        CTSE_SequencePocoClassLoader<CTSE_TestBase> mTestSuitePocoClassLoader;
    };
}