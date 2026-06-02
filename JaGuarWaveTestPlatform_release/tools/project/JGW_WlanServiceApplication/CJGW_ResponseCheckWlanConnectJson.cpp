#include "StdAfx.h"
#include "CJGW_ResponseCheckWlanConnectJson.h"

namespace JGW
{
    CCJGW_ResponseCheckWlanConnectJson::CCJGW_ResponseCheckWlanConnectJson(void)
    {
    }


    CCJGW_ResponseCheckWlanConnectJson::~CCJGW_ResponseCheckWlanConnectJson(void)
    {
    }

    std::string CCJGW_ResponseCheckWlanConnectJson::BuildResponseCheckWlanConnectJson()
    {
        BuildResponseBaseJson();
        mJsonTree.put("wlanAddress",wlanAddress);
        std::stringstream s2;  
        write_json(s2, mJsonTree);  
        return s2.str();  
    }
}

