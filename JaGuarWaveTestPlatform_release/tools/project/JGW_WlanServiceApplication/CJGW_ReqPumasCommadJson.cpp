#include "StdAfx.h"
#include "CJGW_ReqPumasCommadJson.h"

namespace JGW
{
    CCJGW_ReqPumasCommadJson::CCJGW_ReqPumasCommadJson(void)
    {
    }


    CCJGW_ReqPumasCommadJson::~CCJGW_ReqPumasCommadJson(void)
    {
    }

    bool CCJGW_ReqPumasCommadJson::FromJosn(CCJGW_FTM_Json& msgJson)
    {
        msgJson.GetJsonValueToKey<std::string>("command",command);
        return true;
    }
}
