#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_ConnectSSHSCP : public CTSE_TestBase
    {
    public:
        CTSE_ConnectSSHSCP(void);
        ~CTSE_ConnectSSHSCP(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        size_t mnSSHPort;
        std::wstring mstrSourceAddrEnvironment;
        std::wstring mstrHostEnvironment;
        std::wstring mstrUserNameEnvironment;
        std::wstring mstrPasswordEnvironment;
    };
}


