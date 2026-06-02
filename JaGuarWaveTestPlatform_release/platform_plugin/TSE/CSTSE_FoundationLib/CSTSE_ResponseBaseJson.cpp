#include "StdAfx.h"
#include <CSTSE_FoundationLib/CSTSE_ResponseBaseJson.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCSTSE_ResponseBaseJson::CCSTSE_ResponseBaseJson(ECSTSE_ACTION action) : CCSTSE_BaseJson(action),Result(false)
    {
    }


    CCSTSE_ResponseBaseJson::~CCSTSE_ResponseBaseJson(void)
    {
    }

    bool CCSTSE_ResponseBaseJson::ParseResponseBaseJson(const std::string& strJson)
    {
        boost::property_tree::ptree mJsonTree;
        std::stringstream msg_ss(strJson);
        try 
        {
            read_json(msg_ss, mJsonTree);
            CSTSEAction = (ECSTSE_ACTION)mJsonTree.get<int>("CSTSEAction");
            //RequestAction = (ECSTSE_ACTION)mJsonTree.get<int>("RequestAction");
            Result = mJsonTree.get<bool>("Result");
            ErrorMsg = mJsonTree.get<std::string>("ErrorMsg");
            return true;
        }
        catch (boost::property_tree::json_parser::json_parser_error& )
        {
            //PELOG4WA_F("parser Json(%s) Fail(%s)",strJson.c_str(),e.what());   
        }
        catch (boost::property_tree::ptree_bad_path&)
        {
            ///PELOG4WA_F("(%s",e.what());
        }
        catch (boost::property_tree::ptree_bad_data&)
        {
            ///PELOG4WA_F("%s",e.what());
        }
        return false;
    }

    std::string CCSTSE_ResponseBaseJson::GetResponseBaseJson()
    {
        return JGW_GetFormatString("{\"CSTSEAction\":%d,\"Result\":%s,\"ErrorMsg\":\"%s\"}",(int)CSTSEAction,Result ? "true" : "false",ErrorMsg.c_str());
    }
}

