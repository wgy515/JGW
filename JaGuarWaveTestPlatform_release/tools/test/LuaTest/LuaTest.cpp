// LuaTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include <iostream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "TSE_TestSequenceLuaExec.h"
#include "CJGW_LuaHelp.h"


#if 1
//void OnTSELog(int logType,const char* strMessage)
//{
//    std::wcout << logType << L" : " << JGW::JGW_A2W_A(strMessage,CP_UTF8) << std::endl;
//}
//
//static int lua_OnTSELog(lua_State *L)
//{
//    int logType = (int)lua_tointeger(L,1);
//    size_t len = 0;
//    const char* szLogMsg = luaL_checklstring(L,2,&len);
//    OnTSELog(logType,szLogMsg);
//    return 0;
//}

//static int lua_TestRegGlobalFunc(lua_State *L)
//{
//    lua_pushnumber(L, 1234);
//    return 1; 
//}
//namespace JGW
//{
//	class CCJGW_LogListener : public CTSE_TestStatusListener
//	{
//	public:
//		void OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus)
//		{
//
//		}
//
//		void OnTestIndexChange(size_t index,int subIndex = -1)
//		{
//
//		}
//	};
//}


lua_State* LoadLuaConfig(const std::string& strLuaConfigFilePath)
{
	lua_State* pLuaState = luaL_newstate();
	luaL_openlibs(pLuaState);

	if (0 != luaL_loadfile(pLuaState,strLuaConfigFilePath.c_str()))
	{
		const char* strerr = lua_tostring(pLuaState,-1);
		printf("lua load error : %s\n",strerr);
		lua_close(pLuaState);
		return NULL;
	}

	if (0 != lua_pcall(pLuaState,0,LUA_MULTRET,0))
	{
		const char* strerr = lua_tostring(pLuaState,-1);
		printf("lua call error : %s\n",strerr);
		lua_close(pLuaState);
		return NULL;
	}

	return pLuaState;
}
//! "Execution successful."
/*
LUA_API void        (lua_pushnil) (lua_State *L);
LUA_API void        (lua_pushnumber) (lua_State *L, lua_Number n);
LUA_API void        (lua_pushinteger) (lua_State *L, lua_Integer n);
LUA_API const char *(lua_pushlstring) (lua_State *L, const char *s, size_t len);
LUA_API const char *(lua_pushstring) (lua_State *L, const char *s);
LUA_API const char *(lua_pushvfstring) (lua_State *L, const char *fmt,
va_list argp);
LUA_API const char *(lua_pushfstring) (lua_State *L, const char *fmt, ...);
LUA_API void  (lua_pushcclosure) (lua_State *L, lua_CFunction fn, int n);
LUA_API void  (lua_pushboolean) (lua_State *L, int b);
LUA_API void  (lua_pushlightuserdata) (lua_State *L, void *p);
LUA_API int   (lua_pushthread) (lua_State *L);
*/
void SetLuaGlobalVariableString(lua_State* pluaState,const std::string& strGlobalVariableName,const std::string& strGlobalVariableValue)
{
	lua_pushstring(pluaState, strGlobalVariableValue.c_str());
	lua_setglobal(pluaState, strGlobalVariableName.c_str());
}

void SetLuaGlobalVariableInteger(lua_State* pluaState,const std::string& strGlobalVariableName,long long llGlobalVariableValue)
{
	lua_pushinteger(pluaState, llGlobalVariableValue);
	lua_setglobal(pluaState, strGlobalVariableName.c_str());
}

void SetLuaGlobalVariableDouble(lua_State* pluaState,const std::string& strGlobalVariableName,double dfGlobalVariableValue)
{
	lua_pushnumber(pluaState, dfGlobalVariableValue);
	lua_setglobal(pluaState, strGlobalVariableName.c_str());
}

std::string GetLuaGlobalVariableString(lua_State* pluaState,const std::string& strGlobalVariableName,const std::string& strDefaultValue)
{
	if (0 != lua_getglobal(pluaState, strGlobalVariableName.c_str())) 
	{
		if (lua_isstring(pluaState,-1))
		{
			std::string strGlobalVariableValue = lua_tostring(pluaState,-1);
			lua_pop(pluaState, 1);
			return strGlobalVariableValue;
		}	
	}
	//! JGW::JGW_FormatString(strErrorMsg,"global variable(%s) does not exist",strLuaFuncName.c_str());
	return strDefaultValue;
}

long long GetLuaGlobalVariableInteger(lua_State* pluaState,const std::string& strGlobalVariableName,long long llDefaultValue)
{
	if (0 != lua_getglobal(pluaState, strGlobalVariableName.c_str())) 
	{
		if (lua_isinteger(pluaState,-1))
		{
			llDefaultValue = lua_tointeger(pluaState,-1);
			lua_pop(pluaState, 1);
		}
	}
	return llDefaultValue;
}

double GetLuaGlobalVariableDouble(lua_State* pluaState,const std::string& strGlobalVariableName,double dfDefaultValue)
{
	if (0 != lua_getglobal(pluaState, strGlobalVariableName.c_str())) 
	{
		if (lua_isnumber(pluaState,-1))
		{
			dfDefaultValue = lua_tonumber(pluaState,-1);
			lua_pop(pluaState, 1);
		}
	}
	return dfDefaultValue;
}

//bool ExecPreInitializationSequenceManagerLuaFunc(size_t index)
//{
//	lua_getglobal(L, "PreInitializationSequenceManager");
//	lua_pushinteger(L, nIndex); 
//	lua_call(L, 1, 1); 
//	bool result = 1 == lua_toboolean(L, -1) ? true : false; 
//	lua_pop(L, 1);
//	return result;
//}
//
//bool ExecCreateTestIDLuaFunc(size_t index)
//{
//	lua_getglobal(L, "CreateTestID");
//	lua_pushinteger(L, nIndex); 
//	lua_call(L, 1, 1); 
//	bool result = 1 == lua_toboolean(L, -1) ? true : false; 
//	lua_pop(L, 1);
//
//	return result;
//}

//! nargs: 要传递给Lua函数的参数个数。nresults: 期望从Lua函数获取的结果个数。
std::string ExecSingleIntParamReturnBoolLuaFunc(lua_State* pluaState,const std::string& strLuaFuncName,int nParam,bool& bResult)
{
	std::string strErrorMsg;
	bResult = false;
	//! 从 Lua 的全局表中查找名为 name 的变量，并将其值压入栈顶。若变量不存在，则压入 nil
	if (0 != lua_getglobal(pluaState, strLuaFuncName.c_str())) 
	{
		//! 检查当前全局变量是否是函数
		if (lua_isfunction(pluaState,-1))
		{
			//! 将函数参数值压栈
			lua_pushinteger(pluaState, nParam); 
			//! 开始调用当前函数
			if (LUA_OK == lua_pcall(pluaState, 1, 1,0))
			{
				bResult = (1 == lua_toboolean(pluaState, -1)) ? true : false; 
			}
			else
			{
				JGW::JGW_FormatString(strErrorMsg,"Error: %s\n", lua_tostring(pluaState, -1));
			}
		}
		else
		{
			JGW::JGW_FormatString(strErrorMsg,"global variable(%s) is not a function",strLuaFuncName.c_str());
		}
		lua_pop(pluaState, 1);
	}
	else
	{
		JGW::JGW_FormatString(strErrorMsg,"global func(%s) does not exist",strLuaFuncName.c_str());
	}

	return strErrorMsg;
}



//// 定义一个 C 函数
//static int _cdecl lua_test(lua_State *L) {
//	int a = lua_tointeger(L, 1);  // 获取第一个参数
//	int b = lua_tointeger(L, 2);  // 获取第二个参数
//	lua_pushinteger(L, a + b);  // 将结果推送到栈顶
//	return 1;  // 返回结果的数量
//}

int _tmain(int argc, _TCHAR* argv[])
{
	std::wcout.imbue(std::locale(""));
	JGW::CCJGW_LuaHelp cLuaHelp;

	cLuaHelp.InitLuaEnvironment();
	//! G:\git_code_debug\FTM-tools\JaGuarWaveTestPlatform\bin\debug\exec_debug\新建文件夹\Test6.lua
	if (!cLuaHelp.LoadLuaFile("G:\\git_code_debug\\FTM-tools\\JaGuarWaveTestPlatform\\bin\\debug\\exec_debug\\新建文件夹\\Test6.lua"))
	{
		std::wcout << cLuaHelp.GetErrorMessage() << std::endl;
		return -1;
	}

	int nIndex = 0;
	//std::string strGlobalEnvironmentPoint = JGW::JGW_GetFormatString("GlobalEnvironmentPoint%d",nIndex);
	//std::string strLogServicesPoint = JGW::JGW_GetFormatString("LogServicesPoint%d",nIndex);
	//std::string strTestStatusServicePoint = JGW::JGW_GetFormatString("TestStatusServicePoint%d",nIndex);

	//JGW::CTSE_GlobalEnvironment*  pGlobalEnvironment = new JGW::CTSE_GlobalEnvironment();
	//JGW::CTSE_LogServices* pLogServices = new JGW::CTSE_LogServices();
	//JGW::CTSE_TestStatusService* pTestStatusService = new JGW::CTSE_TestStatusService();
	//JGW::CTSE_TestSequenceLuaExec* pTestSequenceLuaExec = new JGW::CTSE_TestSequenceLuaExec(nIndex);
	//pLogServices->AddListener(pTestSequenceLuaExec);
	//pTestStatusService->AddListener(pTestSequenceLuaExec);

	//cLuaHelp.SetLuaGlobalVariableInteger(strGlobalEnvironmentPoint,(int)pGlobalEnvironment);
	//cLuaHelp.SetLuaGlobalVariableInteger(strLogServicesPoint,(int)pLogServices);
	//cLuaHelp.SetLuaGlobalVariableInteger(strTestStatusServicePoint,(int)pTestStatusService);

	JGW::CTSE_TestSequenceLuaExec* pTestSequenceLuaExec = new JGW::CTSE_TestSequenceLuaExec(nIndex);
	std::string strLogServicesPoint = JGW::JGW_GetFormatString("LogServicesPoint%d",nIndex);
	std::string strTestStatusServicePoint = JGW::JGW_GetFormatString("TestStatusServicePoint%d",nIndex);

	cLuaHelp.SetLuaGlobalVariableInteger(strLogServicesPoint,(int)((JGW::CTSE_TestSequenceLuaExec*)pTestSequenceLuaExec));

	//JGW::CTSE_TestStatusListener* pTestStatusListener = pTestSequenceLuaExec;
	cLuaHelp.SetLuaGlobalVariableInteger(strTestStatusServicePoint,(int)((JGW::CTSE_TestStatusListener*)pTestSequenceLuaExec));

#if 0
	std::string strGlobalMainHwnd;
	cLuaHelp.ExecDoubleIntStringParamReturnStringLuaFunc("GetEnv",nIndex,TSE_GLOBAL_MAIN_HWND_A,strGlobalMainHwnd);
	cLuaHelp.ExecThreeIntStringStringParamReturnVoidLuaFunc("SetEnv",nIndex,TSE_GLOBAL_MAIN_HWND_A,"0");
#endif

	bool result = false;
	if (!cLuaHelp.ExecSingleIntParamReturnBoolLuaFunc("PreInitializationSequenceManager",nIndex,result))
	{
		std::wcout << cLuaHelp.GetErrorMessage() << std::endl;
		return -1;
	}



	std::wcout << L"PreInitializationSequenceManager : " << (result ? L"PASS" : L"FAIL") << std::endl;

	if (!cLuaHelp.ExecSingleIntParamReturnBoolLuaFunc("CreateTestID",nIndex,result))
	{
		std::wcout << cLuaHelp.GetErrorMessage() << std::endl;
		return -1;
	}
	std::wcout << L"CreateTestID : " << (result ? L"PASS" : L"FAIL") << std::endl;

	if (!cLuaHelp.ExecSingleIntParamReturnBoolLuaFunc("AddParam",nIndex,result))
	{
		std::wcout << cLuaHelp.GetErrorMessage() << std::endl;
		return -1;
	}
	std::wcout << L"AddParam : " << (result ? L"PASS" : L"FAIL") << std::endl;

	if (!cLuaHelp.ExecSingleIntParamReturnBoolLuaFunc("InitTSETest",nIndex,result))
	{
		std::wcout << cLuaHelp.GetErrorMessage() << std::endl;
		return -1;
	}
	std::wcout << L"InitTSETest : " << (result ? L"PASS" : L"FAIL") << std::endl;
#if 0
	if (!cLuaHelp.ExecSingleIntParamReturnBoolLuaFunc("RunTSETest",nIndex,result))
	{
		std::wcout << cLuaHelp.GetErrorMessage() << std::endl;
		return -1;
	}
	std::wcout << L"RunTSETest : " << (result ? L"PASS" : L"FAIL") << std::endl;

	if (!cLuaHelp.ExecSingleIntParamReturnBoolLuaFunc("ExitTSETest",nIndex,result))
	{
		std::wcout << cLuaHelp.GetErrorMessage() << std::endl;
		return -1;
	}
	std::wcout << L"ExitTSETest : " << (result ? L"PASS" : L"FAIL") << std::endl;
#else
	if (!cLuaHelp.ExecSingleIntParamReturnBoolLuaFunc("ExecuteTSETest",nIndex,result))
	{
		std::wcout << cLuaHelp.GetErrorMessage() << std::endl;
		return -1;
	}
	std::wcout << L"ExecuteTSETest : " << (result ? L"PASS" : L"FAIL") << std::endl;	
#endif

	//lua_State *L = luaL_newstate();  // 创建 Lua 状态机
	//luaL_openlibs(L);  // 打开所有标准库

	//// 将一个整数推送到栈上，并创建一个闭包
	//lua_pushinteger(L, 10);  // 推送整数10作为闭包的上值
	//lua_pushcclosure(L, lua_test, 1);  // 创建闭包并推送到栈上

	//if (luaL_dostring(L, "lua_test(1,2)") == LUA_OK) {
	//	printf("Result: %d\n", lua_tointeger(L, -1));  // 读取并打印结果
	//} else {
	//	printf("Error: %s\n", lua_tostring(L, -1));  // 打印错误信息
	//}

	//lua_close(L);  // 关闭 Lua 状态机
	//return 0;


#if 0







#if 1
	std::wcout.imbue(std::locale(""));

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    //luaL_dofile(L, "G:\\git_code_debug\\FTM-tools\\JaGuarWaveTestPlatform\\bin\\debug\\exec_debug\\Test5.lua");
    if (0 != luaL_loadfile(L,"G:\\git_code_debug\\FTM-tools\\JaGuarWaveTestPlatform\\bin\\debug\\exec_debug\\新建文件夹\\Test6.lua"))
    {
        const char* strerr = lua_tostring(L,-1);
        printf("lua load error : %s\n",strerr);
        return -1;
    }

    if (0 != lua_pcall(L,0,LUA_MULTRET,0))
    {
        const char* strerr = lua_tostring(L,-1);
        printf("lua call error : %s\n",strerr);
        return -1;
    }

    //std::wcout.imbue(std::locale(""));
#if 0
    int index = lua_getglobal(L, "add"); // 获取全局函数add，并压入栈顶
    const char* strerr = lua_tostring(L,-1);
    printf("lua call error : %s\n",strerr);
    lua_pushinteger(L, 30); // 将整型的值30压入栈顶
    lua_pushinteger(L, 50); // 将整型的值50压入栈顶
    lua_call(L, 2, 1); // 对栈顶的30和50执行add函数调用，执行完后将30, 50，add弹出栈，将结果80压栈  注：2为参数个数，1为返回值个数

    int sum = (int)lua_tointeger(L, -1); // 将栈顶80赋值给sum变量
    lua_pop(L, 1); // 从栈顶弹出1个元素
#endif
#if 0
    //lua_register(L, "onTSELog", lua_OnTSELog); 
    // 设置全局变量 "myGlobalVar" 为一个字符串 "Hello from C"
    lua_pushstring(L, "Hello from C");
    lua_setglobal(L, "myGlobalVar");

    lua_register(L,"TestRegGlobalFunc",lua_TestRegGlobalFunc);

    //! LogCallbackPointer
    // ******读取名为sayhi的字符串全局变量的值******
    int result1 = lua_getglobal(L, "LogCallbackPointer");// 将名为sayhi的全局string变量的值压栈
    lua_pop(L, 1); // 从栈顶弹出1个元素
    lua_pushinteger(L,(int)OnTSELog);
    lua_setglobal(L, "LogCallbackPointer");// 将栈顶的元素设置给全局变量sayhi

    lua_getglobal(L, "LogCallbackPointer");
    int index = (int)lua_tointeger(L,1); 
#endif

	int nIndex = 0;
	std::string strGlobalEnvironmentPoint = JGW::JGW_GetFormatString("GlobalEnvironmentPoint%d",nIndex);
	std::string strLogServicesPoint = JGW::JGW_GetFormatString("LogServicesPoint%d",nIndex);
	std::string strTestStatusServicePoint = JGW::JGW_GetFormatString("TestStatusServicePoint%d",nIndex);

	JGW::CTSE_GlobalEnvironment*  pGlobalEnvironment = new JGW::CTSE_GlobalEnvironment();
	JGW::CTSE_LogServices* pLogServices = new JGW::CTSE_LogServices();
	JGW::CTSE_TestStatusService* pTestStatusService = new JGW::CTSE_TestStatusService();
	JGW::CTSE_TestSequenceLuaExec* pTestSequenceLuaExec = new JGW::CTSE_TestSequenceLuaExec(nIndex);
	pLogServices->AddListener(pTestSequenceLuaExec);
	pTestStatusService->AddListener(pTestSequenceLuaExec);
	// 
	lua_pushinteger(L, (int)(pGlobalEnvironment));
	lua_setglobal(L, strGlobalEnvironmentPoint.c_str());

	lua_pushinteger(L, (int)(pLogServices));
	lua_setglobal(L, strLogServicesPoint.c_str());

	lua_pushinteger(L, (int)(pTestStatusService));
	lua_setglobal(L, strTestStatusServicePoint.c_str());

    lua_getglobal(L, "PreInitializationSequenceManager");
    lua_pushinteger(L, nIndex); 
    lua_call(L, 1, 1); 
	bool result = 1 == lua_toboolean(L, -1) ? true : false; 
	lua_pop(L, 1);

	lua_getglobal(L, "CreateTestID");
	lua_pushinteger(L, nIndex); 
	lua_call(L, 1, 1); 
	result = 1 == lua_toboolean(L, -1) ? true : false; 
	lua_pop(L, 1);

	lua_getglobal(L, "AddParam");
	lua_pushinteger(L, nIndex); 
	lua_call(L, 1, 1); 
	result = 1 == lua_toboolean(L, -1) ? true : false; 
	lua_pop(L, 1);

	lua_getglobal(L, "InitTSETest");
	lua_pushinteger(L, nIndex); 
	lua_call(L, 1, 1); 
	result = 1 == lua_toboolean(L, -1) ? true : false; 
	lua_pop(L, 1);

#if 0
    lua_getglobal(L, "ExecuteTSETest");
    lua_pushinteger(L, 30);
    lua_call(L, 1, 1);
    bool result = 1 == lua_toboolean(L, -1) ? true : false; 
    lua_pop(L, 1);
    printf("MyArrayTest Test Result : %s",result ? "true" : "false");
#endif

    lua_close(L);
    return 0;
#else

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    //luaL_requiref(L, "myarray", luaopen_MyArray, 1); // 将MyArray相关方法注册到全局表中，lua中的名为myarray

    luaL_dofile(L, "G:\\git_code_debug\\FTM-tools\\JaGuarWaveTestPlatform\\bin\\debug\\exec_debug\\Test5.lua");

    lua_getglobal(L, "MyArrayTest"); // 将函数名MyArrayTest压栈
    lua_pushinteger(L, 1000); // 传入MyArray的size为1000 压栈
    lua_call(L, 1, 0); // 执行完后将MyArrayTest弹出栈  注: 1表示参数个数，第二个0表示无返回值

    lua_close(L);
    return 0;
#endif
#endif
}

#else
//
//class MyArray
//{
//public:
//    int GetSize() { return size; }
//    void SetSize(int insize) { size = insize; }
//
//    double GetAt(int index) { return values[index]; }
//    void SetAt(int index, double value) { values[index] = value; }
//private:
//    int size;
//    double values[1];
//};
//
//static int newarray(lua_State* L)
//{
//    int n = (int)luaL_checkinteger(L, 1); // 检查栈上索引为1处的元素是否为整型，并返回该元素
//    size_t nbytes = sizeof(MyArray) + (n - 1) * sizeof(double);
//    MyArray* a = (MyArray*)lua_newuserdata(L, nbytes); // 创建一个userdata，并压栈
//    a->SetSize(n);// 设置数组的大小
//    return 1;//表示有1个返回值
//}
//
//static int setarray(lua_State* L)
//{
//    MyArray* a = (MyArray*)lua_touserdata(L, 1);// 获取栈上索引为1处的元素，并转换为userdata指针
//    int index = (int)luaL_checkinteger(L, 2);// 检查栈上索引为1处的元素是否为整型，并返回该元素
//    double value = luaL_checknumber(L, 3);// 检查栈上索引为1处的元素是否为number类型，并返回该元素
//
//    luaL_argcheck(L, a != nullptr, 1, "'array' expected");
//    luaL_argcheck(L, 1 <= index && index <= a->GetSize(), 2, "index out of range");
//    a->SetAt(index-1, value); // 设置数组索引为index-1处的值为value
//
//    return 0; //表示没有返回值
//}
//
//static int getarray(lua_State* L)
//{
//    MyArray* a = (MyArray*)lua_touserdata(L, 1);// 获取栈上索引为1处的元素，并转换为userdata指针
//    int index = (int)luaL_checkinteger(L, 2);// 检查栈上索引为1处的元素是否为整型，并返回该元素
//
//    luaL_argcheck(L, a != nullptr, 1, "'array' expected");
//    luaL_argcheck(L, 1 <= index && index <= a->GetSize(), 2, "index out of range");
//    double value = a->GetAt(index-1);// 获取数组索引为index-1处的值
//
//    lua_pushnumber(L, value); // 将获取的值压栈
//
//    return 1;//表示有1个返回值
//}
//
//static int getsize(lua_State* L)
//{
//    MyArray* a = (MyArray*)lua_touserdata(L, 1); // 获取栈上索引为1处的元素，并转换为userdata指针
//    luaL_argcheck(L, a != nullptr, 1, "'array' expected");
//    lua_pushnumber(L, a->GetSize());    // 获取数组的大小并压栈
//
//    return 1;//表示有1个返回值
//}
//
//static const struct luaL_Reg MyArrayLib[] = {
//    {"new", newarray},
//    {"set", setarray},
//    {"get", getarray},
//    {"size", getsize},
//    {nullptr, nullptr}
//};
//
//int luaopen_MyArray(lua_State* L)
//{
//    luaL_newlib(L, MyArrayLib);
//    return 1;
//}

int main(int argc, char *argv[])
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    //luaL_requiref(L, "myarray", luaopen_MyArray, 1); // 将MyArray相关方法注册到全局表中，lua中的名为myarray

    luaL_dofile(L, "G:\\git_code_debug\\FTM-tools\\JaGuarWaveTestPlatform\\bin\\debug\\exec_debug\\Test5.lua");

    lua_getglobal(L, "MyArrayTest"); // 将函数名MyArrayTest压栈
    lua_pushinteger(L, 1000); // 传入MyArray的size为1000 压栈
    lua_call(L, 1, 0); // 执行完后将MyArrayTest弹出栈  注: 1表示参数个数，第二个0表示无返回值

    lua_close(L);
    return 0;
}


#endif
