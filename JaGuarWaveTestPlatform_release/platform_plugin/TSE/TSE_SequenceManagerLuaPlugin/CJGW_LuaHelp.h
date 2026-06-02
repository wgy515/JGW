#pragma once
#include <string>

namespace JGW
{
	class CCJGW_LuaHelp
	{
	public:
		CCJGW_LuaHelp(void);
		~CCJGW_LuaHelp(void);
	public:
		//! 初始化LUA 环境，创建LUA状态机以及打开标准库
		bool InitLuaEnvironment();
		//! 加载LUA文件
		bool LoadLuaFile(const std::string& strLuaFilePath);
		//! 卸载LUA环境
		void UnInitLuaEnvironment();
	public:
		//! 设置LUA全局变量
		void SetLuaGlobalVariableString(const std::string& strGlobalVariableName,const std::string& strGlobalVariableValue);
		//! 设置LUA全局变量
		void SetLuaGlobalVariableInteger(const std::string& strGlobalVariableName,long long llGlobalVariableValue);
		//! 设置LUA全局变量
		void SetLuaGlobalVariableDouble(const std::string& strGlobalVariableName,double dfGlobalVariableValue);
		//! 获取LUA指定全局变量名称对应的值
		std::string GetLuaGlobalVariableString(const std::string& strGlobalVariableName,const std::string& strDefaultValue = "");
		//! 获取LUA指定全局变量名称对应的值
		long long GetLuaGlobalVariableInteger(const std::string& strGlobalVariableName,long long llDefaultValue);
		//! 获取LUA指定全局变量名称对应的值
		double GetLuaGlobalVariableDouble(const std::string& strGlobalVariableName,double dfDefaultValue);
	public:
		//! nargs: 要传递给Lua函数的参数个数。nresults: 期望从Lua函数获取的结果个数。
		bool ExecSingleIntParamReturnBoolLuaFunc(const std::string& strLuaFuncName,int nParam,bool& bResult);
		//! nargs: 要传递给Lua函数的参数个数。nresults: 期望从Lua函数获取的结果个数。
		bool ExecDoubleIntStringParamReturnStringLuaFunc(const std::string& strLuaFuncName,int nParam,const std::string& strParam1,std::string& strResult);
		//! nargs: 要传递给Lua函数的参数个数。nresults: 期望从Lua函数获取的结果个数。
		bool ExecThreeIntStringStringParamReturnVoidLuaFunc(const std::string& strLuaFuncName,int nParam,const std::string& strParam1,const std::string& strParam2);
		//! 注册函数 lua_register(L,"TestRegGlobalFunc",lua_TestRegGlobalFunc);
		void RegisterGlobalFunction(const std::string& strGlobalFuncName,int (*pGlobalFunc) (void *pluaState));
	public:
		//!
		const std::wstring& GetErrorMessage();
		void* GetLuaState();
	private:
		void* mpLuaState;
		std::wstring mstrErrorMessage;
	};
}


