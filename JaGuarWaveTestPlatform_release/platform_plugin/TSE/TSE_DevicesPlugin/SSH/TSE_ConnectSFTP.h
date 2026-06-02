#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_ConnectSFTP : public CTSE_TestBase
    {
    public:
        CTSE_ConnectSFTP(void);
        ~CTSE_ConnectSFTP(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        size_t mnSSHPort;
        unsigned long mnSFTPFlags;
        long mnSFTPMode;
        std::wstring mstrSFTPPathEnvironment;
        std::wstring mstrSourceAddrEnvironment;
        std::wstring mstrHostEnvironment;
        std::wstring mstrUserNameEnvironment;
        std::wstring mstrPasswordEnvironment;
    };
}