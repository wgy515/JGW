#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_DisConnectSFTPV2 : public CTSE_TestBase
    {
    public:
        CTSE_DisConnectSFTPV2(void);
        ~CTSE_DisConnectSFTPV2(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring mstrSFTPInstanceEnvironment;
    };
}
