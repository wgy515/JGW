#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_ComparisonEnvironment : public CTSE_TestBase
    {
    public:
        CTSE_ComparisonEnvironment(void);
        ~CTSE_ComparisonEnvironment(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Run(void);
    private:
        bool mbIgnoreString;
        std::wstring mstrIgnoreString;
        std::wstring mstrFormat;
        std::wstring mstrSrcEnvironmentName;
        std::wstring mstrDestEnvironmentName;
    };
}