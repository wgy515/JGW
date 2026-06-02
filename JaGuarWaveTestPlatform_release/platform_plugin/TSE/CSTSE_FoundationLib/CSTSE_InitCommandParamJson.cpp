#include "StdAfx.h"
#include "CSTSE_InitCommandParamJson.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/typeof/typeof.hpp>
namespace JGW
{
    CCSTSE_InitCommandParamJson::CCSTSE_InitCommandParamJson(void) : CCSTSE_BaseJson(ECSTSE_ACTION_COMMAND_PARAM_INIT_TSE)
    {
    }


    CCSTSE_InitCommandParamJson::~CCSTSE_InitCommandParamJson(void)
    {
    }

    std::string CCSTSE_InitCommandParamJson::GetInitCommandParamJson()
    {
        boost::property_tree::ptree jsonTree,child_eq;

        jsonTree.put<int>("CSTSEAction",CSTSEAction);
        for (std::map<std::string,std::string>::iterator it = Params.begin();
            it != Params.end();++ it)
        {
            child_eq.put<std::string>(it->first, it->second);
        }
        jsonTree.add_child("Params",child_eq);

        std::stringstream s2;  
        write_json(s2, jsonTree);  
        return s2.str();
    }

    bool CCSTSE_InitCommandParamJson::ParseInitCommandParamJson(const std::string& strJson)
    {
        boost::property_tree::ptree mJsonTree;
        std::stringstream msg_ss(strJson);
        try 
        {
            read_json(msg_ss, mJsonTree);
            CSTSEAction = (ECSTSE_ACTION)mJsonTree.get<int>("CSTSEAction");
            if (ECSTSE_ACTION_COMMAND_PARAM_INIT_TSE != CSTSEAction)
            {
                return false;
            }
            boost::property_tree::ptree ptChildRead = mJsonTree.get_child("Params");
            for(boost::property_tree::ptree::iterator pos = ptChildRead.begin(); pos != ptChildRead.end(); ++pos)
            {
                Params[pos->first] = pos->second.get_value<std::string>();
            }
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
}

