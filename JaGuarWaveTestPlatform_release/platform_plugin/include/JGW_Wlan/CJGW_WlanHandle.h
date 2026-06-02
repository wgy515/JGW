#pragma once
#include <string>
#include <map>
#include "JGW_WLAN_DEFINE.h"

namespace JGW
{
	typedef std::wstring __InterfaceDescription;
	typedef std::wstring __GUID;

	class JGW_WLAN_CLASS CCJGW_WlanHandle
	{
	public:
		CCJGW_WlanHandle(void); //! ==> 发送GUID
		~CCJGW_WlanHandle(void);

	public:
		//! KEY ->   InterfaceDescription 获取注册的所有网卡描述符以及对应的GUID Value 
		bool GetWlanInterfaceDescriptions(std::map<std::wstring,std::wstring>& mapInterfaceDescriptions);
		//! 网卡GUID To 描述符关键字
		std::wstring FindWalnGUIDToInterfaceDescriptionKeyword(const std::wstring& strInterfaceDesKeyword);
		//! 获取SSID集合  SSID,dot11BssType
		bool GetWlanBssList(const std::wstring& strGUID,std::map<std::wstring,int>& mapBss,int scan_time = 1000);
		//!  strWlanProfile == SSID
		bool SetWlanProfile(const std::wstring& strGUID,const std::wstring& strWlanProfile);
		//!
		bool DeleteWlanProfile(const std::wstring& strGUID,const std::wstring& strWlanProfile);
		//!
		bool ConnectWlan(const std::wstring& strGUID,const std::wstring& strSSID,const std::wstring& strProfileName,int dot11BssType);
		//!
		bool ConnectOpenWlan(const std::wstring& strGUID,const std::wstring& strSSID,int dot11BssType,int nWlanConnectMode = 2);
		//!
		bool DisConnectWlan(const std::wstring& strGUID); 
		//!
		const wchar_t* GetWlanErrorMsg();
	private:
		//! 
		DWORD OpenHandleAndCheckVersion(PHANDLE phClient);
		//! 
		const wchar_t* GetInterfaceStateString(__in int wlanInterfaceState);
    public:
        void SetConnectedStatus(DWORD status);
        //!
        VOID PrintReason(__in DWORD reason);
		//! 
		void PrintErrorMsg(__in const wchar_t* strCommand,__in DWORD dwError);
    private:
		//!
		bool WlanScan(const std::wstring& strGUID);
	private:
		//! 
		std::wstring mstrGUID;
		//!
		std::wstring mstrErrorMsg;
        //! 
        DWORD mdwConnectStatus;
	};
}