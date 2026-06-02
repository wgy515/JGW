#pragma once
#include <WinDef.h>

class IWPFWin32WindowInterface
{
public:    
    /// <summary>
    /// 加载WPF - WIN32的窗口
    /// </summary>
    /// <param name="mainHwnd"> WPF 窗口句柄</param>
    /// <returns></returns>
    virtual HWND LoadWin32Window(HWND hMainWnd,DWORD dwUsbDevicePoolThreadID) = 0;
    /// <summary>
    /// 更新WIN32窗口的背景颜色
    /// </summary>
    /// <param name="color">颜色 #FFCFECF8</param>
    virtual void UpdateWin32WindowBackgroundColor(const wchar_t* color) = 0;
    /// <summary>
    /// 更新MES信息到WIN32窗口DLL
    /// </summary>
    /// <param name="mesOnline">是否是在线模式</param>
    /// <param name="taskNumber">订单号</param>
    /// <param name="workOrderNumber">工单号</param>
    /// <param name="userName">MES用户名</param>
    /// <param name="password">MES密码</param>
    /// <param name="shiftID">0: 白班 1 : 晚班</param>
    virtual void UpdateMESInfo(bool mesOnline,const wchar_t* taskNumber, const wchar_t* workOrderNumber, const wchar_t* userName, const wchar_t* password, int shiftID) = 0;
    /// <summary>
    /// 更新需要加载插件信息到WIN32窗口DLL
    /// </summary>
    /// <param name="stationID">MES站点ID</param>
    /// <param name="clsid">导出类名</param>
    /// <param name="viewloader_id">视图加载器ID</param>
    /// <param name="moduleFolder">测试配置文件目录</param>
    /// <param name="pluginName">插件名称</param>
    /// <param name="station_name">站点名称</param>
    /// <param name="image_id">测试项目图像ID</param>
    virtual void UpdatePluginInfo(const wchar_t* stationID,const wchar_t* clsid,int viewloader_id,const wchar_t* moduleFolder,const wchar_t* pluginName,const wchar_t* station_name,int image_id) = 0;
    /// <summary>
    /// 显示LOG信息控件
    /// </summary>
    virtual void ShowLogControl() = 0;
    /// <summary>
    /// 显示LOG对话框
    /// </summary>
    virtual void ShowLogDialog() = 0;
    /// <summary>
    /// 是否开启远程socket log
    /// </summary>
    /// <param name="bOpen"></param>
    virtual void StartRemoteLogSocket(bool bOpen) = 0;
    /// <summary>
    /// 关闭WIN32窗口（WIN32窗口发送COLSE命令）
    /// </summary>
    virtual void CloseWin32Window() = 0;
    /// <summary>
    /// 卸载WPF -> WIN32窗口
    /// </summary>
    virtual void UnLoadWin32Window() = 0;
};