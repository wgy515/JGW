#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_DisConnectSSHSCP : public CTSE_TestBase
    {
    public:
        CTSE_DisConnectSSHSCP(void);
        ~CTSE_DisConnectSSHSCP(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_Run();
    };
}


