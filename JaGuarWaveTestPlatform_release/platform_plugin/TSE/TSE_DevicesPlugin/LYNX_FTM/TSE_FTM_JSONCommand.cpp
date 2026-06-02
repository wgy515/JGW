#include "StdAfx.h"
#include "TSE_FTM_JSONCommand.h"
#include "TSE_FTM_Define.h"

namespace JGW
{
    std::string CTSE_FTM_JSONCommand::GetVersionJson()
    {
        static std::string strVersionJson;
        if (!strVersionJson.empty()) return strVersionJson;
        JGW_FormatString(strVersionJson,"{\"requestCode\":%d}",E_REQ_VERSION_CODE);
        return strVersionJson;
    }

    /*
    getVersionInfo ---> requestMsg = {"requestCode":1}
    onSocketResponse ---> responseVo = SocketBaseResponseVo{ reqCode : 1 resultCode : 0 result : null responseMsg : null}
    keysTest ---> requestMsg = {"type":1,"requestCode":2}
    widgetSelected ---> currentTabItem = Keys
    onSocketResponse ---> responseVo = SocketBaseResponseVo{ reqCode : 2 resultCode : 1 result : {"event":"down","keyType":"PowerKey"} responseMsg : null}
    onSocketResponse ---> responseVo = SocketBaseResponseVo{ reqCode : 2 resultCode : 1 result : {"event":"up","keyType":"PowerKey"} responseMsg : null}

    */

    std::string CTSE_FTM_JSONCommand::GetKesyJson(E_TSE_FTM_KEY_STATUS eKeyStatus)
    {
        std::string strKeyJson;
        JGW_FormatString(strKeyJson,"{\"type\":%d,\"requestCode\":%d}",eKeyStatus,E_REQ_KEYS_CODE);
        return strKeyJson;
    }

    std::string CTSE_FTM_JSONCommand::GetLcdColorJson(size_t color)
    {
        std::string strLcdColorTestJson;
        JGW_FormatString(strLcdColorTestJson,"{\"color\":%d,\"requestCode\":%d}",color,E_REQ_LCD_COLOR_CODE);
        return strLcdColorTestJson;
    }

    std::string CTSE_FTM_JSONCommand::GetLcdBackLightJson()
    {
        static std::string strLcdBackLightJson;
        if (!strLcdBackLightJson.empty()) return strLcdBackLightJson;
        JGW_FormatString(strLcdBackLightJson,"{\"requestCode\":%d}",E_REQ_LCD_BACK_LIGHT_CODE);
        return strLcdBackLightJson;
    }

    std::string CTSE_FTM_JSONCommand::GetMemoryJson()
    {
        static std::string strMemoryJson;
        if (!strMemoryJson.empty()) return strMemoryJson;
        JGW_FormatString(strMemoryJson,"{\"requestCode\":%d}",E_REQ_MEMORY_CARD_CODE);
        return strMemoryJson;
    }

    std::string CTSE_FTM_JSONCommand::GetSimJson()
    {
        static std::string strSimJson;
        if (!strSimJson.empty()) return strSimJson;
        JGW_FormatString(strSimJson,"{\"requestCode\":%d}",E_REQ_SIM_CARD_INFO_CODE);
        return strSimJson;
    }

    std::string CTSE_FTM_JSONCommand::GetBreathingLedJson(int group)
    {
        std::string strBreathingLedJson;
        if (!strBreathingLedJson.empty()) return strBreathingLedJson;
        JGW_FormatString(strBreathingLedJson,"{\"group\":%d,\"requestCode\":7}",group);
        return strBreathingLedJson;
    }

    std::string CTSE_FTM_JSONCommand::GetLedRGBJson()
    {
        static std::string strJson;
        if (!strJson.empty()) return strJson;
        JGW_FormatString(strJson,"{\"requestCode\":%d}",E_REQ_RGB_LED_CODE);
        return strJson;
    }

    std::string CTSE_FTM_JSONCommand::GetHandoverWiFiModeJson(int wifiMode)
    {
        std::string strJson;
        if (!strJson.empty()) return strJson;
        JGW_FormatString(strJson,"{\"type\":0,\"ant\":0,\"mode\":%d,\"requestCode\":%d}",wifiMode,E_REQ_WIFI_MODE);
        return strJson;
    }

    std::string CTSE_FTM_JSONCommand::GetWiFiScanAPJson()
    {
        static std::string strJson;
        if (!strJson.empty()) return strJson;
        JGW_FormatString(strJson,"{\"requestCode\":%d}",E_REQ_SCAN_AP_CODE);
        return strJson;
    }

    std::string CTSE_FTM_JSONCommand::GetHandoverADModeJson(int adMode)
    {
        std::string strJson;
        if (!strJson.empty()) return strJson;
        JGW_FormatString(strJson,"{\"type\":1,\"mode\":%d,\"requestCode\":10}",adMode,E_REQ_WIGIG_CODE);
        return strJson;
    }

    std::string CTSE_FTM_JSONCommand::GetADScanAPJson()
    {
        return "{\"type\":2,\"mode\":0,\"requestCode\":18}";
    }

    std::string CTSE_FTM_JSONCommand::GetQC3InfoJson()
    {
        return "{\"requestCode\":13}";
    }

    std::string CTSE_FTM_JSONCommand::GetOTGTeseJson()
    {
        return "{\"requestCode\":19}";
    }
}

