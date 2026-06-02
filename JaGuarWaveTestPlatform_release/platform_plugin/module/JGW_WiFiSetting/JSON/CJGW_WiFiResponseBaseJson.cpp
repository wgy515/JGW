#include "StdAfx.h"
#include "CJGW_WiFiResponseBaseJson.h"

namespace JGW
{
    CCJGW_WiFiResponseBaseJson::CCJGW_WiFiResponseBaseJson(void) : bStatus(false),responseType(-1)
    {
    }


    CCJGW_WiFiResponseBaseJson::~CCJGW_WiFiResponseBaseJson(void)
    {
    }

    bool CCJGW_WiFiResponseBaseJson::FromJosn(CCJGW_FTM_Json& msgJson)
    {
        msgJson.GetJsonValueToKey<bool>("bStatus",bStatus);
        msgJson.GetJsonValueToKey<int>("responseType",responseType);
        msgJson.GetJsonValueToKey<std::string>("returnMessage",returnMessage);
        return true;
    }
}