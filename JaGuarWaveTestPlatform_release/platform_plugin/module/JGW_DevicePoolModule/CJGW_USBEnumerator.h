#pragma once
#include <Windows.h>
#include <string>
#include <cfgmgr32.h>
#include <SetupAPI.h>
#include <vector>
#include "usb/usb100.h"
#include <JGW_DevicePool/CJGW_DevicePoolModule_Define.h>
#define GET_DEV_INFO_BUFFER_SIZE 2048

namespace JGW
{
    class CCJGW_USBEnumerator
    {
    public:
        CCJGW_USBEnumerator(void);
        ~CCJGW_USBEnumerator(void);
    public:
        //! strDbccName = \\?\USB#Vid_05c6&Pid_9091&MI_00#7&30607750&1&0000#{86e0d1e0-8089-11d0-9ce4-08003e301f73}
        //! DbccName对应的物理地址
        std::wstring    GetPhysicalAddress(const std::wstring& strDbccName);

        std::wstring    GetSerialNumber() { return mstrSerialNumber;}
        //! 过滤USB VID PID 
        //!void            SetFilterUsbVidPidVector(std::vector<PIDANDVIDINFO>* vFilteUsbPidVid);
    private:
        //! 解析DbccName
        void            GetUsbPidVidToDbccName(const std::wstring& strDbccName);
        //! 从PNP设备句柄中读取物理地址
        std::wstring    GetPhysicalAddressToHDEVINFO(HDEVINFO hDevInfo,PSP_DEVINFO_DATA pSpDevInfoData);
        //! 获取DEVINST对应在HUB的端口ID
        size_t          GetConnectionIndex(DEVINST devInst,std::wstring& DevicePath);
        //! 比较DriverName
        bool            CompareDriverName(DEVINST devInst,const wchar_t* strDriverName);
        //! 获取DEVINST对应的注册Driver属性
        std::wstring    GetCMPropertyString(DEVINST ptrDevice,int cmProperty = CM_DRP_DRIVER);
        //!
        void            GetSerialNumberDesc(HANDLE hHubDevice,ULONG ConnectionIndex,PUSB_DEVICE_DESCRIPTOR DeviceDesc);

        PSTRING_DESCRIPTOR_NODE GetStringDescriptors(HANDLE hHubDevice,ULONG ConnectionIndex,
            UCHAR DescriptorIndex,ULONG NumLanguageIDs,
            USHORT* LanguageIDs,PSTRING_DESCRIPTOR_NODE StringDescNodeTail);

        PSTRING_DESCRIPTOR_NODE GetStringDescriptor(HANDLE hHubDevice,
            ULONG ConnectionIndex,UCHAR DescriptorIndex,USHORT LanguageID);
        //! 返回false 则表示过滤当前PID VID
        /*bool            FilterUsbPidVid();*/
    private:
        char*           mszDevInfoBuf;
        wchar_t*        mwszDevInfoBuf;
        /*std::vector<PIDANDVIDINFO>* mvFilteUsbPidVid;*/
        std::wstring    mstrRegUsbPidVid;
        std::wstring    mstrSerialNumber;
    };
}

