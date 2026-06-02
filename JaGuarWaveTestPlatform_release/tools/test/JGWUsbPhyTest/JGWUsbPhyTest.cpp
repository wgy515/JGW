// JGWUsbPhyTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <SetupAPI.h>
#include <string>
#include <iostream>
#include <cfgmgr32.h>
#include <JGW_FoundationFunc/JGW_StringFuncTemplate.hpp>
#include "usb/usbioctl.h"
#include "CJGW_USBEnumerator.h"

#pragma comment(lib,"SetupAPI.lib")
#pragma comment(lib,"Rpcrt4.lib")

// public const string GUID_ADB = "{3f966bd9-fa04-4ec5-991c-d326973b5128}"; //ADB or fastboot devices
// public const string GUID_PORT = "{4D36E978-E325-11CE-BFC1-08002BE10318}"; //Serial and parallel ports
// public const string GUID_HUB = "{36fc9e60-c465-11cf-8056-444553540000}"; //Hub & Composite device. This class includes system-supplied (bus) drivers of USB host controllers and drivers of USB hubs, but not drivers of USB peripherals.
const std::wstring GUID_HUB_DRIVER = L"{f18a0e88-c30c-11d0-8815-00a0c906bed8}";

#define BUFFER_SIZE 2048

#define REGSTR_KEY_USB L"USB"

UINT32 invail = -1;
/*DWORD SPDRP_FRIENDLYNAME = 0x0000000C; // FriendlyName (R/W)*/

//int ConnectIndex = GetConnectionIndex(ParentPNPDevicePath, DriverKeyName);

bool EnumChildHubPortControl( DEVINST dev)
{
    //static std ::wstring strChildInstanceID;
    static TCHAR szBuffer[ MAX_PATH] = {0};
    DEVINST     devInstNext = NULL;
    CONFIGRET   cr = CR_SUCCESS;
    ULONG       len = 0;
    ULONG       walkDone = 0;

    if (CM_Get_Device_ID (dev, szBuffer,260,NULL ) == CR_SUCCESS)
    {
       std::wcout << szBuffer << std::endl;
    }

    cr = CM_Get_Child (&devInstNext, dev,0);
    if (cr != CR_SUCCESS )
    {
        return false ;
    }
    dev = devInstNext ;

    // 深一步搜索与DriverName匹配的 DevNode
    while(!walkDone )
    { // 取DriverName

        if (CM_Get_Device_ID (dev, szBuffer,260,NULL ) == CR_SUCCESS)
        {
            //strChildInstanceID = szBuffer ;
            std::wcout << szBuffer << std::endl;
            //l/*pParam->m_vChildInstanceID.push_back( szBuffer );*/
        }

        cr = CM_Get_Sibling (&devInstNext, dev,0);
        if (cr != CR_SUCCESS )
        {
            return false ;
        }
        dev = devInstNext ;
    }
    return true ;
}

DEVINST CM_GetDevInstToDriverKeyName(const wchar_t* szDriverKeyName)
{
    DEVINST     devInst ;
    DEVINST     devInstNext ;
    CONFIGRET   cr ;
    ULONG       len;
    bool       bFind = false;
    wchar_t     strDeviceNameDesc[MAX_PATH] = {0};

    // 取根节点DevNode
    cr = CM_Locate_DevNode (&devInst, NULL,0);
    if (cr != CR_SUCCESS) return -1;

    while(!bFind)
    {
        len = MAX_PATH ;
        cr = CM_Get_DevNode_Registry_Property (devInst, CM_DRP_DRIVER,NULL ,strDeviceNameDesc,&len,0);
        // 如果与DriverName 匹配,返回设备描述符

        if ( cr == CR_SUCCESS && _tcscmp( szDriverKeyName ,strDeviceNameDesc ) == 0)
        {
            return devInst;
            /*len = MAX_PATH ;
            cr = CM_Get_DevNode_Registry_Property (devInst, CM_DRP_DEVICEDESC,NULL ,strDeviceNameDesc,&len,0);
            TCHAR szBuffer [MAX_PATH] = {0};
            if (CM_Get_Device_ID (devInst, szBuffer,260,NULL ) == CR_SUCCESS)
            {
                psPortInstanceInfo->m_szInstanceID = szBuffer;
                EnumChildHubPortControl(devInst ,psPortInstanceInfo);
            }
            if (cr == CR_SUCCESS ) return psPortInstanceInfo ;
            break;*/
        }
        // 该DevNode 不匹配,再深一级到第一个子节点
        cr = CM_Get_Child (&devInstNext, devInst,0);
        if ( cr == CR_SUCCESS )
        {
            devInst = devInstNext ;
            continue;
        }
        // 不能再往下,就到兄弟节点如果没兄弟 ,朝上一级直到有兄弟
        // 如果不能再朝上,那么到了根节点，也就做完了
        for (;;)
        {
            cr = CM_Get_Sibling (&devInstNext, devInst,0);
            if (cr == CR_SUCCESS )
            {
                devInst = devInstNext ;
                break;
            }
            cr = CM_Get_Parent (&devInstNext, devInst,0);
            if (cr == CR_SUCCESS )
            {
                devInst = devInstNext ;
            }
            else
            {
                bFind = true;
                break;
            }
        }
    }
    return (DWORD)-1;
}

int GetConnectionIndex(std::wstring& DevicePath,std::wstring& DriverKeyName,DEVINST hDevInst)
{
    HANDLE deviceInfoSet = CreateFile(DevicePath.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (INVALID_HANDLE_VALUE == deviceInfoSet) return -1;
    int ConnectionIndex = -1;
    int HubPortCount = 0;
    DWORD cr = CR_SUCCESS;
    DWORD nBytesReturned;  
    PUSB_NODE_INFORMATION Buffer = new USB_NODE_INFORMATION();
    if (DeviceIoControl(deviceInfoSet,
        IOCTL_USB_GET_NODE_INFORMATION,
        Buffer,
        sizeof(USB_NODE_INFORMATION),
        Buffer,
        sizeof(USB_NODE_INFORMATION),
        &nBytesReturned,
        NULL))
    {
        if (Buffer->NodeType == UsbHub)
        {
            HubPortCount = Buffer->u.HubInformation.HubDescriptor.bNumberOfPorts;         // Port Number
        }
        else
        {
            HubPortCount = 0;
            // UpdateUSBDetectionLog("Device is not a Hub, devInst:" + deviceInfoSet.ToString());
        }

        // loop thru all of the ports on the hub
        // BTW: Ports are numbered starting at 1
        for (int i = 1; i <= HubPortCount; i++)
        {
            char szBuf[BUFFER_SIZE] = {0};
            PUSB_NODE_CONNECTION_DRIVERKEY_NAME NodeKeyName = (PUSB_NODE_CONNECTION_DRIVERKEY_NAME)szBuf;

            NodeKeyName->ConnectionIndex = i;
            if (DeviceIoControl(deviceInfoSet,
                IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME,
                NodeKeyName,
                sizeof(USB_NODE_CONNECTION_DRIVERKEY_NAME),
                NodeKeyName,
                BUFFER_SIZE,
                &nBytesReturned,
                NULL))
            {

                DEVINST dDevInst = CM_GetDevInstToDriverKeyName(NodeKeyName->DriverKeyName);
//                 ULONG len = MAX_PATH ;
//                 cr = CM_Get_DevNode_Registry_Property (hDevInst, CM_DRP_DEVICEDESC,NULL ,NodeKeyName->DriverKeyName,&len,0);
                EnumChildHubPortControl((DEVINST)dDevInst);
                std::wcout << NodeKeyName->DriverKeyName << L"   --" <<ConnectionIndex << std::endl;
                if (NodeKeyName->DriverKeyName == DriverKeyName)
                {
                    ConnectionIndex = NodeKeyName->ConnectionIndex;
                    //break;
                }
            }
            else
            {
                // UpdateUSBDetectionLog("Fail to get connection drivery key name of devInst:" + deviceInfoSet.ToString() + "connection index:" + i.ToString());
            }
        }
    }
    else
    {
        // UpdateUSBDetectionLog("Fail to get Node Type of devInst:" + deviceInfoSet.ToString());
    }

    CloseHandle(deviceInfoSet);
    return ConnectionIndex;
}

std::wstring GetCMPropertyString(DEVINST ptrDevice,int cmProperty)
{
    wchar_t ptrBuf[BUFFER_SIZE] = {0};
    ULONG kind = 0,RequiredSize = 0;

    CM_Get_DevNode_Registry_Property(ptrDevice, cmProperty, &kind, NULL,&RequiredSize, 0);
    if (RequiredSize > 0 && kind == REG_SZ)
        if (CM_Get_DevNode_Registry_Property(ptrDevice, cmProperty, & kind, ptrBuf, & RequiredSize, 0) == CR_SUCCESS)
        {
            return ptrBuf;
        }


        return L"";
}

std::wstring GetPhysicalAddress(HDEVINFO deviceInfoSet,PSP_DEVINFO_DATA deviceInfoData)
{
#if 0
    DWORD RegType = 1;
    DWORD RequiredSize = 0;
    char ptrBuf[2048] = {0};

    if (SetupDiGetDeviceRegistryProperty(deviceInfoSet,deviceInfoData, SPDRP_LOCATION_PATHS, &RegType, (PBYTE)ptrBuf, 2048, &RequiredSize))
    {
        //! Port_#0020.Hub_#0001
        //! thisPhysicalAddress = Marshal.PtrToStringAuto(ptrBuf);
        std::wcout << L"PhysicalAddress:" << (const wchar_t*)ptrBuf << std::endl;
        //Get the location information
        if (SetupDiGetDeviceRegistryProperty(deviceInfoSet,deviceInfoData, SPDRP_LOCATION_INFORMATION,&RegType, (PBYTE)ptrBuf, 2048,  &RequiredSize))
        {
            std::wcout << L"LocationInformation:" << (const wchar_t*)ptrBuf << std::endl;
        }
    }
#else
    wchar_t ptrBuf[2048] = {0};
    DWORD cr = CR_SUCCESS;
    DEVINST ptrDevice = invail,ptrParent = invail,ptrDevPar = invail;
    if(CR_SUCCESS == CM_Get_Device_ID(deviceInfoData->DevInst, (PWSTR)ptrBuf, BUFFER_SIZE,0))
    {
        //! Match match = Regex.Match(DeviceID, "VID_[0-9|A-F]{4}&PID_[0-9|A-F]{4}&MI_[0-9|A-F]{2}");
        std::wcout << L"Device ID:" << (const wchar_t*)ptrBuf << std::endl;
        cr = CM_Get_Parent(&ptrDevice,deviceInfoData->DevInst,0);
        ptrDevPar = ptrDevice;
        std::wstring strDriverName = GetCMPropertyString(ptrDevice,CM_DRP_DRIVER);
        std::wcout << L"DriverName:" << strDriverName << std::endl;
        if (cr == CR_SUCCESS && ptrDevice != invail)
        {

            while (1)
            {
                if (CR_SUCCESS == CM_Get_Parent(&ptrParent, ptrDevice, 0) &&
                    ptrParent != invail)
                {
                    strDriverName = GetCMPropertyString(ptrParent,CM_DRP_DRIVER);
                    std::wcout << L"DriverName:" << strDriverName << std::endl;

                    if (CM_Get_Device_ID(ptrDevice, ptrBuf, BUFFER_SIZE, 0) == CR_SUCCESS)
                    {
                        std::wcout << L"PhysicalAddress:" << (const wchar_t*)ptrBuf << std::endl;  
                        if (NULL != _tcsstr(ptrBuf,L"IUSB3"))
                        {
                            std::wstring strDriverName = GetCMPropertyString(ptrDevice,CM_DRP_DRIVER);
                            std::wstring parentPhysicalAddress = ptrBuf;

                            std::wcout << L"DriverName:" << strDriverName << std::endl;

                            JGW::JGW_StringFunc<std::wstring>::ReplaceString(parentPhysicalAddress,L"\\",L"#");
                            std::wstring ParentPNPDevicePath = L"\\\\.\\";
                            ParentPNPDevicePath += parentPhysicalAddress;
                            ParentPNPDevicePath += L"#";
                            ParentPNPDevicePath += GUID_HUB_DRIVER;

                            std::wcout << L"PNP DevicePath: " << ParentPNPDevicePath << std::endl;
                            int ConnectIndex = GetConnectionIndex(ParentPNPDevicePath, strDriverName,ptrDevice);
                            std::wcout << L"PORT_" << ConnectIndex << std::endl;

                            break;
                        }
                        ptrDevice = ptrParent;
                    }
                }
            }
        }
    }

#endif
    return L"";
}
//！COM 端口
#define GUID_COM_INTERFACE_USB_DEVICE  { 0x86e0d1e0, 0x8089, 0x11d0, { 0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73 } }

int _tmain(int argc, _TCHAR* argv[])
{
#if 0
    std::wstring DevEnum = argv[1];//L"USB\\VID_18D1&PID_D00D\\5&2B74E18F&0&9"/*REGSTR_KEY_USB*/;
    std::wstring strKeyName;
    // to generate a list of all USB devices
    TCHAR szGUID[] = _T("4D36E97C-E325-11CE-BFC1-08002BE10318"); 
    GUID guid;                  //全局唯一标识符(GUID，Globally Unique Identifier)
    UuidFromString((RPC_WSTR)szGUID, &guid);
    //GUID guidCom = GUID_COM_INTERFACE_USB_DEVICE;
    //HDEVINFO deviceInfoSet = SetupDiGetClassDevs(&guid, NULL, NULL, (DIGCF_PRESENT | DIGCF_ALLCLASSES));
    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(0, DevEnum.c_str(), NULL, (DIGCF_PRESENT | DIGCF_ALLCLASSES));
    if (INVALID_HANDLE_VALUE == deviceInfoSet) return -1;
    BOOL Success = TRUE;
    DWORD deviceIndex = 0;
    SP_DEVINFO_DATA deviceInfoData = {0};
    DWORD RequiredSize = 0;
    DWORD Property = 1;

    while (Success)
    {
        memset(&deviceInfoData,0x00,sizeof(SP_DEVINFO_DATA));
        deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
        Success = SetupDiEnumDeviceInfo(deviceInfoSet, deviceIndex,&deviceInfoData);
        if (Success)
        {
            char pPropertyRegDataType[2048] = {0};
            if (SetupDiGetDeviceRegistryProperty(deviceInfoSet,&deviceInfoData,SPDRP_FRIENDLYNAME,&Property,(PBYTE)pPropertyRegDataType,2048,&RequiredSize))
            {
                strKeyName = (wchar_t*)pPropertyRegDataType;
                std::wcout << strKeyName << std::endl;
            }
            else if(SetupDiGetDeviceRegistryProperty(deviceInfoSet,&deviceInfoData,SPDRP_DEVICEDESC,&Property,(PBYTE)pPropertyRegDataType,2048,&RequiredSize))
            {
                strKeyName = (wchar_t*)pPropertyRegDataType;
                std::wcout << strKeyName << std::endl;
            }

            if(SetupDiGetDeviceRegistryProperty(deviceInfoSet,&deviceInfoData,SPDRP_CLASSGUID,&Property,(PBYTE)pPropertyRegDataType,2048,&RequiredSize))
            {
                strKeyName = (wchar_t*)pPropertyRegDataType;
                std::wcout << strKeyName << std::endl;
            }

            //! SPDRP_CLASSGUID

            std::wcout << GetPhysicalAddress(deviceInfoSet,&deviceInfoData) << std::endl;
        }
        deviceIndex ++;
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
#else
    long pid = 0,vid = 0;
     _stscanf_s(L"0001-00A2",L"%X-%X",&vid,&pid);


    JGW::CCJGW_USBEnumerator usb;

//     while(1)
//     {
        std::wcout << usb.GetPhysicalAddress(L"USB\\VID_05C6&PID_901D&MI_01\\6&C0DA6AF&0&0001") << std::endl;
        std::wcout << usb.GetSerialNumber() << std::endl;
//         Sleep(10);
//     }
    

#endif
    return 0;
}

