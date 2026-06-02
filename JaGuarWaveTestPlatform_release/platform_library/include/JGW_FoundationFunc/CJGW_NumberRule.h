#pragma once

namespace JGW
{
    enum E_NUMBER_TYPE
    {
        E_SN = 0,
        E_IMEI,
        E_IMEI2,
        E_MEID,
        E_WIFI_MAC,
        E_ETH0_MAC,
        E_BT_MAC,
        E_2_4_SSID,
        E_2_4_PASSWORD,
        E_5_SSID,
        E_5_PASSWORD,
        E_AD_SSID,
        E_AD_PASSWORD,
        E_CLIENT_SN,
        E_Barcode,
        E_TinnoSN,
        E_UNKNOWN_NUMBER
    };

    const wchar_t gszNumberName[][20] = {
        L"SN",
        L"IMEI",
        L"IMEI2",
        L"MEID",
        L"WiFi Mac",
        L"eth0 Mac",
        L"Bluetooth Address",
        L"2.4G SSID",
        L"2.4G PassWord",
        L"5G SSID",
        L"5G PassWord",
        L"11AD SSID",
        L"11AD PassWord",
        L"Client SN",
        L"Barcode",
        L"Tinno SN",
        L"Unknown Number",
    }; 

    const wchar_t gszPerasoNumberName[][20] = {
        L"SN",
        L"MAC",
        L"FW Version",
        L"Unknown Number"
    }; 

    class CCJGW_NumberRule
    {
    public:
        CCJGW_NumberRule(void);
        ~CCJGW_NumberRule(void);
        //! 获取IMEI校验值  strIMEI:待校验的IMEI
        static bool GetIMEIChecksum(const std::wstring& strIMEI,std::wstring& strIMEIChecksum);
    private:
        static bool CheckIMEIIsNumber(const std::wstring& strIMEI);
    };
}