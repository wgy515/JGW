#pragma once
#include "TSE2_SequenceManagerPluginDefine.h"
#include <TSE_SequenceManagerPlugin/TSE_SequenceManageImpl.h>
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
#include <TSE_SequenceManagerPlugin/TSE_SequencePocoClassLoader.hpp>
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "..\TSE_LogServices.h"
#include "..\TSE_TestStatusService.h"
#include "..\TSE_GlobalEnvironment.h"
#include "..\TSE_SequenceTest.h"
#include <TSE_SequenceManagerPlugin/TSE_Define.h>

namespace JGW
{
    class CTSE2_SequenceTest : public CTSE_SequenceTestImpl
    {
    public:
        CTSE2_SequenceTest(CTSE_LogServices& logServices,CTSE_TestStatusService& testStatusService,CTSE_GlobalEnvironment& globalEnvironment,int token_id);
        ~CTSE2_SequenceTest(void);
    public:
        //! TSE 测试配置文件
        bool LoadTSESuiteConfig(const std::string& strResourceConfig);
        //! 执行TSE 测试 bErrorStop : 错误停止
        bool ExecTSEAllSuiteTest(bool bErrorStop = true);
        //! 是否终止TSE测试
        //virtual bool IsStopTSETest(bool stop = true);
        //! 执行TSE配置视图
        bool ExecTSEConfigTest();
        //! 执行TSE自动化测试指令
        bool ExecTSEAutoTest();
        //! 执行TSE 测试 单个Sequence测试
        bool ExecTSETestToIndex(size_t index);
        //! 执行TSE 测试项目组
        bool ExecTSEGroupTestToIndex(size_t index);
        //! 执行测试销毁动作
        bool ExecTSETestFinalize(void);
        //! 执行TSE 初始化测试
        bool InitTSETestConfig(void);
        //! TSE自动化序列是否为空  true: 存在 false:不存在
        bool TSEAutoTestNotEmpty();
        //! 重置已运行测试索引数组
        void ClearTSEExistedTestArrayIndex();
        //! 获取TSE 测试名称
        const wchar_t* GetTSETestNameToIndex(size_t nIndex);
        //! 卸载TSE
        void UnLoadTSETestPlugin();
    private:
        //! 加载模块对应的实例类
        bool LoadTestSuiteModuleClassPtr(std::vector<S_TSE2_TEST>& sTseTest,bool bIsRootTestNode = false);
        //!
        bool LoadTSEPlugin(const std::wstring& strModuleName);

        //
        bool SetTSEModuleGlobalResourcesInfo();
        //! 
        bool LoadTSEModule(const std::wstring& strModuleName);

        bool ExecuteTSE2Test(S_TSE2_TEST& sTse2Test);
        //! 
        CTSE_TestBase* LoadSingeTSEPtrClassPlugin(const std::string& strModuleName,S_TEST_BASE& sTestBase);
        //! bool LoadTSETestModule(std::wstring& strTestSuiteModuleName,std::wstring& strTestSuiteModuleFilePath);
        bool ExecuteSigleTestBase(S_TEST_BASE& sTestBase);
    private:
        //! 清理当前测试项目资源
        bool mbIsFinalizeTest;
        //! 初始化TSE测试项目
        bool mbInitTSETest;
        //! 多路测试令牌ID
        int mnTokenID;
        //! 当前测试索引
        size_t mnCurrentTestIndex;
        //! 上次测试索引s
        size_t mnPrevTestIndex;
        //! TSE 注册的类指针供 TSE DLL回调使用
        CTSE_LogServices& mLogServices;
        CTSE_TestStatusService& mTestStatusService;
        CTSE_GlobalEnvironment& mGlobalEnvironment;
        //! 上次加载模块路径
        std::string mstrPrevModulePath;
        //! TSE2 测试结构体
        S_TSE2_STRUCT msTse2Struct;
        //! TSE DLL加载实例
        CTSE_SequencePocoClassLoader<CTSE_TestBase> mTestSuitePocoClassLoader;
        //! 共享的全局资源组
        std::vector<std::shared_ptr<CTSE_TestBase>> mvSuiteGlobalResources;
        //! 加载TSE模块路径数组
        std::vector<std::string> mvstrTSEModulePath;
        //! map loop param
        std::map<std::wstring,std::wstring> mmapLoopParam;
        //! 
        std::wstring mstrLoopParamShow;
        //! 执行清理当前测试项目资源集合
        std::vector<S_TSE2_TEST*> m_vFinalizeTest;
    };

}

