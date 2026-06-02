#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_GetFileSFTPV2 : public CTSE_TestBase
    {
    public:
        CTSE_GetFileSFTPV2(void);
        ~CTSE_GetFileSFTPV2(void);
    private:    
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        unsigned long mnFTPFlags;
        long mnFTPMode;
        std::wstring mstrSaveLocalFilePathCommand;
        std::wstring mstrRemoteFilePathCommand;
        std::wstring mstrSFTPInstanceEnvironment;
    };
}

