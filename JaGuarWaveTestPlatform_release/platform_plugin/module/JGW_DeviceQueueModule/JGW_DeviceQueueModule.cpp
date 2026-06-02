// JGW_DeviceQueueModule.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <JGW_DeviceQueueModule/CJGW_DeviceQueueImpl.h>
#include "CJGW_DeviceQueueManage.h"

/*
1.窗口将接收到的设备信息通过HandleDeviceQueueMsg传递给设备排序模块
2.InitializationDeviceQueueModule 根据排序方案，如果有网卡排序则初始化网卡设备监听模块并将网卡设备信心转发到窗口消息，然后窗口消息在回调HandleDeviceQueueMsg传递给设备排序模块，并加载已经排序记录
3.SettingDeviceQueueProgram修改排序方案以及排序记录
4、AddChangeDeviceQueueMsgEvent 添加排序信息回调函数
5、RemoveChangeDeviceQueueMsgEvent 移除排序信息回调函数
6、OnResponseClickConfigMsg 响应客户点击配置消息

排序的配置文件
[MAIN]
DeviceQueueProgram=0
FristDeviceMode = 0
SecondDeviceMode = 0
ThreeDeviceMode = 0
DevCounts=0

[DEV_1]
FristDevice=
FristDeviceSortIndex=
SecondDevice
SecondDeviceSortIndex=
ThreeDevice
ThreeDeviceSortIndex=

[DEV_2]
FristDevice=
FristDeviceSortIndex=
SecondDevice
SecondDeviceSortIndex=
ThreeDevice
ThreeDeviceSortIndex=

[DEV_3]
FristDevice=
FristDeviceSortIndex=
SecondDevice
SecondDeviceSortIndex=
ThreeDevice
ThreeDeviceSortIndex=


*/
//! 1、根据排序方案加载配置文件排序信息以及初始化网卡
//! 2、

JGW::CCJGW_DeviceQueueManage gDeviceQueueManage;

bool InitializationDeviceQueueModule(HWND hMainWnd,const std::wstring& strModuleFolder,const S_DEVICEQUEUE_PROGRAM_INFO& sDeviceQueueProgram)
{
    return gDeviceQueueManage.Initialization(hMainWnd,strModuleFolder,sDeviceQueueProgram);
}

bool SettingDeviceQueueProgram(const S_DEVICEQUEUE_PROGRAM_INFO& sDeviceQueueProgram)
{
    gDeviceQueueManage.ChangeDeviceQueueProgramInfo(sDeviceQueueProgram);
    return true;
}

void AddChangeDeviceQueueMsgEvent(OnDeviceQueueMsg pOnDeviceQueueMsg,LPVOID pContext)
{
    gDeviceQueueManage.AddListenerDeviceQueueMsgEvent(pOnDeviceQueueMsg,pContext);
}

void RemoveChangeDeviceQueueMsgEvent(OnDeviceQueueMsg pOnDeviceQueueMsg)
{
    gDeviceQueueManage.RemoveListenerDeviceQueueMsgEvent(pOnDeviceQueueMsg);
}

bool OnResponseClickConfigMsg(HWND hWindow,const S_DEVICEQUEUE_PROGRAM_INFO& sDevoceQueueProgramInfo,std::wstring& strTFTPD32IPAddr,std::wstring& strDownloadFileConfig)
{
    //! 单端口排序则不能进行端口配置
    if (sDevoceQueueProgramInfo.meDeviceQueueProgram == E_SINGLE_PORT_QUEUE_PROGRAM || sDevoceQueueProgramInfo.meDeviceQueueProgram == E_NETCARD_QUEUE_PROGRAM)
    {
        MessageBox(hWindow,L"单端口不需要进行端口配置",L"无法配置",MB_OK);
        return false;
    }
    return gDeviceQueueManage.OnResponseClickConfigMsg(hWindow,sDevoceQueueProgramInfo,strTFTPD32IPAddr,strDownloadFileConfig);
}

void HandleDeviceQueueMsg(E_DEVICE_MSG_TYPE eMsgType,const WPARAM& wParam,const LPARAM& lParam)
{
    gDeviceQueueManage.OnHandleDeviceMsg(eMsgType,wParam,lParam);
}

void UnInitializationDeviceQueueModule()
{
    gDeviceQueueManage.UnInitialization();
}

void ClearDeviceQueueSortConfig()
{
    gDeviceQueueManage.ClearDeviceQueueSortConfig();
}
