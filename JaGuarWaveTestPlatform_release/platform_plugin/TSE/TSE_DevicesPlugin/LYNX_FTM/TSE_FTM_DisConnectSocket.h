#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_FTM_DisConnectSocket : public CTSE_TestBase
    {
    public:
        CTSE_FTM_DisConnectSocket(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_Run(void);
    };
}