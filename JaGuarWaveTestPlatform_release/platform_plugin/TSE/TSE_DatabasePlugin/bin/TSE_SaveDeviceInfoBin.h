#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_SaveDeviceInfoBin : public CTSE_TestBase
    {
    public:
        CTSE_SaveDeviceInfoBin(void);
        ~CTSE_SaveDeviceInfoBin(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool mbIsEncrypt;
        std::wstring mstrSaveDeviceInfoFilePathEnv;
        std::wstring mstrDeviceInfoEnvironment;
    };
}

