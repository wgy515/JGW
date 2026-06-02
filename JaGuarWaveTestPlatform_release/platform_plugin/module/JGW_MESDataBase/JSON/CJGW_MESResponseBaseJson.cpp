#include "StdAfx.h"
#include "CJGW_MESResponseBaseJson.h"

namespace JGW
{
    CCJGW_MESResponseBaseJson::CCJGW_MESResponseBaseJson(void) : bStatus(false),responseType(-1)
    {
    }


    CCJGW_MESResponseBaseJson::~CCJGW_MESResponseBaseJson(void)
    {
    }

    bool CCJGW_MESResponseBaseJson::FromJosn(CCJGW_FTM_Json& msgJson)
    {
        msgJson.GetJsonValueToKey<bool>("bStatus",bStatus);
        msgJson.GetJsonValueToKey<int>("responseType",responseType);
        msgJson.GetJsonValueToKey<std::string>("returnMessage",returnMessage);
        return true;
    }
}