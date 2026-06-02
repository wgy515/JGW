#include "StdAfx.h"
#include "TSE_ConnectWiFi.h"
#include "../TSE_DevicesGlobalResource.h"
#include <JGW_WiFiSetting/CJGW_WiFiSettingImpl.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
namespace JGW
{
    CTSE_ConnectWiFi::CTSE_ConnectWiFi(void) : mnTimeOut(10)
    {
        mnIntervalTimeMS = 1000;
    }


    CTSE_ConnectWiFi::~CTSE_ConnectWiFi(void)
    {
        DestroyWiFiSetting();
    }

    const wchar_t* CTSE_ConnectWiFi::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ConnectWiFi\",\
                \"TestName\": \"WIFI连接\",\
                \"SSIDEnvironment\": \"WIFI SSID环境变量\",\
                \"PASSWORDEnvironment\": \"WIFI PASSWORD环境变量\",\
                \"HostIPEnvironment\": \"WIFI连接服务主机IP地址环境变量\",\
                \"SocketPortEnvironment\": \"WIFI连接服务SOCKET端口环境变量\",\
                \"TimeOut\": \"WIFI连接超时时间设置\"\
                }";
    }

    bool CTSE_ConnectWiFi::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"SSIDEnvironment"))
        {
            mstrSSIDEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PASSWORDEnvironment"))
        {
            mstrPasswordEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"HostIPEnvironment"))
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

    bool CTSE_ConnectWiFi::CheckScanSSIDExists(const std::string& strSSID)
    {
        std::vector<std::string> vstrSSID;
        if (!GetWiFiSetting()->ScanWiFi(vstrSSID))
        {
            LogE_F("扫描WIFI 失败!!!(%s)",GetWiFiSetting()->GetErrorMsg().c_str());
            Sleep(mnIntervalTimeMS);
            return false;
        }

        bool bFind = false;
        for (std::vector<std::string>::iterator it = vstrSSID.begin();
            it != vstrSSID.end();
            ++it)
        {
            if (NULL == it[0].compare(strSSID)) {bFind = true;break ;}
        }

        if (!bFind)
        {
            LogE_F("当前SSID(%s)未扫描到",strSSID.c_str());
            Sleep(mnIntervalTimeMS);
            return false;
        }
        return bFind;
    }

    bool CTSE_ConnectWiFi::TSE_Run()
    {
        std::string strSocketIP = JGW_W2A(GetGlobalEnvironment()->GetString(mstrHostIPEnvironment));
        size_t socketPort = _ttoi(GetGlobalEnvironment()->GetString(mstrSocketPortEnvironment).c_str());
        std::string strSSID = JGW_W2A(GetGlobalEnvironment()->GetString(mstrSSIDEnvironment));
        std::string strPassword = JGW_W2A(GetGlobalEnvironment()->GetString(mstrPasswordEnvironment));
        GetWiFiSetting()->SetWiFiServerInfo(strSocketIP,socketPort,mnTimeOut);

        jgw_timer jt;
        while (mnTimeOut >= jt.elapsed())
        {
            if (!CheckScanSSIDExists(strSSID))
            {
                Sleep(mnIntervalTimeMS);
                continue;
            }
            if (GetWiFiSetting()->ConnectWiFi(strSSID,strPassword)) return true;
             LogE_F("WIFI 连接失败(%s)",GetWiFiSetting()->GetErrorMsg().c_str());
             Sleep(mnIntervalTimeMS);
        }
        
        return false;
    }

    bool CTSE_ConnectWiFi::TSE_Exit()
    {
        GetWiFiSetting()->DisConnectSocket();
        return true;
    }
}