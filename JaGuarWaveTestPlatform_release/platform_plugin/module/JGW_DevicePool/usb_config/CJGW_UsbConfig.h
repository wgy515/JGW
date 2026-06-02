#pragma once
#include <JGW_DevicePool/DevicePortDefine.h>
#include <vector>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
namespace JGW
{
    class CCJGW_UsbConfig
    {
    public:
        CCJGW_UsbConfig(void);
        ~CCJGW_UsbConfig(void);

        bool LoadUsbConfigToConfigPath(const wchar_t* strConfigPath,std::map<GUID_KEY,S_GUID_USB_CONFIG>& sGuidUsbConfig);
    private:
        bool GetUsbConfigGuid(DWORD dwIndex,E_GUID_TYPE& eGuidTyep,GUID &guid);
        void GetUsbConfigInfo(DWORD dwIndex,S_USB_CONFIG& sSubConfig);
    private:
        CCJGW_ConfigIni mIniConfig;
    };
}

