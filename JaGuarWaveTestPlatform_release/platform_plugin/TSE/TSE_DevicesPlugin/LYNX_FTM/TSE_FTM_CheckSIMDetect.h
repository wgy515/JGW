#pragma once
#include "TSE_FTM_TestBase.h"

namespace JGW
{
    class CTSE_FTM_CheckSIMDetect : public CTSE_FTM_TestBase
    {
    public:
        CTSE_FTM_CheckSIMDetect(void);
        ~CTSE_FTM_CheckSIMDetect(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_Run();
    };
}