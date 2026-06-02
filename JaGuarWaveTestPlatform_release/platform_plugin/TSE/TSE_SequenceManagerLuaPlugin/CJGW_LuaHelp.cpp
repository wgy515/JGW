#include "StdAfx.h"
#include "CJGW_LuaHelp.h"
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
	CCJGW_LuaHelp::CCJGW_LuaHelp(void) : mpLuaState(NULL)
	{
	}


	CCJGW_LuaHelp::~CCJGW_LuaHelp(void)
	{
		UnInitLuaEnvironment();
	}

	const std::wstring& CCJGW_LuaHelp::GetErrorMessage()
	{
		return mstrErrorMessage;
	}

	void* CCJGW_LuaHelp::GetLuaState()
	{
		return mpLuaState;
	}

	void CCJGW_LuaHelp::UnInitLuaEnvironment()
	{
		if (NULL != mpLuaState)
		{
			lua_close((lua_State*)mpLuaState);
			mpLuaState = NULL;
		}
	}

	bool CCJGW_LuaHelp::InitLuaEnvironment()
	{
		lua_State* pLuaState = luaL_newstate();
		if (NULL == pLuaState)
		{
			mstrErrorMessage = L"The state machine cannot be created";
			return false;
		}
		luaL_openlibs(pLuaState);
		mpLuaState = pLuaState;

		return true;
	}

	bool CCJGW_LuaHelp::LoadLuaFile(const std::string& strLuaFilePath)
	{
		lua_State* pLuaState = (lua_State*)mpLuaState;

		if (0 != luaL_loadfile(pLuaState,strLuaFilePath.c_str()))
		{
			const char* strerr = lua_tostring(pLuaState,-1);
			JGW_FormatWString(mstrErrorMessage,L"lua load error : %s",JGW_A2W_A(strerr).c_str());
			return false;
		}

		if (0 != lua_pcall(pLuaState,0,LUA_MULTRET,0))
		{
			const char* strerr = lua_tostring(pLuaState,-1);
			JGW_FormatWString(mstrErrorMessage,L"lua call error : %s",JGW_A2W_A(strerr).c_str());
			return false;
		}

		return true;
	}

	void CCJGW_LuaHelp::SetLuaGlobalVariableString(const std::string& strGlobalVariableName,const std::string& strGlobalVariableValue)
	{
		lua_State* pLuaState = (lua_State*)mpLuaState;
		lua_pushstring(pLuaState, strGlobalVariableValue.c_str());
		lua_setglobal(pLuaState, strGlobalVariableName.c_str());
	}

	void CCJGW_LuaHelp::SetLuaGlobalVariableInteger(const std::string& strGlobalVariableName,long long llGlobalVariableValue)
	{
		lua_State* pLuaState = (lua_State*)mpLuaState;
		lua_pushinteger(pLuaState, llGlobalVariableValue);
		lua_setglobal(pLuaState, strGlobalVariableName.c_str());
	}

	void CCJGW_LuaHelp::SetLuaGlobalVariableDouble(const std::string& strGlobalVariableName,double dfGlobalVariableValue)
	{
		lua_State* pLuaState = (lua_State*)mpLuaState;
		lua_pushnumber(pLuaState, dfGlobalVariableValue);
		lua_setglobal(pLuaState, strGlobalVariableName.c_str());
	}

	std::string CCJGW_LuaHelp::GetLuaGlobalVariableString(const std::string& strGlobalVariableName,const std::string& strDefaultValue /* = "" */)
	{
		lua_State* pLuaState = (lua_State*)mpLuaState;
		if (0 != lua_getglobal(pLuaState, strGlobalVariableName.c_str())) 
		{
			if (lua_isstring(pLuaState,-1))
			{
				std::string strGlobalVariableValue = lua_tostring(pLuaState,-1);
				lua_pop(pLuaState, 1);
				return strGlobalVariableValue;
			}	
		}
		//! JGW::JGW_FormatString(strErrorMsg,"global variable(%s) does not exist",strLuaFuncName.c_str());
		return strDefaultValue;
	}

	long long CCJGW_LuaHelp::GetLuaGlobalVariableInteger(const std::string& strGlobalVariableName,long long llDefaultValue)
	{
		lua_State* pLuaState = (lua_State*)mpLuaState;
		if (0 != lua_getglobal(pLuaState, strGlobalVariableName.c_str())) 
		{
			if (lua_isinteger(pLuaState,-1))
			{
				llDefaultValue = lua_tointeger(pLuaState,-1);
				lua_pop(pLuaState, 1);
			}
		}
		return llDefaultValue;
	}

	double CCJGW_LuaHelp::GetLuaGlobalVariableDouble(const std::string& strGlobalVariableName,double dfDefaultValue)
	{
		lua_State* pLuaState = (lua_State*)mpLuaState;
		if (0 != lua_getglobal(pLuaState, strGlobalVariableName.c_str())) 
		{
			if (lua_isnumber(pLuaState,-1))
			{
				dfDefaultValue = lua_tonumber(pLuaState,-1);
				lua_pop(pLuaState, 1);
			}
		}
		return dfDefaultValue;
	}

	bool CCJGW_LuaHelp::ExecSingleIntParamReturnBoolLuaFunc(const std::string& strLuaFuncName,int nParam,bool& bResult)
	{
		lua_State* pLuaState = (lua_State*)mpLuaState;
		bResult = false;
		//! 从 Lua 的全局表中查找名为 name 的变量，并将其值压入栈顶。若变量不存在，则压入 nil
		if (0 != lua_getglobal(pLuaState, strLuaFuncName.c_str())) 
		{
			//! 检查当前全局变量是否是函数
			if (lua_isfunction(pLuaState,-1))
			{
				//! 将函数参数值压栈
				lua_pushinteger(pLuaState, nParam); 
				//! 开始调用当前函数
				if (LUA_OK == lua_pcall(pLuaState, 1, 1,0))
				{
					if (lua_isboolean(pLuaState,-1))
					{
						bResult = (1 == lua_toboolean(pLuaState, -1)) ? true : false; 
					}
					lua_pop(pLuaState, 1);
					return true;
				}
				else
				{
					JGW::JGW_FormatWString(mstrErrorMessage,L"Error: %s\n", JGW_A2W_A(lua_tostring(pLuaState, -1)).c_str());
				}
			}
			else
			{
				JGW::JGW_FormatWString(mstrErrorMessage,L"global variable(%s) is not a function",JGW_A2W(strLuaFuncName).c_str());
			}
			lua_pop(pLuaState, 1);
		}
		else
		{
			JGW::JGW_FormatWString(mstrErrorMessage,L"global func(%s) does not exist",JGW_A2W(strLuaFuncName).c_str());
		}

		return false;
	}

	bool CCJGW_LuaHelp::ExecDoubleIntStringParamReturnStringLuaFunc(const std::string& strLuaFuncName,int nParam,const std::string& strParam1,std::string& strResult)
	{
		lua_State* pLuaState = (lua_State*)mpLuaState;
		//! 从 Lua 的全局表中查找名为 name 的变量，并将其值压入栈顶。若变量不存在，则压入 nil
		if (0 != lua_getglobal(pLuaState, strLuaFuncName.c_str())) 
		{
			//! 检查当前全局变量是否是函数
			if (lua_isfunction(pLuaState,-1))
			{
				//! 将函数参数值压栈
				lua_pushinteger(pLuaState, nParam); 
				lua_pushstring(pLuaState,strParam1.c_str());
				//! 开始调用当前函数
				if (LUA_OK == lua_pcall(pLuaState, 2, 1,0))
				{
					if (lua_isstring(pLuaState,-1))
					{
						strResult = lua_tostring(pLuaState,-1);
					}
					lua_pop(pLuaState, 1);
					return true;
				}
				else
				{
					JGW::JGW_FormatWString(mstrErrorMessage,L"Error: %s\n", JGW_A2W_A(lua_tostring(pLuaState, -1)).c_str());
				}
			}
			else
			{
				JGW::JGW_FormatWString(mstrErrorMessage,L"global variable(%s) is not a function",JGW_A2W(strLuaFuncName).c_str());
			}
			lua_pop(pLuaState, 1);
		}
		else
		{
			JGW::JGW_FormatWString(mstrErrorMessage,L"global func(%s) does not exist",JGW_A2W(strLuaFuncName).c_str());
		}

		return false;
	}

	bool CCJGW_LuaHelp::ExecThreeIntStringStringParamReturnVoidLuaFunc(const std::string& strLuaFuncName,int nParam,const std::string& strParam1,const std::string& strParam2)
	{
		lua_State* pLuaState = (lua_State*)mpLuaState;
		//! 从 Lua 的全局表中查找名为 name 的变量，并将其值压入栈顶。若变量不存在，则压入 nil
		if (0 != lua_getglobal(pLuaState, strLuaFuncName.c_str())) 
		{
			//! 检查当前全局变量是否是函数
			if (lua_isfunction(pLuaState,-1))
			{
				//! 将函数参数值压栈
				lua_pushinteger(pLuaState, nParam); 
				lua_pushstring(pLuaState,strParam1.c_str());
				lua_pushstring(pLuaState,strParam2.c_str());
				//! 开始调用当前函数
				if (LUA_OK == lua_pcall(pLuaState, 2, 0,0))
				{
					return true;
				}
				else
				{
					JGW::JGW_FormatWString(mstrErrorMessage,L"Error: %s\n", JGW_A2W_A(lua_tostring(pLuaState, -1)).c_str());
				}
			}
			else
			{
				JGW::JGW_FormatWString(mstrErrorMessage,L"global variable(%s) is not a function",JGW_A2W(strLuaFuncName).c_str());
			}
		}
		else
		{
			JGW::JGW_FormatWString(mstrErrorMessage,L"global func(%s) does not exist",JGW_A2W(strLuaFuncName).c_str());
		}

		return false;
	}
	
	void CCJGW_LuaHelp::RegisterGlobalFunction(const std::string& strGlobalFuncName,int (*pGlobalFunc) (void *pluaState))
	{
		lua_State* pLuaState = (lua_State*)mpLuaState;
		lua_register(pLuaState,strGlobalFuncName.c_str(),(lua_CFunction)pGlobalFunc);
	}

}
