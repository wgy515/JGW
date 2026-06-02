#include "StdAfx.h"
#include "CJGW_CopyDataTestLogJson.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_CopyDataTestLogJson::CCJGW_CopyDataTestLogJson(void)
    {
    }


    CCJGW_CopyDataTestLogJson::~CCJGW_CopyDataTestLogJson(void)
    {
    }

    bool CCJGW_CopyDataTestLogJson::FromJosn(CCJGW_FTM_Json& msgJson)
    {
        msgJson.GetJsonValueToKey<int>("LogType",LogType);
        msgJson.GetJsonValueToKey<int>("UserDefineID",UserDefineID);
        std::string messgae;
        msgJson.GetJsonValueToKey<std::string>("Message",messgae);
        Message = JGW_A2W(messgae,CP_UTF8);
        return true;
    }
}
