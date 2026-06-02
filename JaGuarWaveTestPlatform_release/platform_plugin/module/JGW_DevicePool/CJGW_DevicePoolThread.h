#pragma once

#include <map>
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JGW_DevicePool/DevicePortDefine.h>
#define AUTO_ID_USB_EXPORTS
#include "usb_config/CJGW_FilteUsbConfig.h"
#include <JGW_DevicePool/Auto_ID_Usb.h>

#include "../../../platform_include/JGW_DevicePoolModule_Impl.h"

namespace JGW
{
    class CCJGW_DevicePoolThread:public CCMessageThread ,public CCJGW_DevicePoolImpl
    {
    public:
        CCJGW_DevicePoolThread(void);
        ~CCJGW_DevicePoolThread(void);
        //! 添加注册线程消息
        bool RegDevicePoolMsg(S_REG_DEVICE_POOL_DEVINFO* psRegDevicePoolDevInfo);
        //! 添加注册线程消息 nMsgID 当前设备插入发送指定消息ID给指定的窗口句柄 
        //! guid注册设备GUID
        bool RegDevicePoolMsg(UINT nMsgID,HWND hwnd,GUID guid,E_DEVICE_MODE eDeviceModeEnum);
        //! 将响应到的设备插入消息传递给线程池
        bool SendDeviceMsgToDevicePool(SDeviceMsg_Info* lpDeviceMsg);
        //! 获取设备池线程ID
        unsigned int GetDevicePoolThreadID();
        //! 设备池线程是否正在运行
        bool IsStartDevicePoolThread();
        //! 关闭设备池线程
        void CloseDevicePoolThread();
        //! 开启设备池线程
        bool StartDevicePoolThread(HWND hWnd,const wchar_t* strConfigPath);

//         static CCJGW_DevicePoolThread* GetDevicePoolInstance()
//         {
//             static CCJGW_DevicePoolThread dev;
//             return &dev;
//         }
    private:
        //! 缓存USB信息
       // bool 
        //! 响应设备状态插拔消息
        void OnRspDeviceStatusChangeMsg(WPARAM wParam,LPARAM lParam);
        //! 清除排序索引
        void OnClearSortIndex(WPARAM wParam,LPARAM lParam);
        //! 移除注册的设备池消息
        void OnRemoveRegDevicePoolDevInfo(WPARAM wParam,LPARAM lParam);
        //! 响应注册设备信息
        void OnRspRegDevicePoolDevInfo(WPARAM wParam,LPARAM lParam);
        //!
        void OnRemovePluginRegDevicePoolDevInfo(WPARAM wParam,LPARAM lParam);
        //! 注册GUID消息通知
        HDEVNOTIFY RegisterNotification(GUID guid);
        //! 
        bool UnregisterDevNotification(HDEVNOTIFY hDevNotify);
        DECLARE_MYTHREAD_MESSAGE_MAP();
    private:
        void DispatchDeviceMsg(SDeviceMsg_Info* lpdevice);

        void RunThread();

        int GetSortIndex(const wchar_t* strInstanceID);

        int GetSortIndexAndSerialNumber(const wchar_t* strInstanceID,std::wstring& strSerialNumber);
    private:
        HWND mhMainHwnd;

        CCJGW_FilteUsbConfig mfilteUsbConfig;
        //! 设备插入PID VID集合
        std::vector<SDeviceMsg_Info*> m_vSDeviceMsg;
        //! 注册设备池信息
        std::map<GUID_KEY,S_GUID_DEVICE_POOL_INFO> mmapGuidDevPoolInfo;
        //! 注册配置信息
        std::map<GUID_KEY,S_GUID_USB_CONFIG> mmapsGuidUsbConfig;
        //!KEY DBCC_NAME 缓存device信息
        std::map<std::wstring,SDeviceMsg_Info*> mmapCacheDeviceInfo;
    };
}


