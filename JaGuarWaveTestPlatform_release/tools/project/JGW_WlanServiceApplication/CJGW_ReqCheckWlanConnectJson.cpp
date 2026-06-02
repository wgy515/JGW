#include "StdAfx.h"
#include "CJGW_ReqCheckWlanConnectJson.h"

namespace JGW
{
    CCJGW_ReqCheckWlanConnectJson::CCJGW_ReqCheckWlanConnectJson(void)
    {
    }


    CCJGW_ReqCheckWlanConnectJson::~CCJGW_ReqCheckWlanConnectJson(void)
    {
    }

    bool CCJGW_ReqCheckWlanConnectJson::FromJosn(CCJGW_FTM_Json& msgJson)
    {
        msgJson.GetJsonValueToKey<std::string>("wlangateway",wlangateway);
        msgJson.GetJsonValueToKey<std::string>("localaddress",localaddress);
        msgJson.GetJsonValueToKey<std::string>("InterfaceDescription",InterfaceDescription);    
        return CCJGW_RequestConnectWlanJson::FromJosn(msgJson);
    }

}
