// JGW_DevicePool_NET.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "CJGW_UsbDevicePool.h"
#include "JGW_DevicePool_NET.h"
#include <JGW_FoundationFunc/JGW_ThreadFunc.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

struct SDevicePoolDevInfo
{
public:
    /// <summary>
    /// E_DEVICE_MODE 设备模式
    /// </summary>
    short DeviceMode;
    /// <summary>
    /// 排序索引
    /// </summary>
    int SortIndex;
    /// <summary>
    /// com 端口
    /// </summary>
    int ComPort;
    /// <summary>
    /// DBT_DEVICEARRIVAL 插入 还是拔出  
    /// </summary>
    size_t DeviceMsgType;
    /// <summary>
    /// 消息GUID
    /// </summary>
    GUID ClassGuid;
    /// <summary>
    /// \\?\USB#Vid_05c6&Pid_9091&MI_00#7&30607750&1&0000#{86e0d1e0-8089-11d0-9ce4-08003e301f73}
    /// </summary>
    char Dbcc_name[256];
    /// <summary>
    /// USB\Vid_05c6&Pid_9008\6&7c2d8f9&0&5
    /// </summary>
    char UsbPidVid[256];
    /// <summary>
    /// adb 或者 fastboot SerialNumber
    /// </summary>
    char SeralNumber[64];
};

JGW::CCJGW_UsbDevicePool* gpUsbDevicePool = NULL;
SDevicePoolDevInfo gsSDevicePoolDevInfo;

size_t StartUsbDevicePool(HWND hMainWnd)
{
    StopUsbDevicePool();
    gpUsbDevicePool = new JGW::CCJGW_UsbDevicePool();

    return gpUsbDevicePool->StartUsbDevicePool(hMainWnd)?gpUsbDevicePool->GetUsbThreadPoolID():0;
}

void OnDeviceChange(WPARAM wParam,LPARAM lParam)
{
    if (gpUsbDevicePool)
    {
        gpUsbDevicePool->OnDeviceChange(wParam,lParam);
    }
}

void RegUsbDevicePoolMsg(UINT dwThreadID,UINT nMsgID,HWND hwnd,GUID guid,short eDeviceModeEnum)
{
    REG_DEVICEPOOL_MSG(dwThreadID,nMsgID,hwnd,guid,eDeviceModeEnum);
}

void StopUsbDevicePool()
{
    if (gpUsbDevicePool)
    {
        gpUsbDevicePool->StopUsbDevicePool();
        delete gpUsbDevicePool;
        gpUsbDevicePool = NULL;
    }
}




void* GetDevicePoolDevInfoToWParam(WPARAM wParam,LPARAM lParam)
{
    if (!wParam) return NULL;
    JGW::SDeviceMsg_Info* lpdevicemsg = (JGW::SDeviceMsg_Info*)wParam;
    //SDevicePoolDevInfo* psSDevicePoolDevInfo = (SDevicePoolDevInfo*)psSDevicePoolDevInfo;

    gsSDevicePoolDevInfo.ClassGuid = lpdevicemsg->m_guid;
    gsSDevicePoolDevInfo.DeviceMode = lpdevicemsg->m_nDeviceMode;
    gsSDevicePoolDevInfo.ComPort = lpdevicemsg->m_nComPort;
    //psSDevicePoolDevInfo->Dbcc_name = 
    memset(gsSDevicePoolDevInfo.Dbcc_name,0x00,256);
    strcpy_s(gsSDevicePoolDevInfo.Dbcc_name,255,JGW::JGW_W2A(lpdevicemsg->m_strDbcc_name).c_str());

    memset(gsSDevicePoolDevInfo.UsbPidVid,0x00,256);
    strcpy_s(gsSDevicePoolDevInfo.UsbPidVid,255,JGW::JGW_W2A(lpdevicemsg->m_strRegUsbPidVid).c_str());

    memset(gsSDevicePoolDevInfo.SeralNumber,0x00,64);
    strcpy_s(gsSDevicePoolDevInfo.SeralNumber,63,JGW::JGW_W2A(lpdevicemsg->m_strSeralNumber).c_str());

    gsSDevicePoolDevInfo.DeviceMsgType = lpdevicemsg->m_nDeviceMsgType;
    gsSDevicePoolDevInfo.SortIndex = lpdevicemsg->m_nSortIndex;

    lpdevicemsg->Dec();

    return &gsSDevicePoolDevInfo;
}

