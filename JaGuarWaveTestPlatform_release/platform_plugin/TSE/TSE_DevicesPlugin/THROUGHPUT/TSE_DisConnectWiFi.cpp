#include "StdAfx.h"
#include "TSE_DisConnectWiFi.h"
#include <JGW_WiFiSetting/CJGW_WiFiSettingImpl.h>

namespace JGW
{
    CTSE_DisConnectWiFi::CTSE_DisConnectWiFi(void) : mnTimeOut(10)
    {
    }


    CTSE_DisConnectWiFi::~CTSE_DisConnectWiFi(void)
    {
    }

    const wchar_t* CTSE_DisConnectWiFi::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_DisConnectWiFi\",\
                \"TestName\": \"断开WIFI连接\",\
                \"HostIPEnvironment\": \"WIFI连接服务主机IP地址环境变量\",\
                \"SocketPortEnvironment\": \"WIFI连接服务SOCKET端口环境变量\"\
                }";
    }

    bool CTSE_DisConnectWiFi::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"HostIPEnvironment"))
        {
            mstrHostIPEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SocketPortEnvironment"))
        {
            mstrSocketPortEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_DisConnectWiFi::TSE_Run()
    {
        if (!mstrHostIPEnvironment.empty())
        {
            std::string strSocketIP = JGW_W2A(GetGlobalEnvironment()->GetString(mstrHostIPEnvironment));
            size_t socketPort = _ttoi(GetGlobalEnvironment()->GetString(mstrSocketPortEnvironment).c_str());
            GetWiFiSetting()->SetWiFiServerInfo(strSocketIP,socketPort,mnTimeOut);
        }
        GetWiFiSetting()->DisConnectWiFi();
        return true;
    }

    bool CTSE_DisConnectWiFi::TSE_Exit()
    {
        GetWiFiSetting()->DisConnectSocket();
        return true;
    }
}