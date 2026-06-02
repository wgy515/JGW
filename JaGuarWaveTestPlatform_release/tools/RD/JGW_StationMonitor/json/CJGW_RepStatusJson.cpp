#include "StdAfx.h"
#include "CJGW_RepStatusJson.h"
#include "CJGW_FTM_Json.h"
#include "..\CJGW_StationMonitorDefine.h"
#include <JGW_FoundationFunc\JGW_StringFunc.h>

namespace JGW
{
    CCJGW_RepStatusJson::CCJGW_RepStatusJson(void) : requestCode(REQ_CODE_STATUS_REPORT),class_num(0)
    {
    }


    CCJGW_RepStatusJson::~CCJGW_RepStatusJson(void)
    {
    }

    std::string CCJGW_RepStatusJson::BuildStatusJson()
    {
#if 0
        boost::property_tree::ptree jsonTree;

        jsonTree.put<int>("requestCode",requestCode);
        jsonTree.put("mac",mac);
        jsonTree.put("ip",ip);
        jsonTree.put<int>("class",class_num);
        jsonTree.put<int>("rssi",rssi);
        jsonTree.put<int>("tx_mcs",tx_mcs);
        jsonTree.put<int>("rx_mcs",rx_mcs);
        jsonTree.put<int>("tx_sector",tx_sector);
        jsonTree.put<int>("rx_sector",rx_sector);
        jsonTree.put<float>("tx_drop_rate",tx_drop_rate);
        jsonTree.put<float>("rx_drop_rate",rx_drop_rate);

        std::stringstream s2;  
        write_json(s2, jsonTree);  
        return s2.str();
#else
        std::string strJson;
        JGW_FormatString(strJson,"{\"requestCode\":%d,\"mac\":\"%s\",\"ip\":\"%s\",\"class\":%d,\"rssi\":%d,\"tx_mcs\":%d,\"rx_mcs\":%d,\"tx_sector\":%d,\"rx_sector\":%d,\"tx_drop_rate\":%.2f,\"rx_drop_rate\":%.2f,\"tx_speed\":\"%s\",\"rx_speed\":\"%s\"}",requestCode,mac.c_str(),ip.c_str(),class_num,rssi,tx_mcs,rx_mcs,tx_sector,rx_sector,tx_drop_rate,rx_drop_rate,tx_speed.c_str(),rx_speed.c_str());
        return strJson;
#endif
    }

    void CCJGW_RepStatusJson::FromJsonToString(const std::string& strJson)
    {
        CCJGW_FTM_Json ftmJson;
        ftmJson.FromJsonToString(strJson);

        ftmJson.GetJsonValueToKey<int>("requestCode",requestCode);
        ftmJson.GetJsonValueToKey<std::string>("mac",mac);
        ftmJson.GetJsonValueToKey<std::string>("ip",ip);
        ftmJson.GetJsonValueToKey<int>("class",class_num);
        ftmJson.GetJsonValueToKey<int>("rssi",rssi);

        ftmJson.GetJsonValueToKey<int>("tx_mcs",tx_mcs);
        ftmJson.GetJsonValueToKey<int>("rx_mcs",rx_mcs);

        ftmJson.GetJsonValueToKey<int>("tx_sector",tx_sector);
        ftmJson.GetJsonValueToKey<int>("rx_sector",rx_sector);

        ftmJson.GetJsonValueToKey<float>("tx_drop_rate",tx_drop_rate);
        ftmJson.GetJsonValueToKey<float>("rx_drop_rate",rx_drop_rate);

        ftmJson.GetJsonValueToKey<std::string>("tx_speed",tx_speed);
        ftmJson.GetJsonValueToKey<std::string>("rx_speed",rx_speed);
    }
}