#pragma once
#include <string>
#include <map>
#include "CJGW_WlanInterfaceImpl.h"
namespace JGW
{
    class CCJGW_WlanInterface : public CCJGW_WlanInterfaceImpl
    {
    public:
        CCJGW_WlanInterface(const std::wstring& strWlanInterfaceDescriptionKey = L"Peraso");
        ~CCJGW_WlanInterface(void);
    public:
        //! 打开WLAN 设备句柄
        bool OpenWlanHandle();
        //! 获取当前主机所有WLAN无线网卡接口描述符
        bool GetWlanInterfaceDescriptions(std::map<std::wstring,std::wstring>& mapInterfaceDescriptions);
        //! 遍历WLAN 网口设备接口,并且指定WLAN无线网卡接口描述符
        bool EnumWlanInterface(const std::wstring& strWlanInterfaceDescriptionKey);
        //! 关闭无线网络连接
        bool DisWlanConnect();

        bool GetScanNetworkList(std::map<std::string,int>& mapBss);
		//! 获取有效网络集合 
		bool GetVisibleNetworkList(std::map<std::string,int>& mapBss);
		//! 设置WLAN连接Profile文件
		bool SetWlanConnectProfile(const std::wstring& strWlanConnectProfile);
		//! 删除WLAN连接Profile文件
		bool DeleteWlanConnectProfile(const std::wstring& strWlanConnectProfileName);
		//! 连接WLAN
		bool ConnectWlanToSSID(const std::string& strSSID,const std::wstring& strWlanConnectProfileName,DOT11_BSS_TYPE dot11BSSType = dot11_BSS_type_infrastructure);
        //!
        bool ConnectOpenWlanToSSID(const std::string& strSSID,DOT11_BSS_TYPE dot11BSSType = dot11_BSS_type_infrastructure);
		//! 检查WLAN是否连接成功
		bool CheckWlanIsConnected(const std::string& strSSID);
        //! 获取SSID集合  SSID,dot11BssType
        //! bool GetWlanBssList(std::map<std::string,int>& mapBss,int scan_time = 1000);
        //! 查找指定SSID
        bool FindWlanSSIDToWlanBSSList(const std::string& strSSID,DOT11_BSS_TYPE dot11BSSType = dot11_BSS_type_infrastructure);
        //! 
        void CloseWlanHandle();
        //! 根据Enum KEY
        std::wstring GetWlanInterfaceDescriptionToEnumKey();
    private:
        //! 扫描WIFI
        bool ScanClientWlan(const std::string& strSSID);
		//!
		void PrintErrorMsg(const wchar_t* strCommand,DWORD dwError);
    private:
        bool mbGuidIsValid;
        //!
        HANDLE mhClientWlanHanle;
        //! WLAN 接口的GUID
        GUID mWlanInterfaceGuid;
        //! WLAN接口描述字符串 关键字
        std::wstring mstrWlanInterfaceDescriptionKey;
        //!
        std::wstring mstrWlanInterfaceDescription;
    };
}