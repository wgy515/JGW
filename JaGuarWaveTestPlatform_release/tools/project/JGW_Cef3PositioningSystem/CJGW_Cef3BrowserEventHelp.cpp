#include "StdAfx.h"
#include "CJGW_Cef3BrowserEventHelp.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
	CCJGW_Cef3BrowserEventHelp::CCJGW_Cef3BrowserEventHelp(void) : mnBrowserID(-1),mbIsShowEcompassNotCalibratePrompt(false)
	{
	}


	CCJGW_Cef3BrowserEventHelp::~CCJGW_Cef3BrowserEventHelp(void)
	{
	}

	int CCJGW_Cef3BrowserEventHelp::GetBrowserID()
	{
		return mnBrowserID;
	}

	void CCJGW_Cef3BrowserEventHelp::OnBrowserCreate(int nBrowserID,const std::wstring& strUrl)
	{
		mnBrowserID = nBrowserID;
	}

	void CCJGW_Cef3BrowserEventHelp::SetLocalGpsInfoToMap(double fLatitude,double fLongitude,float fDistance)
	{
		std::wstring strJsCode;//! set_local_gps_info(22.664027, 113.794979
		JGW_FormatWString(strJsCode,L"set_local_gps_info(%.6lf,%.6lf,%.6f)",fLatitude,fLongitude,fDistance);
		ExecuteJaveScript(mnBrowserID,strJsCode);
	}

	void CCJGW_Cef3BrowserEventHelp::SetRemoteGpsInfoToMap(double fLatitude,double fLongitude)
	{
		std::wstring strJsCode;
		JGW_FormatWString(strJsCode,L"set_remote_gps_info(%.6lf,%.6lf)",fLatitude,fLongitude);
		ExecuteJaveScript(mnBrowserID,strJsCode);
	}

	void CCJGW_Cef3BrowserEventHelp::SetLocalYawInfoToMap(float fCalYaw,float fYaw,float fTrueYaw)
	{
		std::wstring strJsCode;
		JGW_FormatWString(strJsCode,L"set_local_yaw_info(%.6f,%.6f,%.6f)",fCalYaw,fYaw,fTrueYaw);
		ExecuteJaveScript(mnBrowserID,strJsCode);
	}

	void CCJGW_Cef3BrowserEventHelp::SetRemoteYawInfoToMap(float fCalYaw,float fYaw,float fTrueYaw)
	{
		std::wstring strJsCode;
		JGW_FormatWString(strJsCode,L"set_remote_yaw_info(%.6f,%.6f,%.6f)",fCalYaw,fYaw,fTrueYaw);
		ExecuteJaveScript(mnBrowserID,strJsCode);
	}

	void CCJGW_Cef3BrowserEventHelp::SetLocalGpsDesToMap(int nPositionFixIndicator, int nSatellitesUsed,float fHdop, const std::string& strMslAltitude, float fGeoidalSeparation)
	{
		std::wstring strJsCode;
		JGW_FormatWString(strJsCode,L"set_local_gps_des_param(%d,%d,%.6f,'%s',%.6f)",nPositionFixIndicator, nSatellitesUsed,fHdop, JGW_A2W(strMslAltitude,CP_UTF8).c_str(), fGeoidalSeparation);
		ExecuteJaveScript(mnBrowserID,strJsCode);
	}

	void CCJGW_Cef3BrowserEventHelp::AddConnectMessageToMap(const std::string& strMessage)
	{
		std::wstring strJsCode;
		JGW_FormatWString(strJsCode,L"AddConnectMessage('%s')",JGW_A2W(strMessage,CP_UTF8).c_str());
		ExecuteJaveScript(mnBrowserID,strJsCode);
	}

	void CCJGW_Cef3BrowserEventHelp::OnReConnectDXCSToMap()
	{
		std::wstring strJsCode;
		JGW_FormatWString(strJsCode,L"showBleScanModal()");
		ExecuteJaveScript(mnBrowserID,strJsCode);
	}

	void CCJGW_Cef3BrowserEventHelp::CloseConnectModalToMap()
	{
		std::wstring strJsCode;
		JGW_FormatWString(strJsCode,L"onClickCloseBleConnectModal()");
		ExecuteJaveScript(mnBrowserID,strJsCode);
	}

	void CCJGW_Cef3BrowserEventHelp::AddBLEDeviceInfoToMap(const std::string& strBleName,const std::string& strBleID)
	{
		std::wstring strJsCode;
		JGW_FormatWString(strJsCode,L"addBLEDeviceInfo('%s','%s')",JGW_A2W(strBleName,CP_UTF8).c_str(),JGW_A2W(strBleID,CP_UTF8).c_str());
		ExecuteJaveScript(mnBrowserID,strJsCode);
	}

	void CCJGW_Cef3BrowserEventHelp::UpdateAmapWebView()
	{
		std::wstring strJsCode;
		JGW_FormatWString(strJsCode,L"autoUpdateMap()");
		ExecuteJaveScript(mnBrowserID,strJsCode);
	}

	void CCJGW_Cef3BrowserEventHelp::ShowEcompassNotCalibratePromptToMap()
	{
		if (!mbIsShowEcompassNotCalibratePrompt) 
		{
			std::wstring strJsCode;
			JGW_FormatWString(strJsCode,L"showEcompassNotCalibratePrompt()");
			ExecuteJaveScript(mnBrowserID,strJsCode);
			mbIsShowEcompassNotCalibratePrompt = true;
		}
	}

	void CCJGW_Cef3BrowserEventHelp::DismissEcompassNotCalibratePromptToMap()
	{
		if (mbIsShowEcompassNotCalibratePrompt)
		{
			std::wstring strJsCode;
			JGW_FormatWString(strJsCode,L"dismissEcompassNotCalibratePrompt()");
			ExecuteJaveScript(mnBrowserID,strJsCode);
			mbIsShowEcompassNotCalibratePrompt = false;
		}	
	}

	void CCJGW_Cef3BrowserEventHelp::DisableFalseReScanBleBtnToMap()
	{
		std::wstring strJsCode;
		JGW_FormatWString(strJsCode,L"disableFalesReScanBleBtn()");
		ExecuteJaveScript(mnBrowserID,strJsCode);
	}
}
