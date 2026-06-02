#include "StdAfx.h"
#include "TSE_FTM_HandoverWiFiMode.h"
#include "CJGW_FTM_SocketResponceVo.h"
namespace JGW
{
    CTSE_FTM_HandoverWiFiMode::CTSE_FTM_HandoverWiFiMode(void) : mWiFiMode(WIFI_STAT_MODE),mb11AD(false)
    {
    }


    CTSE_FTM_HandoverWiFiMode::~CTSE_FTM_HandoverWiFiMode(void)
    {
    }

    const wchar_t* CTSE_FTM_HandoverWiFiMode::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTM_HandoverWiFiMode\",\
                \"TestName\": \"ÇÐ»»WIFIÄ£Ê½(LYNX FTM)\",\
                \"WiFIMode\": \"1:AP MODE 2 :STA MODE\",\
                \"IsAD\": \"0 : 11bgn 1: 11AD Ä¬ÈÏ0\"\
                }";
    }

    bool CTSE_FTM_HandoverWiFiMode::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_FTM_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"WiFIMode"))
        {
            mWiFiMode = _ttoi(strParamValue);
            if (mWiFiMode > WIFI_STAT_MODE||mWiFiMode < WIFI_AP_MODE) mWiFiMode = WIFI_STAT_MODE;
        } 
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IsAD"))
        {
            mb11AD = (0 != _ttoi(strParamValue));           
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_FTM_HandoverWiFiMode::TSE_Run()
    {
        std::string strJsonCommand;
        if (!mb11AD) strJsonCommand = CTSE_FTM_JSONCommand::GetHandoverWiFiModeJson(mWiFiMode);
        else strJsonCommand = CTSE_FTM_JSONCommand::GetHandoverADModeJson(mWiFiMode);
        CCJGW_FTM_JsonSocket::WriteJsonSocket(strJsonCommand);
        return true;
    }
}