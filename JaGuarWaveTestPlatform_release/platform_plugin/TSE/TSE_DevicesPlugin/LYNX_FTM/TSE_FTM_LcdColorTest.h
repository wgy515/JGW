#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_FTM_LcdColorTest : public CTSE_TestBase
    {
    public:
        CTSE_FTM_LcdColorTest(void);
        ~CTSE_FTM_LcdColorTest(void);

    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Run();
    private:
        size_t mdwColor;
        HWND mMainHwnd;
        std::wstring mstrTip;
    };
}