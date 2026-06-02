#include "StdAfx.h"
#include "CJGW_WiFiServerJsonCommand.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "CJGW_FTM_Json.h"
#include <JGW_WiFiSetting/CJGW_WiFiSettingImpl.h>
namespace JGW
{
    CCJGW_WiFiServerJsonCommand::CCJGW_WiFiServerJsonCommand(void)
    {
    }


    CCJGW_WiFiServerJsonCommand::~CCJGW_WiFiServerJsonCommand(void)
    {
    }

    std::string CCJGW_WiFiServerJsonCommand::GetScanWiFiJson()
    {
        return "{\"requestType\":0}";
    }

    std::string CCJGW_WiFiServerJsonCommand::GetConnectWiFiJson(const std::string& strSSID,const std::string& strPassword)
    {
        std::string strJson;
        JGW_FormatString(strJson,"{\"requestType\":1,\"ssidname\":\"%s\",\"passwrod\":\"%s\"}",strSSID.c_str(),strPassword.c_str());
        return strJson;
    }

    std::string CCJGW_WiFiServerJsonCommand::GetDisConnectWiFiJson()
    {
        return "{\"requestType\":2}";
    }

    std::string CCJGW_WiFiServerJsonCommand::GetConnectPerasoDeviceJson()
    {
        return "{\"requestType\":3}";
    }

    std::string CCJGW_WiFiServerJsonCommand::GetWriteReadPerasoDeviceJson(const std::string& strCommand)
    {
        boost::property_tree::ptree ptWriteReadPerasoJson;
        ptWriteReadPerasoJson.put("requestType",4);
        ptWriteReadPerasoJson.put("command",strCommand);
        std::stringstream sstream;
        boost::property_tree::json_parser::write_json(sstream, ptWriteReadPerasoJson);
        std::string strRequestParams = sstream.str();
        return strRequestParams;
    }


    std::string CCJGW_WiFiServerJsonCommand::GetDisConnectPerasoDeviceJson()
    {
        return "{\"requestType\":5}";
    }

    std::string CCJGW_WiFiServerJsonCommand::GetCheckConnectWiFiJson(const std::string& strSSID,const std::string& strPassword,const std::string& strWlanGateWay,const std::string& strLocalAddress,const std::string& strInterfaceDescription)
    {
        std::string strJson;
        JGW_FormatString(strJson,"{\"requestType\":6,\"ssidname\":\"%s\",\"passwrod\":\"%s\",\"wlangateway\":\"%s\",\"localaddress\":\"%s\",\"InterfaceDescription\":\"%s\"}",strSSID.c_str(),strPassword.c_str(),strWlanGateWay.c_str(),strLocalAddress.c_str(),strInterfaceDescription.c_str());
        return strJson;
    }

    std::string CCJGW_WiFiServerJsonCommand::GetShellExecuteJson(const std::string& strCommand)
    {
        boost::property_tree::ptree ptWriteReadPerasoJson;
        ptWriteReadPerasoJson.put("requestType",E_SHELL_EXECUTE);
        ptWriteReadPerasoJson.put("command",strCommand);
        std::stringstream sstream;
        boost::property_tree::json_parser::write_json(sstream, ptWriteReadPerasoJson);
        std::string strRequestParams = sstream.str();
        return strRequestParams;
    }
}