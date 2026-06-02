#include "StdAfx.h"
#include "TSE_LANIPConfig.h"
#include "CJGW_LANConfigDlg.h"
#include "CJGW_LANSuiteConfigXML.h"

namespace JGW
{
	CTSE_LANIPConfig::CTSE_LANIPConfig(void)
	{
	}


	CTSE_LANIPConfig::~CTSE_LANIPConfig(void)
	{
	}

    const wchar_t* CTSE_LANIPConfig::TSE_GetParamDescription()
    {
        return L"{\"RealName\":\"CTSE_LANIPConfig\",\"TestName\":\"Lynx DHCP配置命令生成\",\"LANIPAddress\":\"LYNX LAN口IP地址配置\",\"BroadcastAddress\":\"LAN口的网关配置\",\"EnvironmentName\":\"LYNX基于LAN口IP和网关，手动配置DHCP命令\",\"mnOperID\":\"自动或者手动配置 1 : 手动配置DHCP,0 : 自动配置DHCP\"}";
    }

	bool CTSE_LANIPConfig::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
	{
		if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
		if (TSE_PARAM_NAME_EQUAL(strParamName,L"LANIPAddress"))
		{
			mstrLANIPAddress = strParamValue;
		}
		else if (TSE_PARAM_NAME_EQUAL(strParamName,L"BroadcastAddress"))
		{
			mstrBroadcastIPAddress = strParamValue;
		}
		else if (TSE_PARAM_NAME_EQUAL(strParamName,L"EnvironmentName"))
		{
			mstrEnvironmentName = strParamValue;
		}
		else if (TSE_PARAM_NAME_EQUAL(strParamName,L"OperID"))
		{
			mnOperID = _ttoi(strParamValue);
		}
		else
		{
			return false;
		}
		return true;
	}

	bool CTSE_LANIPConfig::TSE_Run()
	{
		CCJGW_LANConfigDlg lanConfigDlg(GetGlobalMainHwnd(),mnOperID,mstrLANIPAddress,mstrBroadcastIPAddress);
		if (IDOK == lanConfigDlg.DoModal())
		{
			std::wstring strEnvironmentValue = L"adb shell \"/system/bin/dhcptool eth0\"";
			if (1 == mnOperID)
			{
				if (!mstrBroadcastIPAddress.empty())
					JGW_FormatWString(strEnvironmentValue,L"adb shell ifconfig eth0 %s broadcast %s",mstrLANIPAddress.c_str(),mstrBroadcastIPAddress.c_str());
				else
					JGW_FormatWString(strEnvironmentValue,L"adb shell ifconfig eth0 %s",mstrLANIPAddress.c_str());
			}
			GetGlobalEnvironment()->PutString(mstrEnvironmentName,strEnvironmentValue);
			CCJGW_LANSuiteConfigXML lanconfig;
			lanconfig.SaveLanSuiteConfig(JGW_W2A(GetGlobalEnvironment()->GetString(TSE_GLOBAL_ENVIRONMENT_CONFIG_FILE_PATH)),mnOperID
				,mstrLANIPAddress,mstrBroadcastIPAddress,mstrEnvironmentName,strEnvironmentValue);
		}
		return false;
	}
}
