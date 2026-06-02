#pragma once
#include <TSE_SequenceManagerPlugin/TSE_SequenceManageImpl.h>
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
#include <TSE_SequenceManagerPlugin/TSE_SequencePocoClassLoader.hpp>
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "TSE_LogServices.h"
#include "TSE_TestStatusService.h"
#include "TSE_GlobalEnvironment.h"
//#include "TSE_SequenceTest.h"
//#include <TSE_SequenceManagerPlugin/TSE_Define.h>
//#define TDO_TSE_GROUP_CODE

namespace JGW
{
    class CTSE_SequenceManager
    {
    public:
        CTSE_SequenceManager(int nIndex);
        ~CTSE_SequenceManager(void);
    public:
        CTSE_TestBase* CreateSequence(const std::wstring& strModuleName,const std::string& strRealName,const std::wstring& strTestName);
        const std::wstring& GetErrorMessage();
	public:
		//! 
		void AddLogListener(CTSE_LogListener* pLogListener);
		//! 
		void AddTestStatusListener(CTSE_TestStatusListener* mpTestStatusListener);
	public:
		CTSE_GlobalEnvironmentImpl* GetGlobalEnvironmentImpl();
		CTSE_TestStatusServiceImpl* GetTestStatusServiceImpl();
	//public:
	//	void SetGlobalEnvironmentImpl(CTSE_GlobalEnvironmentImpl* pGlobalEnvironmentImpl);
	//	void SetLogServicesImpl(CTSE_LogServicesImpl* pLogServicesImpl);
	//	void SetTestStatusServiceImpl(CTSE_TestStatusServiceImpl* pTestStatusServiceImpl);
	//	CTSE_GlobalEnvironmentImpl* GetGlobalEnvironmentImpl();
	//	CTSE_LogServicesImpl* GetLogServicesImpl();
	//	CTSE_TestStatusServiceImpl* GetTestStatusServiceImpl();
	private:
		//! 加载TSE测试模块
		bool LoadTSETestModule(const std::wstring& strTestSuiteModuleName,std::wstring& strTestSuiteModuleFilePath);
		//! 设置TSE全局模块资源信息
		bool SetTSEModuleGlobalResourcesInfo(const std::wstring& strTSEModuleFilePath);
	private:
		int mnIndex;
		std::wstring mstrErrorMsg;
		CTSE_LogServices mcLogServicesImpl ;
		CTSE_TestStatusService mcTestStatusServiceImpl;
		CTSE_GlobalEnvironment mcGlobalEnvironmentImpl;
	private:
		//!
		std::vector<CTSE_TestBase*> mvTSETestBase;
		//! 配置视图的测试命令 TSE_ConfigUIPlugin.dll
		//std::vector<CTSE_TestBase*> m_vConfigViewTestCommand;
		//! 共享的全局资源组
		std::vector<std::shared_ptr<CTSE_TestBase>> mvSuiteGlobalResources;
		//! TSE DLL加载实例
		CTSE_SequencePocoClassLoader<CTSE_TestBase> mTestSuitePocoClassLoader;
    };
}

