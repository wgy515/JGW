#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_CheckAdapterSpeed : public CTSE_TestBase
    {
    public:
        CTSE_CheckAdapterSpeed(void);
        ~CTSE_CheckAdapterSpeed(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        DWORD mdwSpeedUnitMbps;
        //! The speed of the interface in bits per second
        __int64 mdwbitsPerSecondSpeed;
        std::wstring mstrFindNetDescEnv;
        std::wstring mstrIPAddressEnv;
    };
}


