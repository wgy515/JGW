#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_WriterQcomSN : public CTSE_TestBase
    {
    public:
        CTSE_WriterQcomSN();
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Run();
    private:
        size_t mSNLength;
        std::wstring mstrSNPrefix;
        std::wstring mstrSNEnvironment;
    };
}