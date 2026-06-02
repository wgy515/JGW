#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_RKReadOtpData : public CTSE_TestBase
    {
    public:
        CTSE_RKReadOtpData(void);
        ~CTSE_RKReadOtpData(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        //! cpuid type
        int mnOtpType;
    };
}


