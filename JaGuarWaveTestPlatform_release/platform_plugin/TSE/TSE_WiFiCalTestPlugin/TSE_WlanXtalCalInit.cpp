#include "StdAfx.h"
#include "TSE_WlanXtalCalInit.h"

namespace JGW
{
    CTSE_WlanXtalCalInit::CTSE_WlanXtalCalInit(void) : mdCapOut(0x40),mdXtalTargetPPM(-15),mdXtalTolerancePPM(2),mdXtalCapDelta(64),mdXtalCalOtpSaveOption(XTAL_OTP_DISABLE)
    {
    }


    CTSE_WlanXtalCalInit::~CTSE_WlanXtalCalInit(void)
    {
    }

    const wchar_t* CTSE_WlanXtalCalInit::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_WlanXtalCalInit\",\
                \"TestName\": \"Wlan Xtal Cal Init\",\
                \"XtalTargetPPM\": \"xtal cal target in PPM\",\
                \"XtalTolerancePPM\": \"xtal cal tolerance in PPM\",\
                \"XtalCapDelta\": \"xtal cal cap delta offset\",\
                \"XtalCalOtpSaveOption\": \"XTAL_OTP_DISABLE=0;XTAL_OTP_ENABLE=1\",\
                \"CapOut\": \"init capOut only apply for QCA6174, others are hardcoded for 0x40\"\
                }";
    }

    bool CTSE_WlanXtalCalInit::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"XtalTargetPPM",strParamName))
        {
            mdXtalTargetPPM = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"XtalTolerancePPM",strParamName))
        {
            mdXtalTolerancePPM = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"XtalCapDelta",strParamName))
        {
            mdXtalCapDelta = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"XtalCalOtpSaveOption",strParamName))
        {
            mdXtalCalOtpSaveOption = (WLAN_XTAL_STORAGE_OTP)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"CapOut",strParamName))
        {
            wchar_t* str = NULL;
            mdCapOut = wcstol(strParamValue,&str,16);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_WlanXtalCalInit::TSE_Run()
    {
        PHONE->mXtalTolerancePPM = mdXtalTolerancePPM;
        PHONE->mXtalTargetPPM = mdXtalTargetPPM;

        return PHONE->FTM_WLAN_Atheros_xtal_CAL_INIT(mdXtalTargetPPM,mdXtalTolerancePPM,mdXtalCapDelta,mdXtalCalOtpSaveOption,1000,2000);
    }
}
