#include "StdAfx.h"
#include "TSE_JsonParse.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>
namespace JGW
{
    CTSE_JsonParse::CTSE_JsonParse(void)
    {
    }


    CTSE_JsonParse::~CTSE_JsonParse(void)
    {
    }

    const wchar_t* CTSE_JsonParse::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_JsonParse\",\
                \"TestName\": \"Json Parse\",\
                \"JsonEnvironment\": \"Json String Environment\",\
                \"JsonTreeList\": \"Json Tree List,split ','\",\
                \"SaveJsonValueEnvironment\": \"Save Json Value Environment\"\
                }";
    }

    bool CTSE_JsonParse::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"JsonEnvironment",strParamName))
        {
            mstrJsonEnvironment = strParamValue;
        }
        if (TSE_PARAM_NAME_EQUAL(L"JsonTreeList",strParamName))
        {
            //mstrJsonTreeList = strParamValue;
            mvJsonTreeList.clear();
            JGW_ParserStrA(JGW_W2A_W(strParamValue).c_str(),",",mvJsonTreeList);
        } 
        if (TSE_PARAM_NAME_EQUAL(L"SaveJsonValueEnvironment",strParamName))
        {
            mstrSaveJsonValueEnvironment = strParamValue;
        } 
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_JsonParse::TSE_Run()
    {
        boost::property_tree::ptree mJsonTree;
        std::string strTemp,strJson = GetGlobalEnvironment()->GetAnsiString(mstrJsonEnvironment);
        std::stringstream msg_ss(strJson);

        try 
        {
            read_json(msg_ss, mJsonTree);
            boost::property_tree::ptree ptChild = mJsonTree;
            for (size_t i = 0;i < mvJsonTreeList.size();i ++)
            {
                if (i + 1 == mvJsonTreeList.size())
                {
                    strTemp = ptChild.get<std::string>(mvJsonTreeList[i]);
                }
                else
                {
                    if(!ptChild.count(mvJsonTreeList[i]))
                    {
                        LogE_F("Error: Json Tree Key (%s) Not Found.",mvJsonTreeList[i]);
                        return false;
                    }
                    ptChild = ptChild.get_child(mvJsonTreeList[i]);
                }
            }
            LogI_F("Json Value: %s",strTemp.c_str());
            GetGlobalEnvironment()->PutAnsiString(mstrSaveJsonValueEnvironment,strTemp);
            return true;
        }
        catch (boost::property_tree::json_parser::json_parser_error& e)
        {
           LogE_F("parser Json(%s) Fail(%s)\n",strJson.c_str(),e.what());   
        }
        catch (boost::property_tree::ptree_bad_path& e)
        {
            LogE_F("%s\n",e.what());
        }
        catch (boost::property_tree::ptree_bad_data& e)
        {
            LogE_F("%s\n",e.what());
        }
        catch (...)
        {
            LogE(L"Unknown Exception");
        }
        return false;
    }
}

