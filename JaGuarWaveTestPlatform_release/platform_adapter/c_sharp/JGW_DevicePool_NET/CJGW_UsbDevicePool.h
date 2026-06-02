#pragma once
#include <UiLib/UIlib.h>
#include <JaGuarWaveTestPlatform/CJGW_PocoClassLoader.h>
#include <JGW_DevicePool/DevicePortDefine.h>
#include "../../../platform_include/JGW_DevicePoolModule_Impl.h"

namespace JGW
{
    class CCJGW_UsbDevicePool
    {
    public:
        CCJGW_UsbDevicePool(void);
        ~CCJGW_UsbDevicePool(void);
    public:
        // 开启USB设备池
        bool StartUsbDevicePool(HWND hMainWnd);
        // 获取USB线程ID
        size_t GetUsbThreadPoolID();
        // 停止USB设备池
        void StopUsbDevicePool();
        //
        //void RegUsbDevicePoolMsg(UINT nMsgID,HWND hwnd,GUID guid,short eDeviceModeEnum);
        // 响应device msg
        void OnDeviceChange(WPARAM wParam,LPARAM lParam);
    private:
        SDeviceMsg_Info* GetDeviceMsgInfoPtr();
    private:
        size_t mdwThreadID;
        //! USB DEVICE 线程池
        CCJGW_DevicePoolImpl*		    m_lpDevicePoolImpl;
        CCJGW_PocoClassLoader<CCJGW_DevicePoolImpl> m_cLoadDevicePoolSubPlugin;
        //! 已经使用过得DEVICE MSG消息
        std::vector<SDeviceMsg_Info*> m_vSDeviceMsg;
    };
}

