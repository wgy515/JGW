#include "StdAfx.h"
#include "CJGW_StationSocketRequestJson.h"
#include "CJGW_FTM_Json.h"

namespace JGW
{
    CCJGW_StationSocketRequestJson::CCJGW_StationSocketRequestJson(void) : responseCode(0)
    {
    }


    CCJGW_StationSocketRequestJson::~CCJGW_StationSocketRequestJson(void)
    {
    }

    bool CCJGW_StationSocketRequestJson::FromJsonToString(const std::string& strJson)
    {
        CCJGW_FTM_Json ftmJson;
        if (!ftmJson.FromJsonToString(strJson)) return false;

        ftmJson.GetJsonValueToKey<int>("responseCode",responseCode);
        ftmJson.GetJsonValueToKey<std::string>("cmdStr",cmdStr);
        ftmJson.GetJsonValueToKey<std::string>("ip",ip);
        ftmJson.GetJsonValueToKey<std::string>("time",time);

        return true;
    }
}
