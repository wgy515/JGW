#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "../TSE_DevicesGlobalResource.h"
namespace JGW
{
    class CTSE_SettingQualcommMode : public CTSE_TestBase
    {
    public:
        CTSE_SettingQualcommMode(void);
        ~CTSE_SettingQualcommMode(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_Run();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
    private:
        mode_enum_type meQualcommMode;
    };

}

