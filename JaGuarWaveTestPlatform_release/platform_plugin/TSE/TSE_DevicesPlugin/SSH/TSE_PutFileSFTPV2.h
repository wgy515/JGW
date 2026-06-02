#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_PutFileSFTPV2 : public CTSE_TestBase
    {
    public:
        CTSE_PutFileSFTPV2(void);
        ~CTSE_PutFileSFTPV2(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        unsigned long mnFTPFlags;
        long mnFTPMode;
        std::wstring mstrLocalFilePathCommand;
        std::wstring mstrRemoteFilePathCommand;
        std::wstring mstrSFTPInstanceEnvironment;
    };
}
