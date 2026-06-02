#pragma once
#include <string>
namespace JGW
{
    class CCJGW_WiFiServerJsonCommand
    {
    public:
        CCJGW_WiFiServerJsonCommand(void);
        ~CCJGW_WiFiServerJsonCommand(void);
    public:
        static std::string GetScanWiFiJson();

        static std::string GetConnectWiFiJson(const std::string& strSSID,const std::string& strPassword);

        static std::string GetDisConnectWiFiJson();

        static std::string GetConnectPerasoDeviceJson();

        static std::string GetDisConnectPerasoDeviceJson();

        static std::string GetWriteReadPerasoDeviceJson(const std::string& strCommand);

        static std::string GetCheckConnectWiFiJson(const std::string& strSSID,const std::string& strPassword,const std::string& strWlanGateWay,const std::string& strLocalAddress,const std::string& strInterfaceDescription);

        static std::string GetShellExecuteJson(const std::string& strCommand);
    };
}