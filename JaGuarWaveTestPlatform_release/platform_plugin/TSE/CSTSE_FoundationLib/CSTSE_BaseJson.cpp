#include "StdAfx.h"
#include <CSTSE_FoundationLib/CSTSE_BaseJson.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/typeof/typeof.hpp>

namespace JGW
{
    CCSTSE_BaseJson::CCSTSE_BaseJson(ECSTSE_ACTION action) : CSTSEAction(action)
    {
    }


    CCSTSE_BaseJson::~CCSTSE_BaseJson(void)
    {
    }

    bool CCSTSE_BaseJson::ParseBaseJson(const std::string& strRead)
    {
        boost::property_tree::ptree mJsonTree;
        std::stringstream msg_ss(strRead);
        try 
        {
            read_json(msg_ss, mJsonTree);
            CSTSEAction = (ECSTSE_ACTION)mJsonTree.get<int>("CSTSEAction");
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

    std::string CCSTSE_BaseJson::GetBaseJson()
    {
        return JGW_GetFormatString("{\"CSTSEAction\":%d}",(int)CSTSEAction); 
    }
}

