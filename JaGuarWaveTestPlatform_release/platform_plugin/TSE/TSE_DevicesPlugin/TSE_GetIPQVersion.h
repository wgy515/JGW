#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_GetIPQVersion : public CTSE_TestBase
    {
    public:
        CTSE_GetIPQVersion(void);
        ~CTSE_GetIPQVersion(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring mstrSrcVersionEnvironment;
        std::wstring mstrSrcVersionStartStr;
        std::wstring mstrSrcVersionEndStr;
        std::wstring mstrVersionFormat;
        std::wstring mstrDestVersionEnvironment;
    };
}


