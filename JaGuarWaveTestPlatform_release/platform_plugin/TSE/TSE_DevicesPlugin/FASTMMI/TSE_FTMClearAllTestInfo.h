#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_FTMClearAllTestInfo : public CTSE_TestBase
    {
    public:
        CTSE_FTMClearAllTestInfo(void);
        ~CTSE_FTMClearAllTestInfo(void);

    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_Run();
    };
}