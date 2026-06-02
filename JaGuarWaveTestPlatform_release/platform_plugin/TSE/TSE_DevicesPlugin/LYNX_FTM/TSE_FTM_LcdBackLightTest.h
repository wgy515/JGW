#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_FTM_LcdBackLightTest : public CTSE_TestBase
    {
    public:
        CTSE_FTM_LcdBackLightTest(void);
        ~CTSE_FTM_LcdBackLightTest(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Run();
    private:
        HWND mMainHwnd;
        std::wstring mstrTip;
    };
}