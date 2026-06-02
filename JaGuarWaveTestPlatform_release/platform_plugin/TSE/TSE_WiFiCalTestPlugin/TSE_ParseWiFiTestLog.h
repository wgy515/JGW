#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "CJGW_WiFiTestResultRecord.h"
namespace JGW
{
    class CTSE_ParseWiFiTestLog : public CTSE_TestBase
    {
    public:
        CTSE_ParseWiFiTestLog(void);
        ~CTSE_ParseWiFiTestLog(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool ParseWiFiTestLogFile(const std::string& strWiFiTestLogFilePath);
        LPS_TRX_TEST_DATA ParseLogHeader(const std::string& strHeader);
        void ParseLog(const std::string& strLog,LPS_TRX_TEST_DATA psTRxTestData);
    private:
        WLAN_UniversalPhyID GetPhyIDToPhyIDText(const std::string& strPhyIDText);
        WLAN_UniversalRateBW GetRateBWToRateBWText(const std::string& strRateBWText);
        WLAN_CHAIN_MASK2 GetWlanChainMask2TpChainMaskText(const std::string& strChainMaskText,std::string& strChainMaskDes);
        WLAN_UniversalDataRate GetDataRateToDataRateText(const std::string& strDataRate);
    private:
        char mszNewLine;
        std::wstring mstrRichEditTestLogFileEnvironment;
        std::wstring mstrNewLineChar;
        std::wstring mstrWiFiTestResultRecordEnvironment;
        CCJGW_WiFiTestResultRecord* mpWiFiTestResultRecord;
    };
}


