#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_SettingFTM : public CTSE_TestBase
    {
    public:
        CTSE_SettingFTM(void);
        ~CTSE_SettingFTM(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_Run();
    };
}
