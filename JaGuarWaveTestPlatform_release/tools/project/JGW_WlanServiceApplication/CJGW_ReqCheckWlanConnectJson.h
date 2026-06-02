#pragma once
#include "CJGW_RequestConnectWlanJson.h"

namespace JGW
{
    class CCJGW_ReqCheckWlanConnectJson : public CCJGW_RequestConnectWlanJson
    {
    public:
        CCJGW_ReqCheckWlanConnectJson(void);
        ~CCJGW_ReqCheckWlanConnectJson(void);
    public:
        bool FromJosn(CCJGW_FTM_Json& msgJson);
    public:
        std::string wlangateway;
        std::string localaddress; //! 当前主机内网IP地址
        std::string InterfaceDescription;
    };
}


