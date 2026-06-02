#pragma once
#include <JGW_DeviceQueueModule/CJGW_DeviceQueueImpl.h>

namespace JGW
{
    class CCJGW_DeviceSortConfigImpl
    {
    public:
        virtual void OnHandleDeviceMsg(E_DEVICE_MSG_TYPE eMsgType,const WPARAM& wParam,const LPARAM& lParam) = 0;
    };
}


