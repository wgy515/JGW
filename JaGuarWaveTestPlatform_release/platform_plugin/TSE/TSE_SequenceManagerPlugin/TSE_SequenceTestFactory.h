#pragma once
#include <TSE_SequenceManagerPlugin/TSE_SequenceTestImpl.h>
#include <string>
#include "TSE_LogServices.h"
#include "TSE_TestStatusService.h"
#include "TSE_GlobalEnvironment.h"
namespace JGW
{
    class CTSE_SequenceTestFactory : public CTSE_SequenceTestImpl
    {
    public:
        CTSE_SequenceTestFactory(CTSE_LogServices& logServices,CTSE_TestStatusService& testStatusService,CTSE_GlobalEnvironment& globalEnvironment,int token_id);
        ~CTSE_SequenceTestFactory(void);
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
        CTSE_SequenceTestImpl* mpSequenceTestImpl;
        //! 多路测试令牌ID
        int mnTokenID;
        //! TSE 注册的类指针供 TSE DLL回调使用
        CTSE_LogServices& mLogServices;
        CTSE_TestStatusService& mTestStatusService;
        CTSE_GlobalEnvironment& mGlobalEnvironment;
    };
}

