#include "StdAfx.h"
#include "CJGW_FTM_Json.h"

namespace JGW
{
    CCJGW_FTM_Json::CCJGW_FTM_Json(void)
    {
    }


    CCJGW_FTM_Json::~CCJGW_FTM_Json(void)
    {
    }

    bool CCJGW_FTM_Json::FromJsonToString(const std::string& strJson)
    {
        mJsonTree.clear();
        std::stringstream msg_ss(strJson);
        try 
        {
            read_json(msg_ss, mJsonTree);
            return true;
        }
        catch (boost::property_tree::json_parser::json_parser_error& e)
        {
            SendWindowMsgFormat(L"parser Json(%s) Fail(%s)",JGW_A2W(strJson).c_str(),JGW_A2W_A(e.what()).c_str());   
        }
        return false;
    }

    boost::property_tree::iptree& CCJGW_FTM_Json::GetJsonTree()
    {
        return mJsonTree;
    }
}
