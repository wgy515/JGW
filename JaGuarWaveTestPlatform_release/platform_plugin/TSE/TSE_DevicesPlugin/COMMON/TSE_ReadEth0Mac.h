#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_ReadEth0Mac : public CTSE_TestBase
    {
    public:
        CTSE_ReadEth0Mac(void);
        ~CTSE_ReadEth0Mac(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Run();
    private:
        size_t mNumberLen;
        std::wstring mstrETH0EnvironmentInfo;
        std::wstring mstrEnvironment;
    };
}