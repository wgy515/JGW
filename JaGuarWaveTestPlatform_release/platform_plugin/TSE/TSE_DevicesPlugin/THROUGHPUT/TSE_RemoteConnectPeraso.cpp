#include "StdAfx.h"
#include "TSE_RemoteConnectPeraso.h"
#include <JGW_WiFiSetting/CJGW_WiFiSettingImpl.h>

namespace JGW
{
    CTSE_RemoteConnectPeraso::CTSE_RemoteConnectPeraso(void)
    {
    }


    CTSE_RemoteConnectPeraso::~CTSE_RemoteConnectPeraso(void)
    {
    }

    const wchar_t* CTSE_RemoteConnectPeraso::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_RemoteConnectPeraso\",\
                \"TestName\": \"远程连接peraso设备(AP)\",\
                \"HostIPEnvironment\": \"WIFI服务主机IP地址环境变量\",\
                \"SocketPortEnvironment\": \"WIFI连接服务SOCKET端口环境变量\",\
                \"TimeOut\": \"WIFI连接服务超时时间设置\"\
                }";
    }

    bool CTSE_RemoteConnectPeraso::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TimeOut"))
        {
            mnTimeOut = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_RemoteConnectPeraso::TSE_Run()
    {
        //! 初始化
        std::string strSocketIP = JGW_W2A(GetGlobalEnvironment()->GetString(mstrHostIPEnvironment));
        size_t socketPort = _ttoi(GetGlobalEnvironment()->GetString(mstrSocketPortEnvironment).c_str());
        GetWiFiSetting()->SetWiFiServerInfo(strSocketIP,socketPort,mnTimeOut);

        if (!GetWiFiSetting()->ConnectPerasoDevice())
        {
            Sleep(mnIntervalTimeMS);
            LogE_F("Peraso 连接失败(%s)",GetWiFiSetting()->GetErrorMsg().c_str());
            return false;
        }
        return true;
    }
}
