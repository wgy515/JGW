#include "StdAfx.h"
#include "CJGW_FTM_WiFiAPInfoResponceVo.h"

namespace JGW
{
    CCJGW_FTM_WiFiAPInfoResponceVo::CCJGW_FTM_WiFiAPInfoResponceVo(void)
    {
    }


    CCJGW_FTM_WiFiAPInfoResponceVo::~CCJGW_FTM_WiFiAPInfoResponceVo(void)
    {
    }

    bool CCJGW_FTM_WiFiAPInfoResponceVo::FromJosn(CCJGW_FTM_Json& ftmJson)
    {
        if (!CCJGW_FTM_SocketResponceVo::FromJosn(ftmJson)) return false;
        if (mResultCode == TSE_FTM_DEVICE_FAIL)
        {
            LogE_F("主机设备业务处理有问题(%s)...\n",mResponseErrorMsg.c_str());
            return false;
        }

        CCJGW_FTM_Json keyJson;
        if (!keyJson.FromJsonToString(mstrResult)) return false;

        for(boost::property_tree::ptree::iterator msg_it = keyJson.GetJsonTree().begin(); msg_it != keyJson.GetJsonTree().end(); ++msg_it)
        {
            boost::property_tree::ptree msg_1 = msg_it->second;
            for(boost::property_tree::ptree::iterator msg_it1 = msg_1.begin(); msg_it1 != msg_1.end(); ++msg_it1)
            {
                LogI_F("%s : %s\n",msg_it1->first.c_str(),msg_it1->second.get_value<std::string>().c_str());
            }
        }

        for(boost::property_tree::ptree::iterator msg_it = keyJson.GetJsonTree().begin(); msg_it != keyJson.GetJsonTree().end(); ++msg_it)
        {
            S_WIFI_AP_INFO sWiFiAPInfo;
            keyJson.GetJsonValueToKey<int>(msg_it->second,"level",sWiFiAPInfo.level);
            keyJson.GetJsonValueToKey<int>(msg_it->second,"frequency",sWiFiAPInfo.frequency);
            keyJson.GetJsonValueToKey<std::string>(msg_it->second,"ssid",sWiFiAPInfo.ssid);
            keyJson.GetJsonValueToKey<std::string>(msg_it->second,"bssid",sWiFiAPInfo.bssid);
            mvsWiFiAPInfo.push_back(sWiFiAPInfo);
        }
        return true;
    }
}