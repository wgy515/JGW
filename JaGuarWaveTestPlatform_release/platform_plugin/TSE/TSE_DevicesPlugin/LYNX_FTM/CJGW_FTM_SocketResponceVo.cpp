#include "StdAfx.h"
#include "CJGW_FTM_SocketResponceVo.h"
#include <TSE_FoundationLib/TSE_FoundationLib_Define.h>
#include <sstream>
namespace JGW
{   
    CCJGW_FTM_SocketResponceVo::CCJGW_FTM_SocketResponceVo():mReqCode(-1)
        ,mResultCode(-1)
    {

    }

    bool CCJGW_FTM_SocketResponceVo::FromJosn(const std::string& strJson)
    {
        std::stringstream msg_ss(strJson);
        CCJGW_FTM_Json ftmJson;
        if (!ftmJson.FromJsonToString(strJson)) return false;
        return FromJosn(ftmJson);
    }

    bool CCJGW_FTM_SocketResponceVo::FromJosn(CCJGW_FTM_Json& msgJson)
    {
        msgJson.GetJsonValueToKey<int>("reqCode",mReqCode);
        msgJson.GetJsonValueToKey<int>("resultCode",mResultCode);
        msgJson.GetJsonValueToKey<std::string>("result",mstrResult);
        msgJson.GetJsonValueToKey<std::string>("responseMsg",mResponseErrorMsg);
        return true;
    }
}