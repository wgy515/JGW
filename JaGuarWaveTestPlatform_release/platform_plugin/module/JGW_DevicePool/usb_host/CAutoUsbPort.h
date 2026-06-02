#pragma once
#include "..\stdafx.h"
#include "AutoUsbDefine.h"
#include "UsbQueryBase.h"

class CCAutoUsbPort :public CUsbQueryBase
{
public:
    void						GetSerialNumber(HANDLE hHubDevice,ULONG ConnectionIndex,PUSB_DEVICE_DESCRIPTOR DeviceDesc);
    void						EnumerateHostControllers();// 遍历总线控制
    //bool                           GetHubCurrentPortsInfo(_Inout_ HUBPORTPOINTINFO &sHubCurrentPortInfo);
    void						ClearSortInfo();
    //单例模式
    static CCAutoUsbPort*		GetInstance()
    {
        static CCAutoUsbPort cAutoUsbPort;
        return &cAutoUsbPort ;
    }
    //private function
private:
    CCAutoUsbPort();
    ~ CCAutoUsbPort();
    void UpdateSortIniConfig();
    void WriteSortIniConfig(PHUBPORTPOINTINFO lpUsbInfo);
    void LoadSortIniConfig(LPCTSTR lpIniPath);
    void LoadFilterIniConfig();
    bool EnumChildHubPortControl(DEVINST dev);
    void EnumerateHubPorts(HANDLE hHubDevice, ULONG NumPorts,std::wstring strHubName);// 遍历HUB上端口

    void EnumerateHub(LPTSTR HubName,
        PUSB_NODE_CONNECTION_INFORMATION ConnectionInfo ,
        PUSB_DESCRIPTOR_REQUEST ConfigDesc ,
        PSTRING_DESCRIPTOR_NODE StringDescs ,
        LPTSTR DeviceDesc,std::wstring strHubName); //遍历HUB

    PSTRING_DESCRIPTOR_NODE GetStringDescriptors(HANDLE hHubDevice,ULONG ConnectionIndex,
        UCHAR DescriptorIndex,ULONG NumLanguageIDs,
        USHORT* LanguageIDs,PSTRING_DESCRIPTOR_NODE StringDescNodeTail);

    PSTRING_DESCRIPTOR_NODE GetStringDescriptor(HANDLE hHubDevice,
        ULONG ConnectionIndex,UCHAR DescriptorIndex,USHORT LanguageID);
private:
    int m_nConnectionInfoBytes;
    int m_nHCDIndex;
    PUSB_NODE_CONNECTION_INFORMATION m_lpConnectionInfo ;
    PHUBPORTPOINTINFO m_lpHubPortPointInfo ;
    TCHAR m_szBuffer[MAX_PATH ];
    std::wstring mstrRootHubName;
    //! std::wstring m_strHubName;
    std::wstring m_strPhysicalAddress;// 外部HUB端口索引
};