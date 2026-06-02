#include "StdAfx.h"
#include "CJGW_FTM_KeyEventResponceVoJson.h"

namespace JGW
{
    bool CCJGW_FTM_KeyEventResponceVoJson::FromJosn(CCJGW_FTM_Json& ftmJson)
    {
        if (!CCJGW_FTM_SocketResponceVo::FromJosn(ftmJson)) return false;
        if (mResultCode == TSE_FTM_DEVICE_FAIL)
        {
            LogE_F("主机设备业务处理有问题(%s)...\n",mResponseErrorMsg.c_str());
            return false;
        }
        CCJGW_FTM_Json keyJson;
        if (!keyJson.FromJsonToString(mstrResult)) return false;

        keyJson.GetJsonValueToKey<std::string>("keyType",keyType);
        keyJson.GetJsonValueToKey<std::string>("event",event);
        return true;
    }
}