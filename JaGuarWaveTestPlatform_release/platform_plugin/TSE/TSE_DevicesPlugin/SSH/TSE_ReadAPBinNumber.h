#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_ReadAPBinNumber : public CTSE_TestBase
    {
    public:
        CTSE_ReadAPBinNumber(void);
        ~CTSE_ReadAPBinNumber(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring mstrSNEnvironment;
        std::wstring mstrMACEnvironment;
        std::wstring mstrSSIDEnvironment;
        std::wstring mstrPasswordEnvironment;
        std::wstring mstrAPBinFilePathEnvironment;
    };
}