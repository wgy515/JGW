#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_DisConnectWiFiSocket : public CTSE_TestBase
    {
    public:
        CTSE_DisConnectWiFiSocket(void);
        ~CTSE_DisConnectWiFiSocket(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_Run();
    };
}


