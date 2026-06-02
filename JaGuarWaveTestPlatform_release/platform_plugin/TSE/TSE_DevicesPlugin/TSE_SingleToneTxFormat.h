#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_SingleToneTxFormat : public CTSE_TestBase
    {
    public:
        CTSE_SingleToneTxFormat(void);
        ~CTSE_SingleToneTxFormat(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring GetSingleToneTxCommandFormat(unsigned int channel,short txPower);
    private:
        short muTxPower;
        unsigned int muTxChannel;
        std::wstring mstrSaveSingleToneTxEnvironment;
    };
}


