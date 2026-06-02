#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    typedef struct
    {
        std::wstring mstrTestTime;
        std::wstring mstrTestRange;
        std::wstring mstrWiFiScanIPAddress;
        std::wstring mstrIperfServerIPAddress;
        std::wstring mstrTestTimeEnvironment;
        std::wstring mstrTestRangeEnvironment;
        std::wstring mstrWiFiScanIPAddressEnvironment;
        std::wstring mstrIperfServerIPAddressEnvironment;
        std::wstring mstrIperfCommand;
        std::wstring mstrIperfCommandEnvironment;
        std::wstring mstrIperfCommandTimeOut;
        std::wstring mstrIperfCommandTimeOutEnvironment;
    }S_THROUGHPUT_CONFIG;

    class CTSE_ThroughputConfig : public CTSE_TestBase
    {
    public:
        CTSE_ThroughputConfig(void);
        ~CTSE_ThroughputConfig(void);
    private:
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
        const wchar_t* TSE_GetParamDescription();
    private:
        S_THROUGHPUT_CONFIG msThroughputConfig;
    };
}