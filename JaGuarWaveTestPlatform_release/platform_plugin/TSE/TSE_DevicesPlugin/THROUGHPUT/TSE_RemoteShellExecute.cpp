#include "StdAfx.h"
#include "TSE_RemoteShellExecute.h"
#include <JGW_WiFiSetting/CJGW_WiFiSettingImpl.h>

namespace JGW
{
    CTSE_RemoteShellExecute::CTSE_RemoteShellExecute(void) : mstrSocketPortEnvironment(L"3001"),mstrHostIPEnvironment(L"127.0.0.1")
    {
    }


    CTSE_RemoteShellExecute::~CTSE_RemoteShellExecute(void)
    {
    }

    const wchar_t* CTSE_RemoteShellExecute::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_RemoteShellExecute\",\
                \"TestName\": \"远程运行一个外部程序)\",\
                \"HostIPEnvironment\": \"WIFI服务主机IP地址环境变量\",\
                \"SocketPortEnvironment\": \"WIFI连接服务SOCKET端口环境变量\",\
                \"Command\": \"外部程序命令\",\
                \"TimeOut\": \"WIFI连接服务超时时间设置\"\
                }";
    }

    bool CTSE_RemoteShellExecute::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"HostIPEnvironment"))
        {
            mstrHostIPEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SourceIPEnvironment"))
        {
            mstrSourceIPEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SocketPortEnvironment"))
        {
            mstrSocketPortEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"Command"))
        {
            mstrCommand = JGW_W2A(strParamValue);
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

    bool CTSE_RemoteShellExecute::TSE_Run()
    {
        //! 初始化
        std::string strSocketIP = JGW_W2A(GetGlobalEnvironment()->GetString(mstrHostIPEnvironment));
        std::string strSourceIP = JGW_W2A(GetGlobalEnvironment()->GetString(mstrSourceIPEnvironment));
        size_t socketPort = _ttoi(GetGlobalEnvironment()->GetString(mstrSocketPortEnvironment).c_str());
        GetWiFiSetting()->SetWiFiServerInfo(strSocketIP,socketPort,mnTimeOut,strSourceIP);

        std::wstring strCommand = JGW_A2W(mstrCommand);
        GetEnvironmentCommandString(strCommand);

        if (!GetWiFiSetting()->ShellExecuteCommad(JGW_W2A(strCommand)))
        {
            Sleep(mnIntervalTimeMS);
            LogE_F(L"远程运行外部程序失败(%s)",GetWiFiSetting()->GetErrorMsg().c_str());
            return false;
        }
        return true;
    }
}

