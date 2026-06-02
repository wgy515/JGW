#include "StdAfx.h"
#include "TSE_IPQ807xWlanXtalCalInit2.h"
#include "CJGW_IPQ807xPhone.h"
namespace JGW
{
    CTSE_IPQ807xWlanXtalCalInit2::CTSE_IPQ807xWlanXtalCalInit2(void) : mdXtalTargetPPM(0),mdXtalTolerancePPM(1),meXtalCalOtpSaveOption(XTAL_OTP_DISABLE)
    {
    }


    CTSE_IPQ807xWlanXtalCalInit2::~CTSE_IPQ807xWlanXtalCalInit2(void)
    {
    }

    const wchar_t* CTSE_IPQ807xWlanXtalCalInit2::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_IPQ807xWlanXtalCalInit2::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if(TSE_PARAM_NAME_EQUAL(_T("PhyID"),strParamName))
        {
            mePhyId = (PhyID)_ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("XtalTargetPPM"),strParamName))
        {
            mdXtalTargetPPM = _ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("XtalTolerancePPM"),strParamName))
        {
            mdXtalTolerancePPM = _ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("XtalCalOtpSaveOption"),strParamName))
        {
            meXtalCalOtpSaveOption = (WLAN_XTAL_STORAGE_OTP)_ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_IPQ807xWlanXtalCalInit2::TSE_Run()
    {
        return CCJGW_IPQ807xPhone::GetIPQ807xPhone()->WlanXtalCalInit2(mePhyId,mdXtalTargetPPM,mdXtalTolerancePPM,meXtalCalOtpSaveOption); 
    }
}
