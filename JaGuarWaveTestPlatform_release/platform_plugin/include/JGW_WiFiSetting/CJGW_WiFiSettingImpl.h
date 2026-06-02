#pragma once
#include "JGW_WIFISETTING_Define.h"

namespace JGW
{
    enum E_WIFI_SETTING_OPER_ID
    {
        E_UNKNOW_WIFI = -1,
        E_SCAN_WIFI = 0,
        E_CONNECT_WIFI = 1,
        E_DISCONNECT_WIFI = 2,
        E_CONNECT_PUMAS = 3, //! 连接PUMAS 
        E_COMMAND_PUMAS = 4, //! 发送PUMAS命令
        E_DISCONNECT_PUMAS = 5, //! pumas
        E_CHECKCONNECT_WIFI = 6,
        //! ShellExecute 运行一个外部程序
        E_SHELL_EXECUTE = 7
    };


    class CCJGW_WiFiSettingImpl
    {
    public:
        //!
        virtual std::wstring& GetErrorMsg() = 0;
        //! 
        virtual void SetWiFiServerInfo(const std::string& strSocketIP,const size_t socketPort,const size_t timeOutSec,const std::string& strSourceIP = "") = 0;
        //!
        virtual void GetWiFiServerInfo(std::string& strSocketIP,size_t& socketPort) = 0;
        //! 
        virtual bool ScanWiFi(std::vector<std::string>& vstrSSID) = 0;
        //! 
        virtual bool ConnectWiFi(const std::string& strSSID,const std::string& strPassword) = 0;
        //!
        virtual void DisConnectWiFi() = 0;
        //!
        virtual void DisConnectSocket() = 0;
        //!
        virtual bool CheckConnectWiFi(const std::string& strSSID,const std::string& strPassword,const std::string& strWlanGateWay,const std::string& strLocalAddress,const std::string& strInterfaceDescription,std::string& strWlanAddress) = 0;
        /*********************puma **********************/
        //!
        virtual bool ConnectPerasoDevice() = 0;
        //!
        virtual bool WriteReadPerasoCommad(const std::string& strWrite,std::string& strRead) = 0;
        //!
        virtual void DisConnectPerasoDevice() = 0;
        //! 
        virtual bool ShellExecuteCommad(const std::string& command) = 0;
    };

#ifdef __cplusplus
    extern "C" 
    {
#endif
        JGW_WIFISETTING_API CCJGW_WiFiSettingImpl* GetWiFiSetting();

        JGW_WIFISETTING_API void DestroyWiFiSetting();

#ifdef __cplusplus
    };
#endif
}