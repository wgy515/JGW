#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_QMSLFuncPlugin/CCJGW_WCNTestDefine.h>
namespace JGW
{
    class CTSE_IPQ807xWlanXtalCalInit2 : public CTSE_TestBase
    {
    public:
        CTSE_IPQ807xWlanXtalCalInit2(void);
        ~CTSE_IPQ807xWlanXtalCalInit2(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        PhyID mePhyId;
        int mdXtalTargetPPM;
        size_t mdXtalTolerancePPM;
        WLAN_XTAL_STORAGE_OTP meXtalCalOtpSaveOption;
    };
}


