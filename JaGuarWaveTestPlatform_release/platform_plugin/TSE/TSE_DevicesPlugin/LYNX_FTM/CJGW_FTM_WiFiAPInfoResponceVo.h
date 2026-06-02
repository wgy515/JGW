#pragma once
#include "CJGW_FTM_SocketResponceVo.h"

namespace JGW
{
    struct S_WIFI_AP_INFO 
    {
         int level; //! RSSI
         int frequency; //! AP FREQ
         std::string ssid;//! WIFI SSID
         std::string bssid; //! WIFI AP MAC
    };

    class CCJGW_FTM_WiFiAPInfoResponceVo : public CCJGW_FTM_SocketResponceVo
    {
    public:
        CCJGW_FTM_WiFiAPInfoResponceVo(void);
        ~CCJGW_FTM_WiFiAPInfoResponceVo(void);

        bool FromJosn(CCJGW_FTM_Json& ftmJson);
    public:
        std::vector<S_WIFI_AP_INFO> mvsWiFiAPInfo;
    };
}

