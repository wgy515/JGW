#include "StdAfx.h"
#include "TSE_RemoteDisConnectPeraso.h"
#include <JGW_WiFiSetting/CJGW_WiFiSettingImpl.h>
namespace JGW
{
    CTSE_RemoteDisConnectPeraso::CTSE_RemoteDisConnectPeraso(void)
    {
    }


    CTSE_RemoteDisConnectPeraso::~CTSE_RemoteDisConnectPeraso(void)
    {
    }

    const wchar_t* CTSE_RemoteDisConnectPeraso::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_RemoteDisConnectPeraso\",\
                \"TestName\": \"断开远程连接peraso设备(AP)\"\
                }";
    }

    bool CTSE_RemoteDisConnectPeraso::TSE_Run()
    {
        GetWiFiSetting()->DisConnectPerasoDevice();
        GetWiFiSetting()->DisConnectSocket();
        return true;
    }
}

