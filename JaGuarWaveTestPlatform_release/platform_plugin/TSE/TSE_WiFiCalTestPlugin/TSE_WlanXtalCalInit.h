#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_QMSLFuncPlugin/CCJGW_WCNTestDefine.h>

namespace JGW
{
    class CTSE_WlanXtalCalInit : public CTSE_TestBase
    {
    public:
        CTSE_WlanXtalCalInit(void);
        ~CTSE_WlanXtalCalInit(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        int mdXtalTargetPPM;
        int mdXtalTolerancePPM;
        int mdXtalCapDelta;
        //! init capOut only apply for QCA6174, others are hardcoded for 0x40
        int mdCapOut;
        //! XTAL_OTP_DISABLE=0;XTAL_OTP_ENABLE=1
        WLAN_XTAL_STORAGE_OTP mdXtalCalOtpSaveOption;
    };
}


