#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_DisConnectSSH : public CTSE_TestBase
    {
    public:
        CTSE_DisConnectSSH(void);
        ~CTSE_DisConnectSSH(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_Run();
    };
}