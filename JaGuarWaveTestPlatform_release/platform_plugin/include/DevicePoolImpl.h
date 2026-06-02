#pragma once
#include "DevicePortDefine.h"
#include <JGW_DevicePool/Auto_ID_Usb.h>

namespace JGW
{
	class JGW_DevicePoolImpl
	{
	public:
		//! 开启设备池 hWnd 设置响应设备插入消息的窗口句柄 
        virtual bool    StartDevicePoolThread(HWND hWnd,const wchar_t* strConfigPath) = 0;
		//! virtual bool	StartDevicePool(HWND hWnd) = 0;
		//! 添加注册线程消息
		virtual bool	RegDevicePoolMsg(S_REG_DEVICE_POOL_DEVINFO* psRegDevicePoolDevInfo) = 0;
		//! 添加注册线程消息 nMsgID 当前设备插入发送指定消息ID给指定的窗口句柄 
		//! guid注册设备GUID
		virtual bool	RegDevicePoolMsg(UINT nMsgID,HWND hwnd,GUID guid,E_DEVICE_MODE eDeviceModeEnum) = 0;
		//! 将响应到的设备插入消息传递给线程池
		virtual bool	SendDeviceMsgToDevicePool(SDeviceMsg_Info* lpDeviceMsg) = 0;
		//! 获取设备池线程ID
		virtual unsigned int GetDevicePoolThreadID() = 0;
		//! 设备池线程是否运行
		virtual bool	IsStartDevicePoolThread() = 0;
		//! 关闭设备池线程
		virtual void	CloseDevicePoolThread() = 0;
	};
}


#ifdef __cplusplus
extern "C" {
#endif

	AUTO_ID_USB_API JGW::JGW_DevicePoolImpl* GetDevicePoolImplInstance();

#ifdef __cplusplus
}

#ifndef  JGW_DEVICEPOOL_EXPORTS
#ifdef _DEBUG
#pragma comment(lib,"JGW_DevicePool_d.lib")
#else
#pragma comment(lib,"JGW_DevicePool.lib")
#endif

#endif

#endif