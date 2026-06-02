#include "StdAfx.h"
#include "CJGW_CSharpBaseRequstJson.h"

namespace JGW
{
    CCJGW_CSharpBaseRequstJson::CCJGW_CSharpBaseRequstJson(void) : RequestID(E_CSM_UNKNOWN)
    {
    }


    CCJGW_CSharpBaseRequstJson::~CCJGW_CSharpBaseRequstJson(void)
    {
    }

    bool CCJGW_CSharpBaseRequstJson::FromJosn(CCJGW_FTM_Json& msgJson)
    {
        int reqID = E_CSM_UNKNOWN;
        bool result = msgJson.GetJsonValueToKey<int>("RequestID",reqID);
        RequestID = (E_CSM_REQ_ID)reqID;
        return result;
    }
}
