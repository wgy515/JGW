#include "StdAfx.h"
#include "TSE_CheckConnectWiFi.h"
#include <JGW_WiFiSetting/CJGW_WiFiSettingImpl.h>
namespace JGW
{
    CTSE_CheckConnectWiFi::CTSE_CheckConnectWiFi(void) : mstrInterfaceDescription(L"Peraso"),mbConnectFailResetPeraso(false)
    {
    }


    CTSE_CheckConnectWiFi::~CTSE_CheckConnectWiFi(void)
    {
    }

    const wchar_t* CTSE_CheckConnectWiFi::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_CheckConnectWiFi\",\
                \"TestName\": \"检查WIFI连接\",\
                \"SSIDEnvironment\": \"WIFI SSID环境变量\",\
                \"PASSWORDEnvironment\": \"WIFI PASSWORD环境变量\",\
                \"HostIPEnvironment\": \"WIFI连接服务主机IP地址环境变量\",\
                \"SocketPortEnvironment\": \"WIFI连接服务SOCKET端口环境变量，默认3001端口\",\
                \"WlanGatewayEnvironment\": \"WIFI网关环境变量\",\
                \"InterfaceDescription\": \"WIFI网口接口描述，默认配置为Peraso\",\
                \"WlanIPAddressEnvironment\": \"WIFI连接后IP地址环境变量\",\
                \"ConnectFailResetPeraso\": \"连接失败后重启peraso设备\"\
                }";
    }

    bool CTSE_CheckConnectWiFi::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"SSIDEnvironment"))
        {
            mstrSSIDEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PASSWORDEnvironment"))
        {
            mstrPASSWORDEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"HostIPEnvironment"))
        {
            mstrHostIPEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SocketPortEnvironment"))
        {
            mstrSocketPortEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"WlanGatewayEnvironment"))
        {
            mstrWlanGatewayEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"InterfaceDescription"))
        {
            mstrInterfaceDescription = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"WlanIPAddressEnvironment"))
        {
            mstrWlanIPAddressEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ConnectFailResetPeraso"))
        {
            mbConnectFailResetPeraso = (1 == _ttoi(strParamValue));
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_CheckConnectWiFi::TSE_Run()
    {
        std::string strSocketIP = JGW_W2A(GetGlobalEnvironment()->GetString(mstrHostIPEnvironment));
        size_t socketPort = _ttoi(GetGlobalEnvironment()->GetString(mstrSocketPortEnvironment).c_str());
        std::string strSSID = JGW_W2A(GetGlobalEnvironment()->GetString(mstrSSIDEnvironment));
        std::string strPassword = JGW_W2A(GetGlobalEnvironment()->GetString(mstrPASSWORDEnvironment));
        std::string strWlanGateway = GetGlobalEnvironment()->GetAnsiString(mstrWlanGatewayEnvironment);
        //! std::string strRemoteIntranetIPAddress = GetGlobalEnvironment()->GetAnsiString(mstrWlanIPAddressEnvironment);
        std::string strInterfaceDescrip = JGW_W2A(mstrInterfaceDescription);/*GetGlobalEnvironment()->GetAnsiString(mstrInterfaceDescription);*/
        std::string strWlanAddress;
        GetWiFiSetting()->SetWiFiServerInfo(strSocketIP,socketPort,120);

        if (!GetWiFiSetting()->CheckConnectWiFi(strSSID,strPassword,strWlanGateway,strSocketIP,strInterfaceDescrip,strWlanAddress))
        {
            LogE_F("WIFI 连接失败(%s)",GetWiFiSetting()->GetErrorMsg().c_str());
            GetWiFiSetting()->SetWiFiServerInfo(strSocketIP,socketPort,10);
            return false;
        }
        GetGlobalEnvironment()->PutAnsiString(mstrWlanIPAddressEnvironment,strWlanAddress);
        GetWiFiSetting()->SetWiFiServerInfo(strSocketIP,socketPort,10);
        return true;
    }

    bool CTSE_CheckConnectWiFi::TSE_Exit()
    {
        GetWiFiSetting()->DisConnectSocket();
        return true;
    }
}