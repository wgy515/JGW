#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_CheckThroughputTestResult : public CTSE_TestBase
    {
    public:
        CTSE_CheckThroughputTestResult(void);
        ~CTSE_CheckThroughputTestResult(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring mstrTestRangeEnvironment;
        std::wstring mstrTestTimeEnvironment;
        std::wstring mstrLogEnvironment;
        std::wstring mstrlocationString;
    };
}