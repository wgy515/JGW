#include "StdAfx.h"
#include "CJGW_UpdateLogReqJson.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_UpdateLogReqJson::CCJGW_UpdateLogReqJson(void) : Level(E_CSM_DEBUG_LOG)
    {
    }


    CCJGW_UpdateLogReqJson::~CCJGW_UpdateLogReqJson(void)
    {
    }

    bool CCJGW_UpdateLogReqJson::FromJosn(CCJGW_FTM_Json& msgJson)
    {
        msgJson.GetJsonValueToKey<int>("Level",Level);
        msgJson.GetJsonValueToKey<std::string>("Log",Log);
        strwLog = JGW_A2W(Log,CP_UTF8);
        return true;
    }
}
