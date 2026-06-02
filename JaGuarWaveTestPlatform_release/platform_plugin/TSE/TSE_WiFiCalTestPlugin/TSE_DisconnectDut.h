#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>


namespace JGW
{
    class CTSE_DisconnectDut : public CTSE_TestBase
    {
    public:
        CTSE_DisconnectDut(void);
        ~CTSE_DisconnectDut(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_Run();
    };
}


