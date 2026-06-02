// JGW_WlanTool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_Wlan/CJGW_WlanInterfaceImpl.h>


using namespace JGW;
//! Realtek 8188GU Wireless LAN 802.11n USB NIC


typedef enum
{
    //! JGW_Wlan 0 "Wireless" "oplus_co_apxwvg" 
    //! JGW_WlanTool.exe 0 "Wireless" "oplus_co_apxwvg" -90
    E_WLAN_SCAN = 0,
    E_WLAN_OPERAYION_CODE_MAX
}E_OperationCode;

/*
RSSI < -50 dBm：信号强度为100%
-51 to -55 dBm：信号强度为90%
-56 to -62 dBm：信号强度为80%
-63 to -65 dBm：信号强度为75%
-66 to -68 dBm：信号强度为70%
-69 to -74 dBm：信号强度为60%
-75 to -79 dBm：信号强度为50%
-80 to -83 dBm：信号强度为30% 或 0‌
1
*/
int ConvertSignalQualityToRssi(int nSignalQuality)
{
    //quality = min(max(2 * (x + 100), 0), 100)
    return (nSignalQuality / 2) - 100;
}

void StartWLanScan(int argc, _TCHAR* argv[])
{
    int nMinRssi = -128;
    std::wstring strWlanInterfaceDescriptionKey(L""),strSsid;

    if (argc > 2)
    {
        strWlanInterfaceDescriptionKey = argv[2];
    }

    if (argc > 3)
    {
        strSsid = argv[3];
    }

    if (argc > 4)
    {
        nMinRssi = _ttoi(argv[4]);
    }
    //std::wcout << L"Wlan Scan Config : " << strWlanInterfaceDescriptionKey << L"-" << strSsid << L"-" << nMinRssi << std::endl;

    CCJGW_WlanInterfaceImpl* pWlanInterfaceImpl = GetWiFiInterfaceImpl();
    if (NULL == pWlanInterfaceImpl)
    {
        std::wcout << L"Get WiFi Interface is Empty" << std::endl;
        return ;
    }

    if (!pWlanInterfaceImpl->OpenWlanHandle())
    {
        std::wcout << L"Open Wlan Handle Fail" << std::endl;
        return ;
    }

    if (!pWlanInterfaceImpl->EnumWlanInterface(strWlanInterfaceDescriptionKey))
    {
        std::wcout << L"Not Find Wlan Interface " << strWlanInterfaceDescriptionKey << std::endl;
        return ;
    }

    std::map<std::string,int> mapWlanSsidSignalQuality;
    if (!GetWiFiInterfaceImpl()->GetScanNetworkList(mapWlanSsidSignalQuality))
    {
        std::wcout << L"Scan Wlan Fail" << std::endl;
        return;
    }
    //! 返回所有SSID 以及 RSSI
    if (strSsid.empty())
    {
        int count = 0;
        for (std::map<std::string,int>::iterator it = mapWlanSsidSignalQuality.begin();it != mapWlanSsidSignalQuality.end();++it)
        {
            int rssi = ConvertSignalQualityToRssi(it->second);
            if (rssi >= nMinRssi)
            {
                count ++;
                std::wstring strTempRssi = JGW_A2W(it->first,CP_UTF8);
                std::wcout << L"SSID : " ;
                std::cout << JGW_W2A(strTempRssi);
                std::wcout << L"," << ConvertSignalQualityToRssi(it->second) << std::endl;
            }
        }
        if (count > 0) std::wcout << L"Scan SSID successfully" << std::endl;
    }
    else
    {
        for (std::map<std::string,int>::iterator it = mapWlanSsidSignalQuality.begin();it != mapWlanSsidSignalQuality.end();++it)
        {
            int rssi = ConvertSignalQualityToRssi(it->second);
            if (0 == JGW_WStrComparenoCaseWStr(strSsid.c_str(),JGW_A2W(it->first,CP_UTF8).c_str()))
            {
                std::wstring strTempRssi = JGW_A2W(it->first,CP_UTF8);
                std::wcout << L"SSID : " ;
                std::cout << JGW_W2A(strTempRssi);
                std::wcout << L"," << ConvertSignalQualityToRssi(it->second) << std::endl;
                if (rssi >= nMinRssi) std::wcout << L"Scan SSID successfully" << std::endl;
                return;
            } 
        }
    }
    

    
    //if (!GetWiFiInterfaceImpl()->OpenWlanHandle())
    //{
    //    responseScanJson.returnMessage = "OpenWlanHandle Fail";
    //    return responseScanJson.BuildScanWlanJson(mapBss);
    //}

    //if (!GetWiFiInterfaceImpl()->EnumWlanInterface(L"Peraso"))
    //{
    //    responseScanJson.returnMessage = "未发现Peraso网卡设备插入";
    //    return responseScanJson.BuildScanWlanJson(mapBss);
    //}

    //if (!GetWiFiInterfaceImpl()->GetVisibleNetworkList(mapBss))
    //{
    //    responseScanJson.returnMessage = "GetVisibleNetworkList Fail";
    //    return responseScanJson.BuildScanWlanJson(mapBss);
    //}
    ////! GetWiFiInterfaceImpl()->CloseWlanHandle();
    //responseScanJson.bStatus = true;
    //return responseScanJson.BuildScanWlanJson(mapBss);

    return;
}


int _tmain(int argc, _TCHAR* argv[])
{
    std::cout.imbue(std::locale(""));
    std::wcout.imbue(std::locale(""));
    E_OperationCode opCode = E_WLAN_SCAN;
    if (argc > 1)
    {
        int nTemp = _ttoi(argv[1]);
        if (nTemp >= E_WLAN_SCAN && nTemp < E_WLAN_OPERAYION_CODE_MAX)
        {
            opCode = (E_OperationCode)nTemp;
        }
    }


    switch (opCode)
    {
    case E_WLAN_SCAN:
        StartWLanScan(argc,argv);
        break;
    default:
        return false;
    }

	return 0;
}

