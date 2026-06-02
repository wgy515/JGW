#pragma once
#include <JGW_DevicePool/DevicePortDefine.h>

namespace JGW
{
    class CCJGW_FilteUsbConfig
    {
    public:
        CCJGW_FilteUsbConfig(void);
        ~CCJGW_FilteUsbConfig(void);

        bool FilteUsbConfigInfo(PS_GUID_USB_CONFIG& psGuidUsbConfig,SDeviceMsg_Info* lpdevice/*,PS_REG_DEVICE_POOL_DEVINFO psRegDevPoolInfo*/);

        int GetUsbComPort(const wchar_t* strInstance);
    private:
        bool FilteADBConfigInfo(PS_GUID_USB_CONFIG& psGuidUsbConfig,SDeviceMsg_Info* lpdevice,PS_REG_DEVICE_POOL_DEVINFO psRegDevPoolInfo);

        bool FilteComConfigInfo(PS_GUID_USB_CONFIG& psGuidUsbConfig,SDeviceMsg_Info* lpdevice,PS_REG_DEVICE_POOL_DEVINFO psRegDevPoolInfo);

        bool FilteModemonfigInfo(PS_GUID_USB_CONFIG& psGuidUsbConfig,SDeviceMsg_Info* lpdevice,PS_REG_DEVICE_POOL_DEVINFO psRegDevPoolInfo);

        bool FilteLanConfigInfo(PS_GUID_USB_CONFIG& psGuidUsbConfig,SDeviceMsg_Info* lpdevice,PS_REG_DEVICE_POOL_DEVINFO psRegDevPoolInfo);

        bool FilteMaxUsbConfigInfo(PS_GUID_USB_CONFIG& psGuidUsbConfig,SDeviceMsg_Info* lpdevice,PS_REG_DEVICE_POOL_DEVINFO psRegDevPoolInfo);
    private:
        bool CheckUsbPidVid(const wchar_t* strInstance,S_USB_CONFIG* psUsbConfig);

        bool CheckUsbService(const wchar_t* strInstance,S_USB_CONFIG* psUsbConfig);

        bool CheckUsbDeviceDes(const wchar_t* strInstance,S_USB_CONFIG* psUsbConfig);

        bool CheckUsbClass(const wchar_t* strInstance,S_USB_CONFIG* psUsbConfig);

        std::wstring GetADBSerialNumber(const wchar_t* strInstance);
    };

}

