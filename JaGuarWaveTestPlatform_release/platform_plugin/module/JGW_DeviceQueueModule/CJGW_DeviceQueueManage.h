#pragma once
#include <JGW_DeviceQueueModule/CJGW_DeviceQueueImpl.h>
#include <map>
#include "CJGW_DeviceQueueSortConfig.h"
#include "CJGW_DeviceSortConfigImpl.h"
namespace JGW
{
    class CCJGW_DeviceQueueManage
    {
    public:
        CCJGW_DeviceQueueManage(void);
        ~CCJGW_DeviceQueueManage(void);
    public:
        // 初始化设备排序模块
        bool Initialization(HWND hMainWnd,const std::wstring& strModuleFolder,const S_DEVICEQUEUE_PROGRAM_INFO& sDeviceQueueProgramInfo);
        //  更新设备排序信息配置
        void ChangeDeviceQueueProgramInfo(const S_DEVICEQUEUE_PROGRAM_INFO& sDeviceQueueProgramInfo);
        //! 
        void AddListenerDeviceQueueMsgEvent(OnDeviceQueueMsg pOnDeviceQueueMsg,LPVOID pContext);
        //!
        void RemoveListenerDeviceQueueMsgEvent(OnDeviceQueueMsg pOnDeviceQueueMsg);
        // 接收到设备消息
        void OnHandleDeviceMsg(E_DEVICE_MSG_TYPE eMsgType,const WPARAM& wParam,const LPARAM& lParam);
        // 退出初始化设备排序模块
        void UnInitialization();
        //
        bool OnResponseClickConfigMsg(HWND hWindow,const S_DEVICEQUEUE_PROGRAM_INFO& sDevoceQueueProgramInfo,std::wstring& strTFTPD32IPAddr,std::wstring& strDownloadFileConfig);

        void ClearDeviceQueueSortConfig();
    private:
        void StartNetCardDevice();
        void StopNetCardDevice();
        void OnDeviceQueueMsgEvent(LPS_DEVICE_QUEUE_INFO psDeviceQueueInfo);
        E_DEVICE_MODE GetDeviceModeToHandleDeviceMsg(E_DEVICE_MSG_TYPE eMsgType,const WPARAM& wParam,std::wstring& strDevice,bool& bIsPlugInDevice,int& nSortIndex,std::wstring& strDes);
    private:
        bool mbInitNetCardDevice;
        HWND mhMianWnd;
        CCJGW_DeviceSortConfigImpl* mpDeviceSortConfigImpl;
        //! 设备排序信息
        S_DEVICE_QUEUE_INFO msDeviceQueueInfo;
        //! 设备排序方案配置信息
        S_DEVICEQUEUE_PROGRAM_INFO msDeviceQueueProgramInfo;
        //! 设备排序事件
        std::map<OnDeviceQueueMsg,LPVOID> mmapDeviceQueueMsgEvent;
        //! 设备排序配置信息
        CCJGW_DeviceQueueSortConfig mcDeviceQueueSortConfig;
    };
}

