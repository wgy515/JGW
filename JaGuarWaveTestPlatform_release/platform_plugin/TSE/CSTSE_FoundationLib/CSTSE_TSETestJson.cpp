#include "StdAfx.h"
#include "CSTSE_TSETestJson.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace JGW
{
    CCSTSE_TSETestJson::CCSTSE_TSETestJson(void) : CCSTSE_BaseJson(ECSTSE_ACTION_TSE_TEST),SuiteTestType(ECSTSE_TSET_TYPE_NORMAL),SuiteIndex(-1)
    {
    }


    CCSTSE_TSETestJson::~CCSTSE_TSETestJson(void)
    {
    }

    std::string CCSTSE_TSETestJson::GetTSETestJson()
    {
        return JGW_GetFormatString("{\"CSTSEAction\":%d,\"SuiteIndex\":%d,\"SuiteTestType\":%d}",(int)CSTSEAction,SuiteIndex,(int)SuiteTestType);
    }

    bool CCSTSE_TSETestJson::ParseTSETestJson(const std::string& strJson)
    {
        boost::property_tree::ptree mJsonTree;
        std::stringstream msg_ss(strJson);
        try 
        {
            read_json(msg_ss, mJsonTree);
            CSTSEAction = (ECSTSE_ACTION)mJsonTree.get<int>("CSTSEAction");
            if (CSTSEAction != ECSTSE_ACTION_TSE_TEST)
            {
                return false;
            }
            SuiteIndex = mJsonTree.get<int>("SuiteIndex");
            SuiteTestType = (ECSTSE_TSE_TEST_TYPE)mJsonTree.get<int>("SuiteTestType");
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

