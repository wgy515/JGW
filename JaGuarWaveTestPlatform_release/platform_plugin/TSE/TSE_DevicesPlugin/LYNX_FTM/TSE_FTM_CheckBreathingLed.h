#pragma once
#include "TSE_FTM_TestBase.h"

namespace JGW
{
    class CTSE_FTM_CheckBreathingLed : public CTSE_FTM_TestBase
    {
    public:
        CTSE_FTM_CheckBreathingLed(void);
        ~CTSE_FTM_CheckBreathingLed(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Run();

        bool TSE_Exit();
    private:
        int mnGroupID;
        HWND mMainHwnd;
        std::wstring mstrTip;
    };
}