#pragma once
#include "JGW_WindowsFuncPlugin_Define.h"
#include <PortableDeviceApi.h>      // Include this header for Windows Portable Device API interfaces
#include <PortableDevice.h>         // Include this header for Windows Portable Device definitions
#include <string>
#pragma comment(lib,"PortableDeviceGuids.lib")
namespace JGW
{
    class JGWWINDOWSFUNCPLUGIN_CLASS CCJGW_MediaTransferProtocol
    {
    public:
        CCJGW_MediaTransferProtocol(void);
        ~CCJGW_MediaTransferProtocol(void);
        //! 获取MTP设备信息  
        bool GetMTPDevices(const wchar_t* strManufacturer,const wchar_t* strDescription);
    private:
        //! 获取制造商名称
        std::wstring DisplayManufacturer(_In_ IPortableDeviceManager* deviceManager,
            _In_ PCWSTR pnpDeviceID);
        //! 获取设备描述信息
        std::wstring DisplayDescription(
            _In_ IPortableDeviceManager* deviceManager,
            _In_ PCWSTR                  pnpDeviceID);
        //! 获取客户信息
        void GetClientInformation(IPortableDeviceValues** clientInformation);

        bool OpenMTPDevice(LPCWSTR pszPnPDeviceID,IPortableDeviceValues* clientInformation);
    };
}