#pragma once

#ifdef __cplusplus
extern "C" 
{
#endif
    //! 测试log回调函数
    typedef void (*TSETestLogFunc)(int type,int id,const wchar_t* log);
    __declspec(dllexport) void SetTSETestLogCallBack(TSETestLogFunc testLogCallBack);

    //! 测试状态更新回调函数
    typedef void (*TSETestStatusChangeFunc)(int testStatus,const wchar_t* testName);
    __declspec(dllexport) void SetTSETestStatusCallBack(TSETestStatusChangeFunc testStatusCallBack);

    //! 初始化DLL实例
    __declspec(dllexport) bool InitInteropManager();
    //! release DLL 实例
    __declspec(dllexport) void ReleaseInteropManager();

    //！需要加载的TSE模块名称
    __declspec(dllexport) bool LoadTSEModuleToModuleName(const wchar_t* modulePath);
    //! 卸载已经加载TSE测试模块DLL
    __declspec(dllexport) void UnLoadTSEModule();

    //! 更新错误重测次数
    //! __declspec(dllexport) void UpdateErrorRetryTestCount(int errorRetryTestCount);
    //! 获取suite实例 根据模块名
    __declspec(dllexport) void* GetSequenceTestInstanceToRealName(const wchar_t* realName,const wchar_t* modulePath);
    //! 更新suite实例配置参数
    __declspec(dllexport) void UpdateSequenceTestConfigParamToInstance(void* pSeqTestInstance,const wchar_t* paramName,const wchar_t* paramVal);
    //! 执行当前suite测试项目
    __declspec(dllexport) bool ExecuteSequenceTestToInstance(void* pSeqTestInstance,bool isReversedResult);
    //! 释放当前suite测试项目指针
    __declspec(dllexport) void ReleaseSequenceTestInstance(void* pSeqTestInstance);

    //! 更新 GlobalVariable 
    __declspec(dllexport) void UpdateTSEGlobalVariable(const wchar_t* environmentName,const wchar_t* environmentValue);
    //! get GlobalVariable
    __declspec(dllexport) const wchar_t* GetTSEGlobalVariableValue(const wchar_t* environmentName);
    //! clear GlobalVariable
    __declspec(dllexport) void ClearTSEGlobalVariable();



    //! 更新suite xml config文件路径
    __declspec(dllexport) bool UpdateSuiteXmlConfigFilePath(const wchar_t* suiteXmlConfigFilePath);
#ifdef __cplusplus
};
#endif