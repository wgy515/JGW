#pragma once
#include "JGW_WLAN_DEFINE.h"
#include <wlanapi.h>
#include <map>
#include <string>

namespace JGW
{
	//! typedef void (*WlanPrintLogCallbackFunc)(const char* strLog);

    class CCJGW_WlanInterfaceImpl
    {
    public:
        //! 打开WLAN 设备句柄
        virtual bool OpenWlanHandle() = 0;
        //! 获取当前主机所有WLAN无线网卡接口描述符
        virtual bool GetWlanInterfaceDescriptions(std::map<std::wstring,std::wstring>& mapInterfaceDescriptions) = 0;
        //! 
        virtual std::wstring GetWlanInterfaceDescriptionToEnumKey() = 0;
        //! 遍历WLAN 网口设备接口,并且指定WLAN无线网卡接口描述符
        virtual bool EnumWlanInterface(const std::wstring& strWlanInterfaceDescription) = 0;
        //! 关闭无线网络连接
        virtual bool DisWlanConnect() = 0;
        //! 获取有效网络集合 
        virtual bool GetVisibleNetworkList(std::map<std::string,int>& mapBss) = 0;

        //! 获取有效网络集合 
        virtual bool GetScanNetworkList(std::map<std::string,int>& mapBss) = 0;

        //! 设置WLAN连接Profile文件
        virtual bool SetWlanConnectProfile(const std::wstring& strWlanConnectProfile) = 0;
        //! 删除WLAN连接Profile文件
        virtual bool DeleteWlanConnectProfile(const std::wstring& strWlanConnectProfileName) = 0;
        //! 连接WLAN
        virtual bool ConnectWlanToSSID(const std::string& strSSID,const std::wstring& strWlanConnectProfileName,DOT11_BSS_TYPE dot11BSSType = dot11_BSS_type_infrastructure) = 0;
        //! 连接开放WLAN
        virtual bool ConnectOpenWlanToSSID(const std::string& strSSID,DOT11_BSS_TYPE dot11BSSType = dot11_BSS_type_infrastructure) = 0;
        //! 检查WLAN是否连接成功
        virtual bool CheckWlanIsConnected(const std::string& strSSID) = 0;
        //! 获取SSID集合  SSID,dot11BssType
        //! bool GetWlanBssList(std::map<std::string,int>& mapBss,int scan_time = 1000);
        //! 查找指定SSID
        virtual bool FindWlanSSIDToWlanBSSList(const std::string& strSSID,DOT11_BSS_TYPE dot11BSSType = dot11_BSS_type_infrastructure) = 0;
        //! 
        virtual void CloseWlanHandle() = 0;
		//! virtual void 
    };

#ifdef __cplusplus
    extern "C" 
    {
#endif
        JGW_WLAN_API CCJGW_WlanInterfaceImpl* GetWiFiInterfaceImpl();

#ifdef __cplusplus
    };
#endif
}