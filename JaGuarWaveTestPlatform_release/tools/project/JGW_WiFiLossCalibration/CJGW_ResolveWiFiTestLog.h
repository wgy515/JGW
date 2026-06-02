#pragma once
#include "JGW_WiFiLossCalibrationDefine.h"

namespace JGW
{
    class CCJGW_ResolveWiFiTestLog
    {
    public:
        CCJGW_ResolveWiFiTestLog(void);
        ~CCJGW_ResolveWiFiTestLog(void);
    public:
        bool ResolveWiFiTestLog(std::wstring& strTestLogPath,std::vector<S_WIFI_TEST_INFO>& vsWiFiTestInfo);
    public:
        bool ResolveWiFiTestHeader(const std::string& strWiFiTestHeader,S_WIFI_TEST_INFO& sWiFiTestInfo);
        WLAN_UniversalDataRate GetWlanUniversalDataRate(const std::string& strDataRate);
        WLAN_CHAIN_MASK2 GetWlanChainMask(const std::string& strChainMask);
    };

}

