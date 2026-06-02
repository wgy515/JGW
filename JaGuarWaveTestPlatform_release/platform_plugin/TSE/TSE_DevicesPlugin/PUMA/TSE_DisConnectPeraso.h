#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_DisConnectPeraso : public CTSE_TestBase
    {
    public:
        CTSE_DisConnectPeraso(void);
        ~CTSE_DisConnectPeraso(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_Run();
    };
}