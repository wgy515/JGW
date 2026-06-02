#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_ContinousRxCommandFormat : public CTSE_TestBase
    {
    public:
        CTSE_ContinousRxCommandFormat(void);
        ~CTSE_ContinousRxCommandFormat(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring GetContinousRxCommandFormat(unsigned int rx_earfcn,short rx_power);
    private:
        short muRxPower;
        unsigned int muRxChannel;
        std::wstring mstrSaveContinousRxCommandEnvironment;
    };
}


