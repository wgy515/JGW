// TSE_SequenceManagerLuaPlugin.cpp : 定义 DLL 应用程序的导出函数。
//
#include "stdafx.h"




#if 0
#include "stdafx.h"
#include <TSE_SequenceManagerLuaPlugin/TSE_SequenceManagerLuaPlugin.h>
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <string>
#include <iostream>
#include "LuaTSEBaseTest.h"
#include "TSE_SequenceManager.h"
#include <memory>
#include <vector>
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>
using namespace JGW;
//! TSE 注册的类指针供 TSE DLL回调使用
//CTSE_LogServices& mLogServices;
//CTSE_TestStatusService& mTestStatusService;
//CTSE_GlobalEnvironment& mGlobalEnvironment;
//CTSE_GlobalEnvironmentImpl* gpGlobalEnvironmentImpl = NULL;
//CTSE_LogServicesImpl* gpLogServicesImpl = NULL;
//CTSE_TestStatusServiceImpl* gpTestStatusServiceImpl = NULL;
lua_State* gpLuaState = NULL;
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
		pTestStatusListener->OnTestStatusChange(L"aaaaaaaaaaaaa",JGW::E_INIT_TEST_STATUS);
		pSequenceManager->GetTestStatusServiceImpl()->TestStatusChange(L"111111111111111",E_INIT_TEST_STATUS);
		pTestStatusListener->OnTestStatusChange(L"aaaaaaaaaaaaa",JGW::E_INIT_TEST_STATUS);
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

int luaopen_TSE_SequenceManagerLuaPlugin(lua_State *pLuaState)
{
	gpLuaState = pLuaState;
	//lua_register(gpLuaState, "register_log", RegisterTSELogCallback);
	luaL_newlib(pLuaState, gvTSELuaArrayLib);
	return 1;
}


#if 0
//! TSE 注册的类指针供 TSE DLL回调使用
//CTSE_LogServices& mLogServices;
//CTSE_TestStatusService& mTestStatusService;
//CTSE_GlobalEnvironment& mGlobalEnvironment;



#if 1
lua_State* gpLuaState = NULL;

typedef void (*OnTSELogFunc)(int logType,const char* strMessage);
OnTSELogFunc gpOnTSELogFunc = NULL;


void OnTSELog(int logType,const char*  strMessage);

enum LogTypePluginDomain
{
	Debug = 0,
	Info = 1,
	Warn = 2,
	Error = 3,
	UserDefine = 4
};

static int RegisterTSELogCallback(lua_State *L)
{
	lua_settop(L, 0); 

	OnTSELog(12,"RegisterTSELogCallback");
	return 0;
}

static int Average(lua_State *L)
{
	printf("TestRegGlobalFunc : %d\n",lua_getglobal(L, "TestRegGlobalFunc"));
	lua_call(L, 0, 1);
	int sum_callback = (int)lua_tointeger(L, -1); // 将栈顶80赋值给sum变量
	printf("sum_callback : %d\n",sum_callback);
	lua_pop(L, 1); // 从栈顶弹出1个元素

	printf("myGlobalVar : %d\n",lua_getglobal(L, "myGlobalVar"));// 将名为sayhi的全局string变量的值压栈
	//int index = (int)lua_tointeger(L,1); 
	const char* printValue = lua_tostring(L,-1);
	lua_pop(L, 1); // 从栈顶弹出1个元素



	int n = lua_gettop(L);  // 获取栈上元素的个数
	double sum = 0;
	for (int i = 1; i <= n; ++i)
	{
		sum += lua_tonumber(L, i); // 依次去除索引为1到n的元素，并累加到sum
	}
	lua_pushnumber(L, sum / n); // 将sum/n计算得到平均值压栈
	lua_pushnumber(L, sum);  // 将sum压栈

	return 2;  // 表明有2个返回值
}

static int create_tse(lua_State* L)
{
#if 0
	int n = (int)luaL_checkinteger(L, 1); // 检查栈上索引为1处的元素是否为整型，并返回该元素
	size_t nbytes = sizeof(JGW::CTSE_TestBase) + (n - 1) * sizeof(double);
	JGW::CTSE_TestBase* pcTestBase = (JGW::CTSE_TestBase*)lua_newuserdata(L, nbytes); // 创建一个userdata，并压栈
	pcTestBase->SetSize(n);// 设置数组的大小
	return 1;//表示有1个返回值
#else
	size_t len = 0;

	std::string strModuleName = luaL_checklstring(L,1,&len);
	std::cout.imbue(std::locale(""));
	std::cout << "ModuleName : " << strModuleName << " ,Len : " << len << std::endl;
	std::string strRealName = luaL_checklstring(L,2,&len);
	std::cout << "RealName : " << strRealName << " ,Len : " << len << std::endl;
	std::string strTestName = luaL_checklstring(L,3,&len);
	std::cout << "TestName : " << JGW::JGW_W2A(JGW::JGW_A2W(strTestName,CP_UTF8))  << " ,Len : " << len << std::endl;

	size_t nbytes = sizeof(JGW::CLuaTSEBaseTest) * 2;
	JGW::CLuaTSEBaseTest* pcTestBase = (JGW::CLuaTSEBaseTest*)lua_newuserdata(L, nbytes); // 创建一个userdata，并压栈
	//*pcTestBase = new JGW::CLuaTSEBaseTest();
	JGW::CLuaTSEBaseTest* obj = new(pcTestBase) JGW::CLuaTSEBaseTest();
	//pcTestBase->SettingTSETestBase(new JGW::CTSE_TestBase);

	//lua_getglobal(L, "LogCallbackPointer");
	//int index = (int)lua_tointeger(gpLuaState,1); 
	//OnTSELog(6,"create_tse(lua_State* L)");

	return 1;
#endif
}

static int addtseparam(lua_State* L)
{
	JGW::CLuaTSEBaseTest* pcTestBase = (JGW::CLuaTSEBaseTest*)lua_touserdata(L, 1);// 获取栈上索引为1处的元素，并转换为userdata指针

	size_t len = 0;
	std::string strParamName = luaL_checklstring(L,2,&len);
	std::cout << "ParamName : " << strParamName << " ,Len : " << len << std::endl;
	std::string strParamValue = luaL_checklstring(L,3,&len);
	std::cout << "ParamValue : " << strParamValue << " ,Len : " << len << std::endl;
	std::cout.imbue(std::locale(""));
	std::cout << strParamName << " : " << strParamValue << std::endl;
	//luaL_argcheck(L, pcTestBase != nullptr, 1, "'TSE' expected");
	bool bResult = false;
	if (NULL != pcTestBase)
	{
		bResult = pcTestBase->TSE_AddParam(JGW::JGW_A2W(strParamName,CP_UTF8).c_str(),JGW::JGW_A2W(strParamValue,CP_UTF8).c_str());
		std::cout << bResult << std::endl;
	}
	else
	{
		std::cout << "null expected" << len << std::endl;
	}
	lua_pushboolean(L, bResult); // 将获取的值压栈

	return 1;
}
#if 1
static int inittse(lua_State* L)
{
	JGW::CLuaTSEBaseTest* pcTestBase = (JGW::CLuaTSEBaseTest*)lua_touserdata(L, 1);// 获取栈上索引为1处的元素，并转换为userdata指针
	luaL_argcheck(L, pcTestBase != nullptr, 1, "'TSE' expected");
	bool bResult = (pcTestBase)->TSE_Init();
	lua_pushboolean(L, bResult); // 将获取的值压栈
	//delete *pcTestBase;
	//*pcTestBase = NULL;
	return 1;
}

//int lua_auto_gc(lua_State* L)
//{
//    Student** s = (Student**)luaL_checkudata(L, 1, "StudentClass");
//    luaL_argcheck(L, s != NULL, 1, "invalid user data");
//
//    if (s) {
//        delete *s;
//    }
//
//    return 0;
//}

static int runtse(lua_State* L)
{
	JGW::CLuaTSEBaseTest* pcTestBase = (JGW::CLuaTSEBaseTest*)lua_touserdata(L, 1);// 获取栈上索引为1处的元素，并转换为userdata指针
	luaL_argcheck(L, pcTestBase != nullptr, 1, "'TSE' expected");

	bool bResult = pcTestBase->TSE_Run();
	lua_pushboolean(L, bResult); // 将获取的值压栈

	return 1; 
}

static int exittse(lua_State* L)
{
	JGW::CLuaTSEBaseTest* pcTestBase = (JGW::CLuaTSEBaseTest*)lua_touserdata(L, 1);// 获取栈上索引为1处的元素，并转换为userdata指针
	luaL_argcheck(L, pcTestBase != nullptr, 1, "'TSE' expected");

	bool bResult = pcTestBase->TSE_Exit();
	lua_pushboolean(L, bResult); // 将获取的值压栈

	return 1; 
}

static int ErrorRetryTestCount(lua_State* L)
{
	JGW::CLuaTSEBaseTest* pcTestBase = (JGW::CLuaTSEBaseTest*)lua_touserdata(L, 1);// 获取栈上索引为1处的元素，并转换为userdata指针
	luaL_argcheck(L, pcTestBase != nullptr, 1, "'TSE' expected");

	int nErrorRetryTestCount = pcTestBase->TSE_GetErrorRetryTestCount();
	lua_pushnumber(L, nErrorRetryTestCount); // 将获取的值压栈

	return 1; 
}

#endif

static const struct luaL_Reg MyArrayLib[] = {
#if 1
	{"new", create_tse},
	{"addParam", addtseparam},
	{"init", inittse},
	{"run", runtse},
	{"exit", exittse},
	{"retestcount", ErrorRetryTestCount},
	{nullptr, nullptr}
#else
	{"new", newarray},
	{"set", setarray},
	{"get", getarray},
	{"size", getsize},
	{nullptr, nullptr}
#endif
};

void OnTSELog(int logType,const char* strMessage)
{

#if 0
	lua_getglobal(gpLuaState, "TSELog");
	lua_pushinteger(gpLuaState, logType); 
	lua_pushlstring(gpLuaState, strMessage.c_str(),strMessage.length());
	lua_call(gpLuaState, 2, 0); 
#else
	if (NULL == gpOnTSELogFunc)
	{
		lua_getglobal(gpLuaState, "LogCallbackPointer");// 将名为sayhi的全局string变量的值压栈
		int index = (int)lua_tointeger(gpLuaState,1); 
		gpOnTSELogFunc = (OnTSELogFunc)lua_tointeger(gpLuaState,1); 
		if (NULL != gpOnTSELogFunc) gpOnTSELogFunc(logType,strMessage);
	}
	else
	{
		gpOnTSELogFunc(logType,strMessage);
	}
#endif
}

int luaopen_TSE_SequenceManagerLuaPlugin(lua_State *L)
{
	gpLuaState = L;
	lua_register(L, "average", Average); // 将c++的Average函数注册成lua中的average方法
	lua_register(L, "register_log", RegisterTSELogCallback); // 将c++的Average函数注册成lua中的average方法

	luaL_newlib(L, MyArrayLib);
	return 1;
}

#else


class MyArray
{
public:
	int GetSize() { return size; }
	void SetSize(int insize) { size = insize; }

	double GetAt(int index) { return values[index]; }
	void SetAt(int index, double value) { values[index] = value; }
private:
	int size;
	double values[1];
};

static int newarray(lua_State* L)
{
	int n = (int)luaL_checkinteger(L, 1); // 检查栈上索引为1处的元素是否为整型，并返回该元素
	//size_t nbytes = sizeof(JGW::CLuaTSEBaseTest) + (n - 1) * sizeof(double);
	size_t nbytes = sizeof(JGW::CLuaTSEBaseTest) * 2;
	size_t len = 0;
	std::string strModuleName = luaL_checklstring(L,2,&len);
	std::cout.imbue(std::locale(""));
	std::cout << "ModuleName : " << strModuleName << " ,Len : " << len << std::endl;
	std::string strRealName = luaL_checklstring(L,3,&len);
	std::cout << "RealName : " << strRealName << " ,Len : " << len << std::endl;
	std::string strTestName = luaL_checklstring(L,4,&len);
	std::cout << "TestName : " << JGW::JGW_W2A(JGW::JGW_A2W(strTestName,CP_UTF8))  << " ,Len : " << len << std::endl;

	JGW::CLuaTSEBaseTest* a = (JGW::CLuaTSEBaseTest*)lua_newuserdata(L, nbytes); // 创建一个userdata，并压栈
	JGW::CLuaTSEBaseTest* obj = new(a) JGW::CLuaTSEBaseTest();

	//obj->SetSize(n);// 设置数组的大小
	return 1;//表示有1个返回值
}

static int setarray(lua_State* L)
{
	MyArray* a = (MyArray*)lua_touserdata(L, 1);// 获取栈上索引为1处的元素，并转换为userdata指针
	int index = (int)luaL_checkinteger(L, 2);// 检查栈上索引为1处的元素是否为整型，并返回该元素
	double value = luaL_checknumber(L, 3);// 检查栈上索引为1处的元素是否为number类型，并返回该元素

	luaL_argcheck(L, a != nullptr, 1, "'array' expected");
	luaL_argcheck(L, 1 <= index && index <= a->GetSize(), 2, "index out of range");
	a->SetAt(index-1, value); // 设置数组索引为index-1处的值为value

	return 0; //表示没有返回值
}

static int getarray(lua_State* L)
{
	MyArray* a = (MyArray*)lua_touserdata(L, 1);// 获取栈上索引为1处的元素，并转换为userdata指针
	int index = (int)luaL_checkinteger(L, 2);// 检查栈上索引为1处的元素是否为整型，并返回该元素

	luaL_argcheck(L, a != nullptr, 1, "'array' expected");
	luaL_argcheck(L, 1 <= index && index <= a->GetSize(), 2, "index out of range");
	double value = a->GetAt(index-1);// 获取数组索引为index-1处的值

	lua_pushnumber(L, value); // 将获取的值压栈

	return 1;//表示有1个返回值
}

static int getsize(lua_State* L)
{
	MyArray* a = (MyArray*)lua_touserdata(L, 1); // 获取栈上索引为1处的元素，并转换为userdata指针
	luaL_argcheck(L, a != nullptr, 1, "'array' expected");
	lua_pushnumber(L, a->GetSize());    // 获取数组的大小并压栈

	return 1;//表示有1个返回值
}

static const struct luaL_Reg MyArrayLib[] = {
	{"new", newarray},
	{"set", setarray},
	{"get", getarray},
	{"size", getsize},
	{nullptr, nullptr}
};

//int luaopen_MyArray(lua_State* L)
//{
//    luaL_newlib(L, MyArrayLib);
//    return 1;
//}

int luaopen_TSE_SequenceManagerLuaPlugin(lua_State *L)
{
	//gpLuaState = L;
	//lua_register(L, "average", Average); // 将c++的Average函数注册成lua中的average方法
	//lua_register(L, "register_log", RegisterTSELogCallback); // 将c++的Average函数注册成lua中的average方法

	luaL_newlib(L, MyArrayLib);
	return 1;
}


#endif
#endif
#endif
