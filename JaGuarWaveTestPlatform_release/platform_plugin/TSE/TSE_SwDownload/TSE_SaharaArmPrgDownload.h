#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_SaharaArmPrgDownload : public CTSE_TestBase
    {
    public:
        CTSE_SaharaArmPrgDownload(void);
        ~CTSE_SaharaArmPrgDownload(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring mstrResourceContextEnv;
        std::wstring mstrArmPrgDownloadFilePathCmd;
    };

}

