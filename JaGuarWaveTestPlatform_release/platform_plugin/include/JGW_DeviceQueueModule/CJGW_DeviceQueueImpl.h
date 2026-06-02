#pragma once
#ifndef JGW_DEVICE_QUEUE_IMPL_H__
#define JGW_DEVICE_QUEUE_IMPL_H__
#include <string>
#include <JGW_DevicePool/DevicePortDefine.h>

#ifdef JGW_DEVICEQUEUEMODULE_EXPORTS
#define DEVICE_QUEUE_MODULE_API __declspec(dllexport)
#else
#define DEVICE_QUEUE_MODULE_API __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib,"JGW_DeviceQueueModule_d.lib")
#else
#pragma comment(lib,"JGW_DeviceQueueModule.lib")
#endif
#endif

typedef struct  
{
    // 插入或者移除设备
    bool mbIsPlugInDevice;
    // 视图索引
    int mnViewIndex;
    std::wstring mstrFristDevice;
    std::wstring mstrSecondDevice;
    std::wstring mstrThreeDevice;
    std::wstring mstrDes;
}S_DEVICE_QUEUE_INFO,*LPS_DEVICE_QUEUE_INFO;

enum E_DEVICE_QUEUE_PROGRAM
{
    // 单端口排序方案
    E_SINGLE_PORT_QUEUE_PROGRAM = 0,
    // 双端口排序方案
    E_DUAL_PORT_QUEUE_PROGRAM = 1,
    // 双端口排序+网口IP自动叠加
    E_DUAL_PORT_NETCARD_QUEUE_PROGRAM = 2,
    // 单网口排序方案
    E_NETCARD_QUEUE_PROGRAM
};

typedef struct  
{
    int mnFristDeviceSortIndex;
    int mnSecondDeviceSortIndex;
    int mnThreeDeviceSortIndex;
    std::wstring mstrFristDevice;
    std::wstring mstrSecondDevice;
    std::wstring mstrThreeDevice;
}S_DEVICESORT_CONFIG;

typedef struct
{
    // 双COM端口排序，双端口排序+网口IP自动叠加，单COM端口排序，网口排序）
    E_DEVICE_QUEUE_PROGRAM meDeviceQueueProgram;
    // 第一个端口设备模式
    JGW::E_DEVICE_MODE meFristDeviceMode;
    // 第二个端口设备模式
    JGW::E_DEVICE_MODE meSecondDeviceMode;
    // 第三个端口设备模式
    JGW::E_DEVICE_MODE meThreeDeviceMode;
    // 网卡主机IP地址段
    std::wstring mstrNetCardHostIPSeg;
} S_DEVICEQUEUE_PROGRAM_INFO;

// typedef void (*win_thread_func_t)(void*  arg);
typedef void (*OnDeviceQueueMsg)(LPS_DEVICE_QUEUE_INFO psDeviceQueueInfo,LPVOID pContext);

typedef enum
{
    E_COM_MSG_TYPE = 0,
    E_NETCARD_MSG_TYPE = 1
}E_DEVICE_MSG_TYPE;

#ifdef __cplusplus
extern "C" {
#endif
    // 初始化设备排序模块
    //DEVICE_QUEUE_MODULE_API bool InitializationDeviceQueueModule(HWND hMainWnd,const std::wstring& strDeviceQueueConfigPath,const std::wstring& strModuleFolder,E_DEVICE_QUEUE_PROGRAM eDeviceQueueProgram);
    DEVICE_QUEUE_MODULE_API bool InitializationDeviceQueueModule(HWND hMainWnd,const std::wstring& strModuleFolder,const S_DEVICEQUEUE_PROGRAM_INFO& sDeviceQueueProgram);
    // 设置排序方案
    DEVICE_QUEUE_MODULE_API bool SettingDeviceQueueProgram(const S_DEVICEQUEUE_PROGRAM_INFO& sDeviceQueueProgram);
    // 添加响应设备排序消息事件
    DEVICE_QUEUE_MODULE_API void AddChangeDeviceQueueMsgEvent(OnDeviceQueueMsg pOnDeviceQueueMsg,LPVOID pContext);
    // 移除响应设备排序消息事件
    DEVICE_QUEUE_MODULE_API void RemoveChangeDeviceQueueMsgEvent(OnDeviceQueueMsg pOnDeviceQueueMsg);
    // 点击配置按钮
    DEVICE_QUEUE_MODULE_API bool OnResponseClickConfigMsg(HWND hWindow,const S_DEVICEQUEUE_PROGRAM_INFO& sDevoceQueueProgramInfo,std::wstring& strTFTPD32IPAddr,std::wstring& strDownloadFileConfig);
    // 窗口监听的设备消息传递给设备排序模块 const WPARAM& wParam,const LPARAM& lParam
    DEVICE_QUEUE_MODULE_API void HandleDeviceQueueMsg(E_DEVICE_MSG_TYPE eMsgType,const WPARAM& wParam,const LPARAM& lParam);
    DEVICE_QUEUE_MODULE_API void ClearDeviceQueueSortConfig();
    //!
    DEVICE_QUEUE_MODULE_API void UnInitializationDeviceQueueModule();
#ifdef __cplusplus
}
#endif


#endif
