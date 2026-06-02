#include "StdAfx.h"
#include "CSTSE_TestLogJson.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace JGW
{
    CCSTSE_TestLogJson::CCSTSE_TestLogJson(void) : CSTSEAction(ECSTSE_ACTION_TEST_LOG),LogType(ECSTSE_TSE_TEST_LOG_TYPE_DEBUG),UserDefineType(E_UNKNOW_USER_DEFINE)
    {
    }


    CCSTSE_TestLogJson::~CCSTSE_TestLogJson(void)
    {
    }

    std::string CCSTSE_TestLogJson::GetTestLogJson()
    {
        return JGW_GetFormatString("{\"CSTSEAction\":%d,\"LogType\":%d,\"UserDefineType\":%d,\"Log\":\"%s\"}",(int)CSTSEAction,LogType,UserDefineType,Log.c_str()); 
    }

    bool CCSTSE_TestLogJson::ParseTestLogJson(const std::string& strJson)
    {
        boost::property_tree::ptree mJsonTree;
        std::stringstream msg_ss(strJson);
        try 
        {
            read_json(msg_ss, mJsonTree);
            CSTSEAction = (ECSTSE_ACTION)mJsonTree.get<int>("CSTSEAction");
            if (CSTSEAction != ECSTSE_ACTION_TEST_LOG)
            {
                return false;
            }

            LogType = (ECSTSE_TSE_TEST_LOG_TYPE)mJsonTree.get<int>("LogType");
            UserDefineType = (E_USER_DEFINE_LOG_TYPE)mJsonTree.get<int>("UserDefineType");
            Log = mJsonTree.get<std::string>("Log");
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

