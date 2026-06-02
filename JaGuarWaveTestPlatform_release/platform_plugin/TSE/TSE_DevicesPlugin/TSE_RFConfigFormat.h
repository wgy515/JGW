#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_RFConfigFormat : public CTSE_TestBase
    {
    public:
        CTSE_RFConfigFormat(void);
        ~CTSE_RFConfigFormat(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
        bool TSE_Init();
    private:
        std::wstring mstrSaveSecondKeyEnvironment;
        std::wstring mstrSecondKeyEnvironment;
        std::wstring mstrPrimaryKeyEnvironment;
    };
}

