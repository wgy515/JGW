#include "StdAfx.h"
#include "TSEServerFlagsJson.h"

namespace JGW
{
    CTSEServerFlagsJson::CTSEServerFlagsJson(void) : Flags("")
    {
    }


    CTSEServerFlagsJson::~CTSEServerFlagsJson(void)
    {
    }

    bool CTSEServerFlagsJson::FromJosn(CCJGW_FTM_Json& msgJson)
    {
        bool ret = CTSEServerBaseJson::FromJosn(msgJson);
        ret &= msgJson.GetJsonValueToKey<std::string>("Flags",Flags);
        return ret;
    }
}
