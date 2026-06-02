#include "StdAfx.h"
#include "CSTSE_TestIndexChangeJson.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace JGW
{
    CCSTSE_TestIndexChangeJson::CCSTSE_TestIndexChangeJson(void) : CSTSEAction(ECSTSE_ACTION_TEST_INDEX_CHANGE)
    {
    }


    CCSTSE_TestIndexChangeJson::~CCSTSE_TestIndexChangeJson(void)
    {
    }

    std::string CCSTSE_TestIndexChangeJson::GetTestIndexChangeJson()
    {
        return JGW_GetFormatString("{\"CSTSEAction\":%d,\"Index\":%d,\"SubIndex\":%d}",(int)CSTSEAction,Index,SubIndex); 
    }

    bool CCSTSE_TestIndexChangeJson::ParseTestIndexChangeJson(const std::string& strJson)
    {
        boost::property_tree::ptree mJsonTree;
        std::stringstream msg_ss(strJson);
        try 
        {
            read_json(msg_ss, mJsonTree);
            CSTSEAction = (ECSTSE_ACTION)mJsonTree.get<int>("CSTSEAction");
            if (CSTSEAction != ECSTSE_ACTION_TEST_INDEX_CHANGE)
            {
                return false;
            }

            Index = mJsonTree.get<int>("Index");
            SubIndex = mJsonTree.get<int>("SubIndex");
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
