#include "StdAfx.h"
#include "TSEControlCmdJson.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CTSEControlCmdJson::CTSEControlCmdJson(void)
    {
    }


    CTSEControlCmdJson::~CTSEControlCmdJson(void)
    {
    }

    TSETestControlEnum CTSEControlCmdJson::GetTestControlCmdIndex()
    {
        for (int i = 0;i <= MAX_TCE;i ++)
        {
            if (0 == JGW_StrComparenoCaseStr(gstrTestGuids[i].c_str(),TestGuid.c_str()))
            {
                return (TSETestControlEnum)(i - 1);
            }
        }

        return Unknown_TCE;
    }

    bool CTSEControlCmdJson::FromJosn(CCJGW_FTM_Json& msgJson)
    {
        bool ret = CTSEServerBaseJson::FromJosn(msgJson);
        ret &= msgJson.GetJsonValueToKey("TestGuid",TestGuid);

        boost::property_tree::ptree ptChildRead = msgJson.GetJsonTree().get_child("Params");
        ///boost::property_tree::ptree msg_1 = ptChildRead.second;
        for(boost::property_tree::ptree::iterator msg_it1 = ptChildRead.begin(); msg_it1 != ptChildRead.end(); ++msg_it1)
        {
            Params[msg_it1->first] = msg_it1->second.get_value<std::string>();
        }
        return ret;
    }
}

