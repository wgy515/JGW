#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_SSHPlugin/CJGW_SSHSFTPV2.h>
namespace JGW
{
    class CTSE_ConnectSFTPV2 : public CTSE_TestBase
    {
    public:
        CTSE_ConnectSFTPV2(void);
        ~CTSE_ConnectSFTPV2(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        size_t mnSFTPPort;
        std::wstring mstrSourceAddrEnvironment;
        std::wstring mstrHostEnvironment;
        std::wstring mstrUserNameEnvironment;
        std::wstring mstrPasswordEnvironment;
        std::wstring mstrSaveSFTPInstanceEnvironment;
        CCJGW_SSHSFTPV2 mcSSHSFTPV2;
    };
}


