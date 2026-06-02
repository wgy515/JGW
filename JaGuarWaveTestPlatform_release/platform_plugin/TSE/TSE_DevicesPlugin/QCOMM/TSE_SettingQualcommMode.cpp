#include "StdAfx.h"
#include "TSE_SettingQualcommMode.h"

namespace JGW
{
    CTSE_SettingQualcommMode::CTSE_SettingQualcommMode(void) : meQualcommMode(MODE_ONLINE_F)
    {
    }


    CTSE_SettingQualcommMode::~CTSE_SettingQualcommMode(void)
    {
    }
    /*
    MODE_OFFLINE_A_F = 0,    //!<' Go to offline analog
    MODE_OFFLINE_D_F = 1,    //!<' Go to offline digital
    MODE_RESET_F = 2,        //!<' Reset. Only exit from offline
    MODE_FTM_F = 3,          //!<' FTM mode
    MODE_ONLINE_F = 4,       //!<' Go to Online
    MODE_LPM_F = 5,          //!<' Low Power Mode (if supported)
    MODE_POWER_OFF_F = 6,    //!<' Power off (if supported)
    */
    const wchar_t* CTSE_SettingQualcommMode::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_SettingQualcommMode\",\
                \"TestName\": \"Setting Qualcomm Mode\",\
                \"QualcommMode\": \"0:Go to offline analog,1:Go to offline digital,2:Reset. Only exit from offline,3:FTM mode,4:Go to Online,5:Low Power Mode (if supported),6:Power off (if supported)\"\
                }";
    }

    bool CTSE_SettingQualcommMode::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;

        if (TSE_PARAM_NAME_EQUAL(_T("QualcommMode"),strParamName))
        {
            meQualcommMode = (mode_enum_type)_ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_SettingQualcommMode::TSE_Run()
    {
        for (int i = 0;i < 4;i ++)
        {
            if (CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->SetDeviceMode(meQualcommMode))
            {
                return true;
            }
            Sleep(1000);
        }
        return false;
    }
}
