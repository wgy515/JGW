#pragma once
#include "CJGW_ResponseBaseJson.h"

namespace JGW
{
    class CCJGW_ResponseCheckWlanConnectJson : public CCJGW_ResponseBaseJson
    {
    public:
        CCJGW_ResponseCheckWlanConnectJson(void);
        ~CCJGW_ResponseCheckWlanConnectJson(void);
    public:
        std::string BuildResponseCheckWlanConnectJson();
    public:
        std::string wlanAddress;
    };
}