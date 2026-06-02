#pragma once
#include <string>
#ifdef __cplusplus
extern "C" 
{
#endif
    //
    __declspec(dllexport) HWND LoadWin32Window(HWND hMainWnd,DWORD dwUsbDevicePoolThreadID);
    //
    __declspec(dllexport) void UpdateWin32WindowBackgroundColor(const wchar_t* color);
    //
    __declspec(dllexport) void UpdateMESInfo(bool mesOnline,const wchar_t* taskNumber, const wchar_t* workOrderNumber, const wchar_t* userName, const wchar_t* password, int shiftID);
    //! stationID MES站点ID 
    //! clsid：DLL clsid
    //! viewloader_id: 视图加载ID
    //! moduleFolder 配置文件目录
    //！pluginName: 插件名称
    //! station_name: 测试项目名称
    //！image_id : 测试项目图像ID
    __declspec(dllexport) void UpdatePluginInfo(const wchar_t* stationID,const wchar_t* clsid,int viewloader_id,const wchar_t* moduleFolder,const wchar_t* pluginName,const wchar_t* station_name,int image_id);
    //
    __declspec(dllexport) void ShowLogControl();
    //
    __declspec(dllexport) void ShowLogDialog();
    //
    __declspec(dllexport) void StartRemoteLogSocket(bool bOpen);
    //
    __declspec(dllexport) void CloseWin32Window();
    //
    __declspec(dllexport) void UnLoadWin32Window();
#ifdef __cplusplus
};
#endif