#include "StdAfx.h"
#include "CJGW_USBEnumerator.h"

#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_StringFuncTemplate.hpp>
#include <JGW_FoundationFunc/jgw_smartpointer_handle_func.h>
#include "usb/usbioctl.h"

#pragma comment(lib,"SetupAPI.lib")
const std::wstring GUID_HUB_DRIVER = L"{f18a0e88-c30c-11d0-8815-00a0c906bed8}";
#define INVALID_DEVINST_VALUE ((DWORD)-1)

namespace JGW
{
    CCJGW_USBEnumerator::CCJGW_USBEnumerator(void)
    {
        mszDevInfoBuf = (char*)calloc(0x01,GET_DEV_INFO_BUFFER_SIZE);
        mwszDevInfoBuf = (wchar_t*)mszDevInfoBuf;
    }


    CCJGW_USBEnumerator::~CCJGW_USBEnumerator(void)
    {
        if (mszDevInfoBuf) free(mszDevInfoBuf);
    }

    void CCJGW_USBEnumerator::GetUsbPidVidToDbccName(const std::wstring& strDbccName)
    {
        mstrRegUsbPidVid = strDbccName;
        if( std::wstring::npos != mstrRegUsbPidVid.find(L"USB#") )
        {
            mstrRegUsbPidVid.erase(0,mstrRegUsbPidVid.find(L"USB#") );
            mstrRegUsbPidVid.erase(mstrRegUsbPidVid.find(L"#{"));
            JGW_ReplaceCharW(mstrRegUsbPidVid,L'#',L'\\');
        }
    }

    std::wstring CCJGW_USBEnumerator::GetPhysicalAddress(const std::wstring& strDbccName)
    {
        GetUsbPidVidToDbccName(strDbccName);
        mstrSerialNumber = L"";
        std::shared_ptr<void> hDevInfo(SetupDiGetClassDevs(0, mstrRegUsbPidVid.c_str(), NULL, (DIGCF_PRESENT | DIGCF_ALLCLASSES)),DestroyDeviceInfoList);
        if (INVALID_HANDLE_VALUE == hDevInfo.get()) return L"";

        SP_DEVINFO_DATA spDevInfoData = {0};
        spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
        if (!SetupDiEnumDeviceInfo(hDevInfo.get(), 0,&spDevInfoData)) return L"";

        return GetPhysicalAddressToHDEVINFO(hDevInfo.get(),&spDevInfoData);
    }

    std::wstring CCJGW_USBEnumerator::GetPhysicalAddressToHDEVINFO(HDEVINFO hDevInfo,PSP_DEVINFO_DATA pSpDevInfoData)
    {
        DEVINST pParentDevInst = INVALID_DEVINST_VALUE,pChildDevInst = INVALID_DEVINST_VALUE;
        DWORD crResult = CR_SUCCESS;
        size_t connectIndex = 0;
        std::wstring strPhysicalAddress,ParentPNPDevicePath,strTemp;
        if (CR_SUCCESS != CM_Get_Parent(&pParentDevInst,pSpDevInfoData->DevInst,0) || INVALID_DEVINST_VALUE == pParentDevInst) return L"";

        while (!crResult)
        {
            if (CR_SUCCESS != CM_Get_Device_ID(pParentDevInst,(PWSTR)mszDevInfoBuf, GET_DEV_INFO_BUFFER_SIZE, 0)) break;
            if (NULL != _tcsstr((const wchar_t*)mszDevInfoBuf,L"IUSB3"))
            {
                strPhysicalAddress = (wchar_t*)mszDevInfoBuf;
                JGW::JGW_StringFunc<std::wstring>::ReplaceString(strPhysicalAddress,L"\\",L"#");
                JGW::JGW_StringFunc<std::wstring>::FormatWString(ParentPNPDevicePath,L"\\\\.\\%s#%s",strPhysicalAddress.c_str(),GUID_HUB_DRIVER.c_str());
                connectIndex = GetConnectionIndex(pSpDevInfoData->DevInst,ParentPNPDevicePath);
                if (!connectIndex) return L"";
                JGW::JGW_StringFunc<std::wstring>::FormatWString(strTemp,L"%sPORT%d",strPhysicalAddress.c_str(),connectIndex);
                return strTemp;
            }
            crResult = CM_Get_Parent(&pChildDevInst,pParentDevInst,0);
            pParentDevInst = pChildDevInst;
        }
        return L"";
    }

    size_t CCJGW_USBEnumerator::GetConnectionIndex(DEVINST devInst,std::wstring& DevicePath)
    {
        std::shared_ptr<void> hDeviceHandle(CreateFile(DevicePath.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL),CloseFileHandle);
        if (INVALID_HANDLE_VALUE == hDeviceHandle.get()) return 0;

        DWORD nBytesReturned = 0;  
        char szBuf[GET_DEV_INFO_BUFFER_SIZE] = {0};
        PUSB_NODE_CONNECTION_INFORMATION pUsbNodeConnnectionInformation = (PUSB_NODE_CONNECTION_INFORMATION)szBuf;
        USB_NODE_INFORMATION sUsbNodeInformation;
        PUSB_NODE_CONNECTION_DRIVERKEY_NAME pUsbNodeConnectionDriverkeyName = (PUSB_NODE_CONNECTION_DRIVERKEY_NAME)mszDevInfoBuf;

        if (!DeviceIoControl(hDeviceHandle.get(),
            IOCTL_USB_GET_NODE_INFORMATION,
            &sUsbNodeInformation,
            sizeof(USB_NODE_INFORMATION),
            &sUsbNodeInformation,
            sizeof(USB_NODE_INFORMATION),
            &nBytesReturned,
            NULL)) 
            return 0;

        if (sUsbNodeInformation.NodeType != UsbHub) return 0;

        // loop thru all of the ports on the hub
        // BTW: Ports are numbered starting at 1
        for (int i = 1; 
            i <= sUsbNodeInformation.u.HubInformation.HubDescriptor.bNumberOfPorts; 
            i++)
        {
            memset(mszDevInfoBuf,0x00,GET_DEV_INFO_BUFFER_SIZE);
            pUsbNodeConnectionDriverkeyName->ConnectionIndex = i;

            pUsbNodeConnnectionInformation->ConnectionIndex = i;
            if (!DeviceIoControl(hDeviceHandle.get(),
                IOCTL_USB_GET_NODE_CONNECTION_INFORMATION,
                pUsbNodeConnnectionInformation,
                GET_DEV_INFO_BUFFER_SIZE,
                pUsbNodeConnnectionInformation,
                GET_DEV_INFO_BUFFER_SIZE,
                &nBytesReturned,
                NULL)) return 0;

            if (!DeviceIoControl(hDeviceHandle.get(),
                IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME,
                pUsbNodeConnectionDriverkeyName,
                sizeof(USB_NODE_CONNECTION_DRIVERKEY_NAME),
                pUsbNodeConnectionDriverkeyName,
                GET_DEV_INFO_BUFFER_SIZE,
                &nBytesReturned,
                NULL)) continue;

            if (CompareDriverName(devInst,pUsbNodeConnectionDriverkeyName->DriverKeyName))
            {
                GetSerialNumberDesc(hDeviceHandle.get(),i, &pUsbNodeConnnectionInformation->DeviceDescriptor);
                return i;
            }
        }
        return 0;
    }

    PSTRING_DESCRIPTOR_NODE CCJGW_USBEnumerator::GetStringDescriptors(HANDLE hHubDevice,ULONG ConnectionIndex,
        UCHAR DescriptorIndex,ULONG NumLanguageIDs,
        USHORT* LanguageIDs,PSTRING_DESCRIPTOR_NODE StringDescNodeTail)
    {
        for (ULONG i = 0; i < NumLanguageIDs;i ++)
        {
            StringDescNodeTail->Next = GetStringDescriptor(hHubDevice,ConnectionIndex,DescriptorIndex,*LanguageIDs);
            if ( StringDescNodeTail->Next ) StringDescNodeTail = StringDescNodeTail->Next;
            LanguageIDs++;
        }
        //
        return StringDescNodeTail;
    }

    PSTRING_DESCRIPTOR_NODE CCJGW_USBEnumerator::GetStringDescriptor(HANDLE hHubDevice,
        ULONG ConnectionIndex,UCHAR DescriptorIndex,USHORT LanguageID)
    {
        BOOL    success;
        ULONG   nBytes;
        ULONG   nBytesReturned;

        UCHAR   stringDescReqBuf[sizeof(USB_DESCRIPTOR_REQUEST) + MAXIMUM_USB_STRING_LENGTH];
        PUSB_DESCRIPTOR_REQUEST stringDescReq;
        PUSB_STRING_DESCRIPTOR  stringDesc;
        PSTRING_DESCRIPTOR_NODE stringDescNode;
        nBytes = sizeof(stringDescReqBuf);
        stringDescReq = (PUSB_DESCRIPTOR_REQUEST)stringDescReqBuf;
        stringDesc = (PUSB_STRING_DESCRIPTOR)(stringDescReq+1);
        // 置0
        memset(stringDescReq, 0, nBytes);
        // 指出所请求描述符的口
        stringDescReq->ConnectionIndex = ConnectionIndex;
        // USBHUB使用URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE来处理该
        // IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION 请求.
        // USBD会自动初始化这些段:
        // bmRequest = 0x80
        // bRequest  = 0x06
        // 我们必须初始化下面的段:
        // wValue    = 描述符类型(高字节)和描述符索引(低字节)
        // wIndex    = 0 (或者对字串描述符为语言ID)
        // wLength   = 描述符缓存的长度
        stringDescReq->SetupPacket.wValue = (USB_STRING_DESCRIPTOR_TYPE << 8) | DescriptorIndex;
        stringDescReq->SetupPacket.wIndex = LanguageID;
        stringDescReq->SetupPacket.wLength = (USHORT)(nBytes - sizeof(USB_DESCRIPTOR_REQUEST));
        // 发出取描述符请求.
        success = DeviceIoControl(hHubDevice,IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
            stringDescReq,nBytes,stringDescReq,nBytes,&nBytesReturned,NULL);
        // 检查返回值.
        if (!success 
		|| nBytesReturned < 2
		|| stringDesc->bDescriptorType != USB_STRING_DESCRIPTOR_TYPE
		|| stringDesc->bLength != nBytesReturned - sizeof(USB_DESCRIPTOR_REQUEST)
		|| stringDesc->bLength % 2 != 0)
        {
            return NULL;
        }

        // 分配内存并复制字串描述符.
        stringDescNode = (PSTRING_DESCRIPTOR_NODE) new CHAR[sizeof(STRING_DESCRIPTOR_NODE) + stringDesc->bLength];
        memset(stringDescNode,0x00,sizeof(STRING_DESCRIPTOR_NODE) + stringDesc->bLength);
        stringDescNode->DescriptorIndex = DescriptorIndex;
        stringDescNode->LanguageID = LanguageID;
        memcpy(stringDescNode->StringDescriptor,stringDesc,stringDesc->bLength);

        return stringDescNode;
    }

    void CCJGW_USBEnumerator::GetSerialNumberDesc(HANDLE hHubDevice,ULONG ConnectionIndex,PUSB_DEVICE_DESCRIPTOR DeviceDesc)
    {
        if ( !DeviceDesc->iSerialNumber ) return;
        PSTRING_DESCRIPTOR_NODE supportedLanguagesString;
        PSTRING_DESCRIPTOR_NODE stringDescNodeTail;
        ULONG                   numLanguageIDs;
        USHORT                  *languageIDs;
        // 	PUCHAR                  descEnd;
        // 	PUSB_COMMON_DESCRIPTOR  commonDesc;
        // 取支持的语言ID数组,它在字串描述符的0中返回
        supportedLanguagesString = GetStringDescriptor(hHubDevice,ConnectionIndex,0,0);
        if (supportedLanguagesString == NULL) return /*NULL*/;
        numLanguageIDs = (supportedLanguagesString->StringDescriptor->bLength - 2) / 2;
        languageIDs = (USHORT*)&supportedLanguagesString->StringDescriptor->bString[0];
        stringDescNodeTail = supportedLanguagesString;
        // 取设备描述符字串
        stringDescNodeTail = GetStringDescriptors(hHubDevice,ConnectionIndex,DeviceDesc->iSerialNumber,
            numLanguageIDs,languageIDs,stringDescNodeTail);

        unsigned long str_len = (stringDescNodeTail->StringDescriptor->bLength -
            FIELD_OFFSET(USB_STRING_DESCRIPTOR, bString)) /
            sizeof(wchar_t);

        mstrSerialNumber = stringDescNodeTail->StringDescriptor->bString;
        mstrSerialNumber[str_len] = '\0';

        PSTRING_DESCRIPTOR_NODE pTemp = NULL;
        while (supportedLanguagesString)
        {
            pTemp = supportedLanguagesString->Next;
            delete supportedLanguagesString;
            if (!pTemp) break;
            supportedLanguagesString = pTemp;
        }
    }

    std::wstring CCJGW_USBEnumerator::GetCMPropertyString(DEVINST ptrDevice,int cmProperty)
    {
        ULONG kind = REG_SZ,RequiredSize = GET_DEV_INFO_BUFFER_SIZE;

        CM_Get_DevNode_Registry_Property(ptrDevice, cmProperty, &kind, mszDevInfoBuf,&RequiredSize, 0);

        return mwszDevInfoBuf;
    }

    bool CCJGW_USBEnumerator::CompareDriverName(DEVINST devInst,const wchar_t* strDriverName)
    {
        DEVINST pParentDev ,pChildDev = devInst;
        std::wstring strTemp = strDriverName;
        while (1)
        {
            if (NULL == JGW_WStrComparenoCaseWStr(strTemp.c_str(),GetCMPropertyString(pChildDev,CM_DRP_DRIVER).c_str())) return true;
            if (CR_SUCCESS != CM_Get_Parent(&pParentDev,pChildDev,0)) return false;
            pChildDev = pParentDev;
        }
        return false;
    }
}