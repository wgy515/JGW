#pragma once
#include <string>

typedef void (_cdecl *LogCallBackDelegate)(int type, int id, const wchar_t* log);

#ifdef __cplusplus
extern "C" 
{
#endif
    /// <summary>
    /// 加载TSE模块
    /// </summary>
    /// <param name="moduleName">模块DLL名称</param>
    /// <returns></returns>
    __declspec(dllexport) bool LoadTSEModule(const wchar_t* moduleName);
    /// <summary>
    /// 创建TSE测试类实例
    /// </summary>
    /// <param name="realName">TSE测试类名</param>
    /// <param name="moduleName">TSE模块DLL名称</param>
    /// <returns>返回实例指针</returns>
    __declspec(dllexport) void* CreateTSEInstance(const wchar_t* realName, const wchar_t* moduleName);
    /// <summary>
    /// 跟新配置参数
    /// </summary>
    /// <param name="instance"></param>
    /// <returns></returns>
    /// [DllImport("TSE_SequenceManagerPlugin_NET.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    __declspec(dllexport) bool UpdateConfigParamToTSEInstance(void* instance, const wchar_t* paramName, const wchar_t* paramValue);
    /// <summary>
    /// 初始化当前测试
    /// </summary>
    /// <param name="instance"></param>
    /// <returns></returns>
    /// [DllImport("TSE_SequenceManagerPlugin_NET.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    __declspec(dllexport) bool InitTestToTSEInstance(void* instance);
    /// <summary>
    /// 执行TSE测试
    /// </summary>
    /// <param name="instance"></param>
    /// <returns>true or false</returns>
    /// [DllImport("TSE_SequenceManagerPlugin_NET.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    __declspec(dllexport) bool ExecuteTSEInstance(void* instance);
    /// <summary>
    /// 退出当前测试项目
    /// </summary>
    /// <param name="instance"></param>
    /// <returns></returns>
    /// [DllImport("TSE_SequenceManagerPlugin_NET.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    __declspec(dllexport) bool ExitTestToTSEInstance(void* instance);
    /// <summary>
    /// 获取全局环境变量
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    /// [DllImport("TSE_SequenceManagerPlugin_NET.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    __declspec(dllexport) int GetGlobalEnvironment(const wchar_t* key,wchar_t* val);
    ///// <summary>
    ///// 设置全局环境变量
    ///// </summary>
    ///// <param name="key"></param>
    ///// <param name="val"></param>
    /// [DllImport("TSE_SequenceManagerPlugin_NET.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    __declspec(dllexport) void SetGlobalEnvironment(const wchar_t* key, const wchar_t* val);
    /// <summary>
    /// 注册log回调函数
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    /// [DllImport("TSE_SequenceManagerPlugin_NET.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    __declspec(dllexport) void RegistLogCallBackFunc(LogCallBackDelegate logCallBack);
    /// <summary>
    /// 卸载TSE模块
    /// </summary>
    /// <param name="moduleName"></param>
    /// <returns></returns>
    /// [DllImport("TSE_SequenceManagerPlugin_NET.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    __declspec(dllexport) void UnLoadTSEModule(const wchar_t* moduleName);
	/// <summary>
	/// 加载MES CONFIG
	/// </summary>
	/// <param name="mesConfigPath"></param>
	/// <returns></returns>
	/// [DllImport("TSE_SequenceManagerPlugin_NET.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    __declspec(dllexport) bool LoadTSEMESConfig(const wchar_t* mesConfigPath);
	/// <summary>
	/// 更新操作ID
	/// </summary>
	/// <param name="mesConfigPath"></param>
	/// <returns></returns>
	/// [DllImport("TSE_SequenceManagerPlugin_NET.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
	__declspec(dllexport) void UpdateTSEMESOperation(const wchar_t* operationID);
	/// <summary>
	/// 更新用户名和密码
	/// </summary>
	/// <param name="mesConfigPath"></param>
	/// <returns></returns>
	/// [DllImport("TSE_SequenceManagerPlugin_NET.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
	__declspec(dllexport) void UpdateTSEMESUserPassword(const wchar_t* userName,const wchar_t* password);
	/// <summary>
	/// 加载HMTL解析文件配置
	/// </summary>
	/// <param name="mesConfigPath"></param>
	/// <returns></returns>
	/// [DllImport("TSE_SequenceManagerPlugin_NET.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
	__declspec(dllexport) bool LoadHtmlResolveRuleFilePath(const wchar_t* htmlResolveRuleFilePath);
	/// <summary>
	/// 解析HTML文件信息
	/// </summary>
	/// <param name="showTestLogType">0:不显示PASS和FAIL LOG,1:TSET FAIL 显示FAIL LOG 但不显示PASS LOG,2:TSET FAIL 显示所有LOG,3:TSET PASS 显示所有LOG</param>
	/// <returns></returns>
	/// [DllImport("TSE_SequenceManagerPlugin_NET.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
	__declspec(dllexport) const wchar_t* ResolveHtmlFileInfo(const wchar_t* htmlFilePath,int showTestLogType,bool saveTestLogging,const wchar_t* sn,bool testResult); 

#ifdef __cplusplus
};
#endif