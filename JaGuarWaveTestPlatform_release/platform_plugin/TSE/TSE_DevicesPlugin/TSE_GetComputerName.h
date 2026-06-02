#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_GetComputerName : public CTSE_TestBase
    {
    public:
        CTSE_GetComputerName(void);
        ~CTSE_GetComputerName(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring mstrComputerName;
        std::wstring mstrComputerNameEnvironment;
    };
}


