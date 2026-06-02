#include "StdAfx.h"
#include "CSTSE_EnverionmentJson.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/typeof/typeof.hpp>

namespace JGW
{
    CCSTSE_EnverionmentJson::CCSTSE_EnverionmentJson(void) : IsUpdate(false),CCSTSE_BaseJson(ECSTSE_ACTION_Enverionment)
    {
    }


    CCSTSE_EnverionmentJson::~CCSTSE_EnverionmentJson(void)
    {
    }

    std::string CCSTSE_EnverionmentJson::GetEnverionmentJson()
    {
        boost::property_tree::ptree jsonTree,child_eq;

        jsonTree.put<int>("CSTSEAction",CSTSEAction);
        jsonTree.put<bool>("IsUpdate",IsUpdate);
        for (std::map<std::string,std::string>::iterator it = EnvironmentVariables.begin();
            it != EnvironmentVariables.end();++ it)
        {
            child_eq.put<std::string>(it->first, it->second);
        }
        jsonTree.add_child("EnvironmentVariables",child_eq);

        std::stringstream s2;  
        write_json(s2, jsonTree);  
        return s2.str();
    }

    bool CCSTSE_EnverionmentJson::ParseEnverionmentJson(const std::string& strJson)
    {
        boost::property_tree::ptree mJsonTree;
        std::stringstream msg_ss(strJson);
        try 
        {
            read_json(msg_ss, mJsonTree);
            CSTSEAction = (ECSTSE_ACTION)mJsonTree.get<int>("CSTSEAction");
            if (ECSTSE_ACTION_Enverionment != CSTSEAction)
            {
                return false;
            }
            IsUpdate = mJsonTree.get<bool>("IsUpdate");
            boost::property_tree::ptree ptChildRead = mJsonTree.get_child("EnvironmentVariables");
            for(boost::property_tree::ptree::iterator pos = ptChildRead.begin(); pos != ptChildRead.end(); ++pos)
            {
                EnvironmentVariables[pos->first] = pos->second.get_value<std::string>();
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

