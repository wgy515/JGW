#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "CJGW_TSEIntrument.h"
namespace JGW
{
    class CTSE_OpenInstrumentVISA : public CTSE_TestBase
    {
    public:
        CTSE_OpenInstrumentVISA(void);
        ~CTSE_OpenInstrumentVISA(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Run();
    private:
        std::wstring mstrVisaAddress;
        std::wstring mstrVisaAddressEnvionment;
        std::wstring mstrInstrumentEnvironment;
        CCJGW_TSEIntrument mcTSEIntrument;
    };
}