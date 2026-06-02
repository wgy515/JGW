#pragma once
#include "TSE_FTM_TestBase.h"

namespace JGW
{
    class CTSE_FTM_CheckLEDRGB : public CTSE_FTM_TestBase
    {
    public:
        CTSE_FTM_CheckLEDRGB(void);
        ~CTSE_FTM_CheckLEDRGB(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Run();
    private:
        std::wstring mstrTip;
    };
}