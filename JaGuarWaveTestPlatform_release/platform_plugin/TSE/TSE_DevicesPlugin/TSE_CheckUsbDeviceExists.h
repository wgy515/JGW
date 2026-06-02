#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_CheckUsbDeviceExists : public CTSE_TestBase
    {
    public:
        CTSE_CheckUsbDeviceExists(void);
        ~CTSE_CheckUsbDeviceExists(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        size_t mnCheckTimeOut;
        std::wstring mstrUsbVid;
        std::wstring mstrUsbPid;
        std::wstring mstrUsbServiceName;
        std::vector<std::wstring> mvstrRegKeys;
        std::vector<std::wstring> mvstrFindRegValues;
    };
}
