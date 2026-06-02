#include "StdAfx.h"
#include "TSE_DisConnectWiFiSocket.h"
#include <JGW_WiFiSetting/CJGW_WiFiSettingImpl.h>
namespace JGW
{
    CTSE_DisConnectWiFiSocket::CTSE_DisConnectWiFiSocket(void)
    {
    }


    CTSE_DisConnectWiFiSocket::~CTSE_DisConnectWiFiSocket(void)
    {
    }

    const wchar_t* CTSE_DisConnectWiFiSocket::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_DisConnectWiFiSocket\",\
                \"TestName\": \"¶Ï¿ªWIFI SocketÁ¬½Ó\",\
                }";
    }

    bool CTSE_DisConnectWiFiSocket::TSE_Run()
    {
        GetWiFiSetting()->DisConnectSocket();
        return true;
    }
}

