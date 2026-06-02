#include "StdAfx.h"
#include "TSETestStatusChangeJson.h"

namespace JGW
{
    CTSETestStatusChangeJson::CTSETestStatusChangeJson(void) : TestStatusEnumID(E_INIT_TEST_STATUS)
    {
    }


    CTSETestStatusChangeJson::~CTSETestStatusChangeJson(void)
    {
    }

    bool CTSETestStatusChangeJson::FromJosn(CCJGW_FTM_Json& msgJson)
    {
        bool ret = CTSEServerBaseJson::FromJosn(msgJson);
        int testStatus = 0;
        ret &= msgJson.GetJsonValueToKey<int>("TestStatusEnumID",testStatus);
        TestStatusEnumID = (E_TEST_STATUS)testStatus;
        ret &= msgJson.GetJsonValueToKey<std::string>("TestName",TestName);
        return ret;
    }
}

