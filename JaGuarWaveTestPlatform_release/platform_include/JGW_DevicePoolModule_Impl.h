#pragma once

namespace JGW
{
    class CCJGW_DevicePoolImpl
    {
    public:
        virtual ~CCJGW_DevicePoolImpl(){}
        //! 开启设备池线程
        virtual bool StartDevicePoolThread(HWND hWnd,const wchar_t* strConfigPath) = 0;
        //! 线程池是否已经开启
        virtual bool IsStartDevicePoolThread() = 0;
        //! 获取线程池ID
        virtual size_t GetDevicePoolThreadID() = 0;
        //! 关闭设备池线程
        virtual void CloseDevicePoolThread() = 0;

        //! 添加注册线程消息
        virtual bool RegDevicePoolMsg(S_REG_DEVICE_POOL_DEVINFO* psRegDevicePoolDevInfo) = 0;
        //! 添加注册线程消息 nMsgID 当前设备插入发送指定消息ID给指定的窗口句柄 
        //! guid注册设备GUID
        virtual bool RegDevicePoolMsg(UINT nMsgID,HWND hwnd,GUID guid,E_DEVICE_MODE eDeviceModeEnum) = 0;
        //! 将响应到的设备插入消息传递给线程池
        virtual bool SendDeviceMsgToDevicePool(SDeviceMsg_Info* lpDeviceMsg) = 0;
    };
}