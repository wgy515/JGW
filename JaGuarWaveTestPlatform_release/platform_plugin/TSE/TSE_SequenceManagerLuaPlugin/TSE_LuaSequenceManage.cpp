#include "StdAfx.h"
#include "TSE_LuaSequenceManage.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#include "TSE_LuaSequenceTest.h"
namespace JGW
{
	CTSE_LuaSequenceManage::CTSE_LuaSequenceManage(unsigned long token_id) : mTokenID(token_id),mpGlobalEnvironment(NULL),mbIsDefaultGlobalEnvironment(false),mpSequenceTest(NULL)
	{
	}


	CTSE_LuaSequenceManage::~CTSE_LuaSequenceManage(void)
	{
#if 0
		if (mbIsDefaultGlobalEnvironment && NULL != mpGlobalEnvironment)
		{
			delete mpGlobalEnvironment;
			mpGlobalEnvironment = NULL;
		}
#endif
		if (NULL == mpSequenceTest)
		{
			delete mpSequenceTest;
		}
	}

	unsigned long CTSE_LuaSequenceManage::GetTokenID()
	{
		return mTokenID;
	}

	void CTSE_LuaSequenceManage::SetOnTestStatusListener(CTSE_TestStatusListener* pListener)
	{
		mpTestStatusListener = pListener;
		//if (NULL != mcLuaHelp.GetLuaState())
		//{
		//	std::string strTestStatusServicePoint = JGW_GetFormatString("TestStatusServicePoint%d",mTokenID);
		//	mcLuaHelp.SetLuaGlobalVariableInteger(strTestStatusServicePoint,(int)pListener);
		//}
	}

	void CTSE_LuaSequenceManage::RemoveOnTestStatusListener(CTSE_TestStatusListener* pListener)
	{

	}

	void CTSE_LuaSequenceManage::SetOnLogListener(CTSE_LogListener* pListener)
	{
		mpLogListener = pListener;
		//if (NULL != mcLuaHelp.GetLuaState())
		//{
		//	std::string strLogServicesPoint = JGW::JGW_GetFormatString("LogServicesPoint%d",mTokenID);
		//	mcLuaHelp.SetLuaGlobalVariableInteger(strLogServicesPoint,(int)pListener);
		//}
	}

	void CTSE_LuaSequenceManage::RemoveOnLogListener(CTSE_LogListener* pListener)
	{

	}

	void CTSE_LuaSequenceManage::SetGlobalEnvironmentImpl(CTSE_GlobalEnvironmentImpl* pGlobalEnvironmentImpl)
	{
		mpGlobalEnvironment = pGlobalEnvironmentImpl;
		if (NULL != pGlobalEnvironmentImpl)
		{
			for (std::map<std::wstring,std::wstring>::iterator it = mcLuaGlobalEnvironmentDefault.mmapKeyValue.begin();it != mcLuaGlobalEnvironmentDefault.mmapKeyValue.end();++it)
			{
				mpGlobalEnvironment->PutString(it->first,it->second);
			}
		}
	}

	CTSE_GlobalEnvironmentImpl* CTSE_LuaSequenceManage::GetGlobalEnvironmentImpl()
	{
#if 0
		if (mbIsDefaultGlobalEnvironment && NULL != mpGlobalEnvironment)
		{
			std::string strGlobalEnvironmentPoint = JGW::JGW_GetFormatString("GlobalEnvironmentPoint%d",mTokenID);
			CTSE_GlobalEnvironmentImpl* pTempGlobalEnvironmentImpl = (CTSE_GlobalEnvironmentImpl*)mcLuaHelp.GetLuaGlobalVariableInteger(strGlobalEnvironmentPoint,NULL);
			if (NULL != pTempGlobalEnvironmentImpl)
			{
				mpGlobalEnvironment = pTempGlobalEnvironmentImpl;
			}
		}

		if (NULL == mpGlobalEnvironment)
		{
			std::string strGlobalEnvironmentPoint = JGW::JGW_GetFormatString("GlobalEnvironmentPoint%d",mTokenID);
			mpGlobalEnvironment = (CTSE_GlobalEnvironmentImpl*)mcLuaHelp.GetLuaGlobalVariableInteger(strGlobalEnvironmentPoint,NULL);
			if (NULL == mpGlobalEnvironment)
			{
				mbIsDefaultGlobalEnvironment = true;
				mpGlobalEnvironment = new CTSE_LuaGlobalEnvironmentDefault();
			}
		}
		return mpGlobalEnvironment;
#else
		if (NULL == mpGlobalEnvironment)
		{
			return &mcLuaGlobalEnvironmentDefault;
		}
#if 0
		if (NULL != mpGlobalEnvironment)
		{
			return mpGlobalEnvironment;
		}

		if (NULL != mpSequenceTest)
		{
			mpGlobalEnvironment = ((CTSE_LuaSequenceTest*)mpSequenceTest)->GetGlobalEnvironmentImpl();
		}
#endif

		return mpGlobalEnvironment;
#endif
	}

	CTSE_SequenceTestImpl* CTSE_LuaSequenceManage::GetSequenceTestImpl()
	{
		if (NULL == mpSequenceTest)
		{
			mpSequenceTest = new CTSE_LuaSequenceTest(mTokenID,mpLogListener,mpTestStatusListener,this,mcLuaHelp);
		}
		return mpSequenceTest;
	}

}

