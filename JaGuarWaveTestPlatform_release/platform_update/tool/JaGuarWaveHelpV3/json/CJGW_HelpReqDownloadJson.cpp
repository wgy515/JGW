#include "StdAfx.h"
#include "CJGW_HelpReqDownloadJson.h"

namespace JGW
{
    CCJGW_HelpReqDownloadJson::CCJGW_HelpReqDownloadJson(void)
    {
    }


    CCJGW_HelpReqDownloadJson::~CCJGW_HelpReqDownloadJson(void)
    {
    }

    bool CCJGW_HelpReqDownloadJson::FromJosn(CCJGW_FTM_Json& msgJson)
    {
        msgJson.GetJsonValueToKey<std::string>("execFolder",execFolder);
        msgJson.GetJsonValueToKey<std::string>("taskNumber",taskNumber);
        return true;
    }
}