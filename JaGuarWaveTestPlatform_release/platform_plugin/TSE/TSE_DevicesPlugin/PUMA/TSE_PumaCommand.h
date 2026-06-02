#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_PumaCommand : public CTSE_TestBase
    {
    public:
        CTSE_PumaCommand(void);
        ~CTSE_PumaCommand(void);

    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Run();

    private:
        int mnTimeOut;
        std::wstring mstrCommand;
        std::wstring mstrResult;
        std::wstring mstrReadResultEnvironment;
    };
}