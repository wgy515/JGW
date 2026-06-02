#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_ReadDeviceInfoBin : public CTSE_TestBase
    {
    public:
        CTSE_ReadDeviceInfoBin(void);
        ~CTSE_ReadDeviceInfoBin(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring mstrDeviceInfoEnvironment;
        std::vector<std::string> mvEntryNames;
        std::vector<std::wstring> mvEntryValueEnv;
    };
}


