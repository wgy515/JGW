#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_ResetPeraso : public CTSE_TestBase
    {
    public:
        CTSE_ResetPeraso(void);
        ~CTSE_ResetPeraso(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_Run();
    };
}