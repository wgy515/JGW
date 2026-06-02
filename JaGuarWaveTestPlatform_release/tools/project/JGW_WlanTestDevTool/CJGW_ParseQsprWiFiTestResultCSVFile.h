#pragma once
#include <CJGW_WiFiTestResultRecord.h>

namespace JGW
{
    typedef enum
    {
        E_UNKNOW_WIFI_TEST_METRIC = -1,
        E_chAvgTxPowerDbm_WIFI_TEST_METRIC = 0,
        E_chEvmDb_WIFI_TEST_METRIC = 1,
        E_chAmpImbDb_WIFI_TEST_METRIC = 2,
        E_chPhaseImbDeg_WIFI_TEST_METRIC,
        E_chCarrierFreqErrorPpm_WIFI_TEST_METRIC,
        E_chLoLeakageDbc_WIFI_TEST_METRIC,
        E_PER_WIFI_TEST_METRIC,
        E_PerPower_WIFI_TEST_METRIC,
        E_chSpectralMaskMarginsPass_WIFI_TEST_METRIC
    } E_WIFI_TEST_METRIC;

    class CCJGW_ParseQsprWiFiTestResultCSVFile
    {
    public:
        CCJGW_ParseQsprWiFiTestResultCSVFile(void);
        ~CCJGW_ParseQsprWiFiTestResultCSVFile(void);
    public:
        bool LoadQsprWiFiTestResultCSVFile(const std::string& strWiFiCSVPath,CCJGW_WiFiTestResultRecord& wifiTestResultRecord);
        std::wstring GetErrorMessage();
    private:
        E_WIFI_TEST_METRIC IsParseWiFiParam(const std::string& strParameterName);
        LPS_TRX_TEST_DATA ParseLogHeader(const std::string& strHeader,CCJGW_WiFiTestResultRecord& wifiTestResultRecord);
    private:
        WLAN_UniversalPhyID GetPhyIDToPhyIDText(const std::string& strPhyIDText);
        WLAN_UniversalRateBW GetRateBWToRateBWText(const std::string& strRateBWText);
        WLAN_CHAIN_MASK2 GetWlanChainMask2TpChainMaskText(const std::string& strChainMaskText,std::string& strChainMaskDes);
        WLAN_UniversalDataRate GetDataRateToDataRateText(const std::string& strDataRate);
    private:
        char mszNewLine;
        std::wstring mstrErrorMessage;
    };

}

