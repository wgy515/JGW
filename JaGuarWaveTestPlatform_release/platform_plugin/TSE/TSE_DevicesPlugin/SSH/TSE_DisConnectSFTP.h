#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_DisConnectSFTP : public CTSE_TestBase
    {
    public:
        CTSE_DisConnectSFTP(void);
        ~CTSE_DisConnectSFTP(void);

    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_Run();
    };
}