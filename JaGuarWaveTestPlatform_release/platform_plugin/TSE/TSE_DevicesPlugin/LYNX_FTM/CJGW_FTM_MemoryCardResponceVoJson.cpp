#include "StdAfx.h"
#include "CJGW_FTM_MemoryCardResponceVoJson.h"

namespace JGW
{
    CCJGW_FTM_MemoryCardResponceVoJson::CCJGW_FTM_MemoryCardResponceVoJson(void)
    {
    }


    CCJGW_FTM_MemoryCardResponceVoJson::~CCJGW_FTM_MemoryCardResponceVoJson(void)
    {
    }

    bool CCJGW_FTM_MemoryCardResponceVoJson::FromJosn(CCJGW_FTM_Json& ftmJson)
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
            S_MEMORY_CARD_INFO sMemoryCardInfo;
            keyJson.GetJsonValueToKey<__int64>(msg_it->second,"totalBlocks",sMemoryCardInfo.totalBlocks);
            keyJson.GetJsonValueToKey<__int64>(msg_it->second,"availableBlocks",sMemoryCardInfo.availableBlocks);
            keyJson.GetJsonValueToKey<__int64>(msg_it->second,"blockByteSize",sMemoryCardInfo.blockByteSize);
            keyJson.GetJsonValueToKey<__int64>(msg_it->second,"freeBlocks",sMemoryCardInfo.freeBlocks);
            keyJson.GetJsonValueToKey<__int64>(msg_it->second,"freeBytes",sMemoryCardInfo.freeBytes);
            keyJson.GetJsonValueToKey<__int64>(msg_it->second,"totalBlocks",sMemoryCardInfo.totalBlocks);
            keyJson.GetJsonValueToKey<__int64>(msg_it->second,"totalBytes",sMemoryCardInfo.totalBytes);
            keyJson.GetJsonValueToKey<std::string>(msg_it->second,"description",sMemoryCardInfo.description);
            mvsMemoryCardInfo.push_back(sMemoryCardInfo);
        }
        return true;
    }
}