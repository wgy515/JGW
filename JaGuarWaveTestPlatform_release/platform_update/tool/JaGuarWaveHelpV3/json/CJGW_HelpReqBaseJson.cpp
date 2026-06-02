#include "StdAfx.h"
#include "CJGW_HelpReqBaseJson.h"

namespace JGW
{
    CCJGW_HelpReqBaseJson::CCJGW_HelpReqBaseJson(void)
    {
    }


    CCJGW_HelpReqBaseJson::~CCJGW_HelpReqBaseJson(void)
    {
    }

    bool CCJGW_HelpReqBaseJson::FromJosn(const std::string& strJson)
    {
        CCJGW_FTM_Json ftmJson;
        if (!ftmJson.FromJosn(strJson)) return false;;
        return FromJosn(ftmJson);
    }

    bool CCJGW_HelpReqBaseJson::FromJosn(CCJGW_FTM_Json& msgJson)
    {
        msgJson.GetJsonValueToKey<int>("helpReqType",helpReqType);
        return true;
    }
}

