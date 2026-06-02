// TSE_SequenceLuaHelpPlugin.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <TSE_SequenceLuaHelpPlugin/TSE_SequenceLuaHelpPlugin.h>
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <string>
#include <iostream>
#include "TSE_SequenceManager.h"
#include <memory>
#include <vector>
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>

using namespace JGW;
std::vector<std::unique_ptr<CTSE_SequenceManager>> gvptrSequenceManager;
CCJGW_CriticalSectionLock gCriticalSectionLock;

static int create_tse(lua_State* pLuaState);
static int tse_addparam(lua_State* pLuaState);
static int tse_init(lua_State* pLuaState);
static int tse_run(lua_State* pLuaState);
static int tse_exit(lua_State* pLuaState);
static int tes_getretrycount(lua_State* pLuaState);
static int pre_initialization(lua_State* pLuaState);
static int get_enviroment(lua_State* pLuaState);
static int set_enviroment(lua_State* pLuaState);
static int change_test_status(lua_State* pLuaState);


static const struct luaL_Reg gvTSELuaArrayLib[] = {
#if 1
	//! Pre-initialization
	{"pre_initialization", pre_initialization},
	{"new", create_tse},
	{"add_param", tse_addparam},
	{"init", tse_init},
	{"run", tse_run},
	{"exit", tse_exit},
	{"get_env", get_enviroment},
	{"set_env", set_enviroment},
	{"retest_count", tes_getretrycount},
	{"change_test_status", change_test_status},

	{nullptr, nullptr}
#else
	{"new", newarray},
	{"set", setarray},
	{"get", getarray},
	{"size", getsize},
	{nullptr, nullptr}
#endif
};

int pre_initialization(lua_State* pLuaState)
{
	int nIndex = (int)luaL_checkinteger(pLuaState,1);

	CTSE_SequenceManager* pSequenceManager = new CTSE_SequenceManager(nIndex);
	std::string strGlobalEnvironmentPoint = JGW_GetFormatString("GlobalEnvironmentPoint%d",nIndex);
	std::string strLogServicesPoint = JGW_GetFormatString("LogServicesPoint%d",nIndex);
	std::string strTestStatusServicePoint = JGW_GetFormatString("TestStatusServicePoint%d",nIndex);

	if (0 != lua_getglobal(pLuaState, strLogServicesPoint.c_str())) 
	{
		//const char* printValue = lua_tostring(pLuaState,-1);
		//pSequenceManager->AddLogListener((CTSE_LogListener*)atoi(printValue));
		CTSE_LogListener* pLogListener = (CTSE_LogListener*)lua_tointeger(pLuaState,-1); 
		pSequenceManager->AddLogListener(pLogListener);
		lua_pop(pLuaState, 1);
	}


	if (0 != lua_getglobal(pLuaState,strTestStatusServicePoint.c_str())) 
	{
		//const char* printValue = lua_tostring(pLuaState,-1);
		//pSequenceManager->AddTestStatusListener((CTSE_TestStatusListener*)atoi(printValue));
		CTSE_TestStatusListener* pTestStatusListener = (CTSE_TestStatusListener*)lua_tointeger(pLuaState,-1); 
		pSequenceManager->AddTestStatusListener(pTestStatusListener);
		//pTestStatusListener->OnTestStatusChange(L"aaaaaaaaaaaaa",JGW::E_INIT_TEST_STATUS);
		//pSequenceManager->GetTestStatusServiceImpl()->TestStatusChange(L"111111111111111",E_INIT_TEST_STATUS);
		//pTestStatusListener->OnTestStatusChange(L"aaaaaaaaaaaaa",JGW::E_INIT_TEST_STATUS);
		lua_pop(pLuaState, 1);
	}

	if (NULL == pSequenceManager->GetGlobalEnvironmentImpl())
	{
		lua_pushinteger(pLuaState, -1);
		lua_pushstring(pLuaState, "pre initialization error.");
		delete pSequenceManager;
	}
	else
	{
		{
			CCJGW_CriticalSectionAutoLock autoLock(gCriticalSectionLock);
			gvptrSequenceManager.push_back(std::unique_ptr<CTSE_SequenceManager>(pSequenceManager));
		}
		std::string strGlobalEnvironmentPoint = JGW::JGW_GetFormatString("GlobalEnvironmentPoint%d",nIndex);
		lua_pushinteger(pLuaState, (int)(pSequenceManager->GetGlobalEnvironmentImpl()));
		lua_setglobal(pLuaState, strGlobalEnvironmentPoint.c_str());

		lua_pushinteger(pLuaState,(int)pSequenceManager);
		lua_pushstring(pLuaState, "succeed");
	}

	return 2;
}

int create_tse(lua_State* pLuaState)
{
	size_t len = 0;
	CTSE_SequenceManager* pSequenceManager = (CTSE_SequenceManager*)luaL_checkinteger(pLuaState,1);

	std::string strModuleName = luaL_checklstring(pLuaState,2,&len);
	//std::cout.imbue(std::locale(""));
	//std::cout << "ModuleName : " << strModuleName << " ,Len : " << len << std::endl;
	std::string strRealName = luaL_checklstring(pLuaState,3,&len);
	//std::cout << "RealName : " << strRealName << " ,Len : " << len << std::endl;
	std::string strTestName = luaL_checklstring(pLuaState,4,&len);
	//std::cout << "TestName : " << JGW::JGW_W2A(JGW::JGW_A2W(strTestName,CP_UTF8))  << " ,Len : " << len << std::endl;

	lua_pushinteger(pLuaState,(int)(pSequenceManager->CreateSequence(JGW::JGW_A2W(strModuleName,CP_UTF8),strRealName,JGW::JGW_A2W(strTestName,CP_UTF8))));
	lua_pushstring(pLuaState, JGW_W2A(pSequenceManager->GetErrorMessage()).c_str());

	return 2;
}

int tse_addparam(lua_State* pLuaState)
{
	CTSE_TestBase* pTestBase = (CTSE_TestBase*)luaL_checkinteger(pLuaState,1);
	if (pTestBase)
	{
		size_t len = 0;
		std::string strParamName = luaL_checklstring(pLuaState,2,&len);
		std::string strParamValue = luaL_checklstring(pLuaState,3,&len);
		bool result = pTestBase->TSE_AddParam(JGW_A2W(strParamName,CP_UTF8).c_str(),JGW_A2W(strParamValue,CP_UTF8).c_str());
		//lua_pushinteger(pLuaState,result ? 1 : 0);
		lua_pushboolean(pLuaState,result);
		lua_pushstring(pLuaState, result ? "" : "Invalid parameter");
	}
	else
	{
		//lua_pushinteger(pLuaState,-1);
		lua_pushboolean(pLuaState,false);
		lua_pushstring(pLuaState, "null pointer exception");
	}
	return 2;
}

int tse_init(lua_State* pLuaState)
{
	CTSE_TestBase* pTestBase = (CTSE_TestBase*)luaL_checkinteger(pLuaState,1);
	if (pTestBase)
	{
		bool result = pTestBase->TSE_Init();
		//lua_pushinteger(pLuaState,result ? 1 : 0);
		lua_pushboolean(pLuaState,result);
		lua_pushstring(pLuaState, result ? "" : "init test fail");
	}
	else
	{
		//lua_pushinteger(pLuaState,-1);
		lua_pushboolean(pLuaState,false);
		lua_pushstring(pLuaState, "null pointer exception");
	}
	return 2;
}

int tse_run(lua_State* pLuaState)
{
	CTSE_TestBase* pTestBase = (CTSE_TestBase*)luaL_checkinteger(pLuaState,1);
	if (pTestBase)
	{
		bool result = pTestBase->TSE_Run();
		//lua_pushinteger(pLuaState,result ? 1 : 0);
		lua_pushboolean(pLuaState,result);
		lua_pushstring(pLuaState, result ? "" : "run test fail");
	}
	else
	{
		//lua_pushinteger(pLuaState,-1);
		lua_pushboolean(pLuaState,false);
		lua_pushstring(pLuaState, "null pointer exception");
	}
	return 2;
}

int tse_exit(lua_State* pLuaState)
{
	CTSE_TestBase* pTestBase = (CTSE_TestBase*)luaL_checkinteger(pLuaState,1);
	if (pTestBase)
	{
		bool result = pTestBase->TSE_Exit();
		//lua_pushinteger(pLuaState,result ? 1 : 0);
		lua_pushboolean(pLuaState,result);
		lua_pushstring(pLuaState, result ? "" : "exit test fail");
	}
	else
	{
		//lua_pushinteger(pLuaState,-1);
		lua_pushboolean(pLuaState,false);
		lua_pushstring(pLuaState, "null pointer exception");
	}
	return 2;
}


int tes_getretrycount(lua_State* pLuaState)
{
	CTSE_TestBase* pTestBase = (CTSE_TestBase*)luaL_checkinteger(pLuaState,1);
	if (pTestBase)
	{
		lua_pushinteger(pLuaState,pTestBase->TSE_GetErrorRetryTestCount());
		lua_pushstring(pLuaState,"");
	}
	else
	{
		lua_pushinteger(pLuaState,-1);
		lua_pushstring(pLuaState, "null pointer exception");
	}
	return 2;
}

int get_enviroment(lua_State* pLuaState)
{
	size_t len = 0;
	CTSE_SequenceManager* pSequenceManager = (CTSE_SequenceManager*)luaL_checkinteger(pLuaState,1);

	std::string strEnvironmentName = luaL_checklstring(pLuaState,2,&len);
	if (NULL == pSequenceManager || NULL == pSequenceManager->GetGlobalEnvironmentImpl() || strEnvironmentName.empty())
	{
#ifdef _DEBUG
		std::wstring strEnvironmentValue = JGW_A2W(strEnvironmentName,CP_UTF8);
		lua_pushstring(pLuaState, JGW_W2A(strEnvironmentValue).c_str());
#else
		lua_pushstring(pLuaState, "");
#endif

	}
	else
	{
		std::wstring strEnvironmentValue = pSequenceManager->GetGlobalEnvironmentImpl()->GetString(JGW_A2W(strEnvironmentName,CP_UTF8));
		lua_pushstring(pLuaState, JGW_W2A(strEnvironmentValue).c_str());
	}

	return 1;
}

int set_enviroment(lua_State* pLuaState)
{
	size_t len = 0;
	CTSE_SequenceManager* pSequenceManager = (CTSE_SequenceManager*)luaL_checkinteger(pLuaState,1);
	std::string strEnvironmentName = luaL_checklstring(pLuaState,2,&len);
	std::string strEnvironmentValue = luaL_checklstring(pLuaState,3,&len);

	if (NULL != pSequenceManager && NULL != pSequenceManager->GetGlobalEnvironmentImpl() && !strEnvironmentName.empty())
	{
		pSequenceManager->GetGlobalEnvironmentImpl()->PutString(JGW_A2W(strEnvironmentName,CP_UTF8),JGW_A2W(strEnvironmentValue,CP_UTF8));
		lua_pushinteger(pLuaState,1);
	}
	else
	{
		lua_pushinteger(pLuaState,0);
	}

	return 1;
}

int change_test_status(lua_State* pLuaState)
{
	CTSE_SequenceManager* pSequenceManager = (CTSE_SequenceManager*)luaL_checkinteger(pLuaState,1);
	if (NULL != pSequenceManager && NULL != pSequenceManager->GetTestStatusServiceImpl())
	{
		CTSE_TestBase* pTestBase = (CTSE_TestBase*)luaL_checkinteger(pLuaState,2);
		//! std::string strTestName = luaL_checklstring(pLuaState,2,&len);
		E_TEST_STATUS eTestStatus = (E_TEST_STATUS)luaL_checkinteger(pLuaState,3);
		pSequenceManager->GetTestStatusServiceImpl()->TestStatusChange(pTestBase->TSE_GetTestName().c_str(),eTestStatus);
		lua_pushinteger(pLuaState,1);
	}
	else
	{
		lua_pushinteger(pLuaState,0);
	}
	return 1;
}

int luaopen_TSE_SequenceLuaHelpPlugin(lua_State *pLuaState)
{
	//gpLuaState = pLuaState;
	//lua_register(gpLuaState, "register_log", RegisterTSELogCallback);
	luaL_newlib(pLuaState, gvTSELuaArrayLib);
	return 1;
}
