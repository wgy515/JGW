#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_DownloadPDBToPeraso : public CTSE_TestBase
    {
    public:
        CTSE_DownloadPDBToPeraso(void);
        ~CTSE_DownloadPDBToPeraso(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool mbDownloadToFile;
        bool mbInitPDBInfo;
        bool mbDownloadOTP;
        bool mbDownloadConfig;
        std::wstring mstrPDBHeaderFilePathEnvironment;
        std::wstring mstrPDBBobyFilePathEnvironment;
    };
}