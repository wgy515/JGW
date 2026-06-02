#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_FTMExecuteSingleTest : public CTSE_TestBase
    {
    public:
        CTSE_FTMExecuteSingleTest(void);
        ~CTSE_FTMExecuteSingleTest(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Run();
    private:
        std::string mstrTestCase;
        std::wstring mstrTip;
    };
}