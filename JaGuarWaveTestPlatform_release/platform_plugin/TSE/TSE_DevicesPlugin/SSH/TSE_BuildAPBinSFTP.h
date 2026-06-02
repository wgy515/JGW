#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_BuildAPBinSFTP : public CTSE_TestBase
    {
    public:
        CTSE_BuildAPBinSFTP(void);
        ~CTSE_BuildAPBinSFTP(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring mstrSNEnvironment;
        std::wstring mstrMACEnvironment;
        std::wstring mstrSSIDEnvironment;
        std::wstring mstrPasswortEnvironment;
        std::wstring mstrAPBinFilePathEnvironment;
    };
}