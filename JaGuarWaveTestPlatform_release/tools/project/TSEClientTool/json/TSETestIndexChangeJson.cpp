#include "StdAfx.h"
#include "TSETestIndexChangeJson.h"

namespace JGW
{
    CTSETestIndexChangeJson::CTSETestIndexChangeJson(void) : TestIndex(0),SubIndex(-1)
    {
    }


    CTSETestIndexChangeJson::~CTSETestIndexChangeJson(void)
    {
    }

    bool CTSETestIndexChangeJson::FromJosn(CCJGW_FTM_Json& msgJson)
    {
        bool ret = CTSEServerBaseJson::FromJosn(msgJson);
        ret &= msgJson.GetJsonValueToKey<int>("TestIndex",TestIndex);
        ret &= msgJson.GetJsonValueToKey<int>("SubIndex",SubIndex);
        return ret;
    }
}
