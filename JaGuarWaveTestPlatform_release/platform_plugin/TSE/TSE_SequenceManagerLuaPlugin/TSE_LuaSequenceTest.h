#pragma once
#include <TSE_SequenceManagerPlugin/TSE_SequenceManageImpl.h>
#include "CJGW_LuaHelp.h"
namespace JGW
{
	class CTSE_LuaSequenceTest : public CTSE_SequenceTestImpl
	{
	public:
		CTSE_LuaSequenceTest(unsigned long ulTokenID,CTSE_LogListener* pLogListener,CTSE_TestStatusListener* pTestStatusListener,CTSE_SequenceManageImpl* pSequenceManageImpl,CCJGW_LuaHelp& cLuaHelp);
		~CTSE_LuaSequenceTest(void);
	public:
		CTSE_GlobalEnvironmentImpl* GetGlobalEnvironmentImpl();
	private: //! 调用顺序 LoadTSESuiteConfig => InitTSETestConfig =>
		//! TSE 测试配置文件
		bool LoadTSESuiteConfig(const std::string& strResourceConfig);
		//! 执行TSE 初始化测试
		bool InitTSETestConfig(void);
		//! TSE自动化序列是否为空  true: 存在 false:不存在
		bool TSEAutoTestNotEmpty();
		//! 执行TSE自动化测试指令
		bool ExecTSEAutoTest();
		//! 执行TSE 测试 bErrorStop : 错误停止
		bool ExecTSEAllSuiteTest(bool bErrorStop = true);
		//! 是否终止TSE测试
		//virtual bool IsStopTSETest(bool stop = true);
		//! 执行TSE配置视图
		bool ExecTSEConfigTest();

		//! 执行TSE 测试 单个Sequence测试
		bool ExecTSETestToIndex(size_t index);
		//! 执行TSE 测试项目组
		bool ExecTSEGroupTestToIndex(size_t index);
		//! 执行测试销毁动作
		bool ExecTSETestFinalize(void);
		
		
		//! 重置已运行测试索引数组
		void ClearTSEExistedTestArrayIndex();
		//! 获取TSE 测试名称
		const wchar_t* GetTSETestNameToIndex(size_t nIndex);
		//! 卸载TSE
		void UnLoadTSETestPlugin();
	private:
		CTSE_TestStatusListener* mpTestStatusListener;
		CTSE_LogListener* mpLogListener;
		CTSE_GlobalEnvironmentImpl* mpGlobalEnvironmentImpl;
		CTSE_SequenceManageImpl* mpSequenceManageImpl;
		unsigned long mulTokenID;
		CCJGW_LuaHelp& mcLuaHelp;
	};
}
