#pragma once
#include <TSE_SequenceManagerPlugin/TSE_SequenceManageImpl.h>
#include "CJGW_LuaHelp.h"
#include "TSE_LuaGlobalEnvironmentDefault.h"

namespace JGW
{
	class CTSE_LuaSequenceManage : public CTSE_SequenceManageImpl
	{
	public:
		CTSE_LuaSequenceManage(unsigned long token_id);
		~CTSE_LuaSequenceManage(void);
	public:
		void SetGlobalEnvironmentImpl(CTSE_GlobalEnvironmentImpl* pGlobalEnvironmentImpl);
	private:
		//! Get Token ID
		unsigned long GetTokenID();
		//! 设置Test Status 监听
		void SetOnTestStatusListener(CTSE_TestStatusListener* pListener);
		//! 移除Test Status 监听
		void RemoveOnTestStatusListener(CTSE_TestStatusListener* pListener);
		//! 设置log 监听
		void SetOnLogListener(CTSE_LogListener* pListener);
		//! 移除log 监听
		void RemoveOnLogListener(CTSE_LogListener* pListener);
		//! 获取环境变量接口
		CTSE_GlobalEnvironmentImpl* GetGlobalEnvironmentImpl();
		//! 获取测试类接口
		CTSE_SequenceTestImpl* GetSequenceTestImpl();
	private:
		unsigned long mTokenID;
		
		bool mbIsDefaultGlobalEnvironment;
		CTSE_GlobalEnvironmentImpl* mpGlobalEnvironment;
		CTSE_SequenceTestImpl* mpSequenceTest;
		CTSE_TestStatusListener* mpTestStatusListener;
		CTSE_LogListener* mpLogListener;
		CCJGW_LuaHelp mcLuaHelp;
		CTSE_LuaGlobalEnvironmentDefault mcLuaGlobalEnvironmentDefault;
	};

}

