#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_FTMClearTestResultFile : public CTSE_TestBase
    {
    public:
        CTSE_FTMClearTestResultFile(void);
        ~CTSE_FTMClearTestResultFile(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_Run();
    };
}


