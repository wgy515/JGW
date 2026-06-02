#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_ConnectDiagPhone : public CTSE_TestBase
    {
    public:
        CTSE_ConnectDiagPhone(void);
        ~CTSE_ConnectDiagPhone(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
        bool TSE_Init();
    private:
        bool mbUseQpst;
        size_t mnTimeOutSec;
        std::wstring mstrPortEnv;
        std::wstring mstrResourceContextEnv;
    };

}

