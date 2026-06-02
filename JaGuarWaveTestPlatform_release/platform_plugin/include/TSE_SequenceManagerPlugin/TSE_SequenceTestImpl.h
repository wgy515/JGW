#pragma once
#include <string>
namespace JGW
{
    class CTSE_SequenceTestImpl
    {
    public:
        //! TSE 测试配置文件
        virtual bool LoadTSESuiteConfig(const std::string& strResourceConfig) = 0;
        //! 执行TSE 测试 bErrorStop : 错误停止
        virtual bool ExecTSEAllSuiteTest(bool bErrorStop = true) = 0;
        //! 是否终止TSE测试
        //virtual bool IsStopTSETest(bool stop = true);
		//! 执行TSE配置视图
		virtual bool ExecTSEConfigTest() = 0;
		//! 执行TSE自动化测试指令
		virtual bool ExecTSEAutoTest() = 0;
        //! 执行TSE 测试 单个Sequence测试
        virtual bool ExecTSETestToIndex(size_t index) = 0;
        //! 执行TSE 测试项目组
        virtual bool ExecTSEGroupTestToIndex(size_t index) = 0;
        //! 执行测试销毁动作
        virtual bool ExecTSETestFinalize(void) = 0;
        //! 执行TSE 初始化测试
        virtual bool InitTSETestConfig(void) = 0;
        //! TSE自动化序列是否为空  true: 存在 false:不存在
        virtual bool TSEAutoTestNotEmpty() = 0;
        //! 重置已运行测试索引数组
        virtual void ClearTSEExistedTestArrayIndex() = 0;
        //! 获取TSE 测试名称
        virtual const wchar_t* GetTSETestNameToIndex(size_t nIndex) = 0;
        //! 卸载TSE
        virtual void UnLoadTSETestPlugin() = 0;
    };
}