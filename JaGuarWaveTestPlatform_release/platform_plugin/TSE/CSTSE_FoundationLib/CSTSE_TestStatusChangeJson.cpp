#include "StdAfx.h"
#include "CSTSE_TestStatusChangeJson.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
namespace JGW
{
    CCSTSE_TestStatusChangeJson::CCSTSE_TestStatusChangeJson(void) : CSTSEAction(ECSTSE_ACTION_TEST_STATUS_CHANGE)
    {
    }


    CCSTSE_TestStatusChangeJson::~CCSTSE_TestStatusChangeJson(void)
    {

    }

    std::string CCSTSE_TestStatusChangeJson::GetTestStatusChangeJson()
    {
        return JGW_GetFormatString("{\"CSTSEAction\":%d,\"TestStatus\":%d,\"TestName\":\"%s\"}",(int)CSTSEAction,(int)TestStatus,TestName.c_str()); 
    }

    bool CCSTSE_TestStatusChangeJson::ParseTestStatusChangeJson(const std::string& strJson)
    {
        boost::property_tree::ptree mJsonTree;
        std::stringstream msg_ss(strJson);
        try 
        {
            read_json(msg_ss, mJsonTree);
            CSTSEAction = (ECSTSE_ACTION)mJsonTree.get<int>("CSTSEAction");
            if (CSTSEAction != ECSTSE_ACTION_TEST_STATUS_CHANGE)
            {
                return false;
            }

            TestStatus = (E_TEST_STATUS)mJsonTree.get<int>("TestStatus");
            TestName = mJsonTree.get<std::string>("TestName");
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
