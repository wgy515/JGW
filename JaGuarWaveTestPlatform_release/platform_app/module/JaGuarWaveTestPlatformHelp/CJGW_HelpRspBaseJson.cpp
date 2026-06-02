#include "StdAfx.h"
#include "CJGW_HelpRspBaseJson.h"

namespace JGW
{
    CCJGW_HelpRspBaseJson::CCJGW_HelpRspBaseJson(void)
    {
    }


    CCJGW_HelpRspBaseJson::~CCJGW_HelpRspBaseJson(void)
    {
    }

    bool CCJGW_HelpRspBaseJson::FromJosnString(const std::string& strJson)
    {
        CCJGW_FTM_Json ftmJson;
        if (!ftmJson.FromJsonToString(strJson)) return false;;
        return FromJosn(ftmJson);
    }

    bool CCJGW_HelpRspBaseJson::FromJosn(CCJGW_FTM_Json& msgJson)
    {
        msgJson.GetJsonValueToKey<bool>("bUpdate",bUpdate);
        msgJson.GetJsonValueToKey<bool>("status",status);
        msgJson.GetJsonValueToKey<int>("helpRspType",helpRspType);
        msgJson.GetJsonValueToKey<std::string>("error_msg",error_msg);
        msgJson.GetJsonValueToKey<std::string>("version",version);
        return true;
    }
}

