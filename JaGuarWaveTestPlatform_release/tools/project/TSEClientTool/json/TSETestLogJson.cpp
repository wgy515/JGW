#include "StdAfx.h"
#include "TSETestLogJson.h"

namespace JGW
{
    CTSETestLogJson::CTSETestLogJson(void) : LogTypeEnumID(Debug_TSE),UserDefineID(E_UNKNOW_USER_DEFINE)
    {
    }


    CTSETestLogJson::~CTSETestLogJson(void)
    {
    }

    bool CTSETestLogJson::FromJosn(CCJGW_FTM_Json& msgJson)
    {
        bool ret = CTSEServerBaseJson::FromJosn(msgJson);

        int temp = Debug_TSE;
        ret &= msgJson.GetJsonValueToKey<int>("LogTypeEnumID",temp);
        LogTypeEnumID = (TSELogTypeEnum)temp;
        ret &= msgJson.GetJsonValueToKey<std::string>("TestLog",TestLog);
        temp = E_UNKNOW_USER_DEFINE;
        ret &= msgJson.GetJsonValueToKey<int>("UserDefineID",temp);
        UserDefineID = (E_USER_DEFINE_LOG_TYPE)temp;
        return ret;
    }
}

