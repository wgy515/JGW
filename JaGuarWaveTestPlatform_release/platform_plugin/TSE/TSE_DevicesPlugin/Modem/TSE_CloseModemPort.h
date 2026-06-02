#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_CloseModemPort : public CTSE_TestBase
    {
    public:
        CTSE_CloseModemPort(void);
        ~CTSE_CloseModemPort(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_Run();
    };
}

