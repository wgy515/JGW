#include "StdAfx.h"
#include <TSE_MSFoundationLib/TSE_MSLogJson.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#define USE_BOOST_JSON
namespace JGW
{
    CTSE_MSLogJson::CTSE_MSLogJson(void)
    {
    }


    CTSE_MSLogJson::~CTSE_MSLogJson(void)
    {
    }

    std::string CTSE_MSLogJson::GetMSLogJson(const std::string& strID,const std::string& strMsg,bool encrypt /* = false */,bool bLog /* = true */)
    {
        std::string strTemp;
#ifdef USE_BOOST_JSON
        try
        {
            if (bLog)
            {
                boost::property_tree::ptree ptreeJson;
                ptreeJson.put("encrypt",encrypt ? 1 : 0);
                ptreeJson.put("is_log",bLog ? 1 : 0);
                ptreeJson.put("msg",strMsg);
                ptreeJson.put("id",strID);

                std::stringstream sstream;
                boost::property_tree::json_parser::write_json(sstream, ptreeJson);
                std::string strRequestParams = sstream.str();
                return strRequestParams;
                //JGW_FormatString(strTemp,"{\"encrypt\":%d,\"is_log\":%d,\"id\":\"%s\",\"msg\":\"%s\"}",encrypt ? 1 : 0,bLog ? 1 : 0,strID.c_str(),strMsg.c_str());
            }
            else
            {
                boost::property_tree::ptree ptreeJson;
                ptreeJson.put("encrypt",encrypt ? 1 : 0);
                ptreeJson.put("is_log",bLog ? 1 : 0);
                ptreeJson.put("id",strID);

                boost::property_tree::ptree jsonTree;
                std::stringstream msg_ss(strMsg);
                read_json(msg_ss, jsonTree);
                ptreeJson.put_child("msg",jsonTree);

                std::stringstream sstream;
                boost::property_tree::json_parser::write_json(sstream, ptreeJson);
                std::string strRequestParams = sstream.str();
                return strRequestParams;

                //JGW_FormatString(strTemp,"{\"encrypt\":%d,\"is_log\":%d,\"id\":\"%s\",\"msg\":%s}",encrypt ? 1 : 0,bLog ? 1 : 0,strID.c_str(),strMsg.c_str());
            }
        }
        catch (boost::property_tree::json_parser::json_parser_error& )
        {
            //LogE_F("parser Json(%s) Fail(%s)\n",strJson.c_str(),e.what());   
        }
        catch (boost::property_tree::ptree_bad_path& )
        {
            //LogE_F("(%s\n",e.what());
        }
        catch (boost::property_tree::ptree_bad_data& )
        {
            // LogE_F("%s\n",e.what());
        }    
#else
        if (bLog)
        {
            JGW_FormatString(strTemp,"{\"encrypt\":%d,\"is_log\":%d,\"id\":\"%s\",\"msg\":\"%s\"}",encrypt ? 1 : 0,bLog ? 1 : 0,strID.c_str(),strMsg.c_str());
        }
        else
        {
            JGW_FormatString(strTemp,"{\"encrypt\":%d,\"is_log\":%d,\"id\":\"%s\",\"msg\":%s}",encrypt ? 1 : 0,bLog ? 1 : 0,strID.c_str(),strMsg.c_str());
        }
#endif
       
        return strTemp;
    }

    bool CTSE_MSLogJson::GetMSLogToJson(std::string& strID,std::string& strMsg,bool& encrypt,bool& bLog,const std::string& strJson)
    {
        boost::property_tree::ptree jsonTree;
        std::stringstream msg_ss(strJson);
        try 
        {
            read_json(msg_ss, jsonTree);
            strID = jsonTree.get<std::string>("id");
            encrypt = jsonTree.get<bool>("encrypt");
            bLog = jsonTree.get<bool>("is_log");
            if (bLog)
            {
                strMsg = jsonTree.get<std::string>("msg");
            }
            else
            {
                auto vul = jsonTree.get_child("msg");
                std::stringstream ss;
                boost::property_tree::write_json(ss, vul);
                strMsg = ss.str();
            }
              
            return true;
        }
        catch (boost::property_tree::json_parser::json_parser_error& )
        {
            //LogE_F("parser Json(%s) Fail(%s)\n",strJson.c_str(),e.what());   
        }
        catch (boost::property_tree::ptree_bad_path& )
        {
            //LogE_F("(%s\n",e.what());
        }
        catch (boost::property_tree::ptree_bad_data& )
        {
            // LogE_F("%s\n",e.what());
        }
        return false;
    }

    bool CTSE_MSLogJson::GetMSControlType(TSEControlTypeEnum& eControlType,const std::string& strJson)
    {
        boost::property_tree::ptree jsonTree;
        std::stringstream msg_ss(strJson);
        try 
        {
            read_json(msg_ss, jsonTree);
            eControlType = (TSEControlTypeEnum)jsonTree.get<int>("Type");
            return true;
        }
        catch (boost::property_tree::json_parser::json_parser_error& )
        {
            //LogE_F("parser Json(%s) Fail(%s)\n",strJson.c_str(),e.what());   
        }
        catch (boost::property_tree::ptree_bad_path& )
        {
            //LogE_F("(%s\n",e.what());
        }
        catch (boost::property_tree::ptree_bad_data& )
        {
            // LogE_F("%s\n",e.what());
        }
        return false;
    }

    std::string CTSE_MSLogJson::GetMSBaseJson(TSEControlTypeEnum eControlType)
    {
#ifdef USE_BOOST_JSON
        try
        {
            boost::property_tree::ptree ptreeJson;
            ptreeJson.put("Type",(int)eControlType);

            std::stringstream sstream;
            boost::property_tree::json_parser::write_json(sstream, ptreeJson);
            std::string strRequestParams = sstream.str();
            return strRequestParams;
        }
        catch (boost::property_tree::json_parser::json_parser_error& )
        {
            //LogE_F("parser Json(%s) Fail(%s)\n",strJson.c_str(),e.what());   
        }
        catch (boost::property_tree::ptree_bad_path& )
        {
            //LogE_F("(%s\n",e.what());
        }
        catch (boost::property_tree::ptree_bad_data& )
        {
            // LogE_F("%s\n",e.what());
        }   
        return "";
#else
        return JGW_GetFormatString("{\"Type\":%d}",eControlType);
#endif
    }

    std::string CTSE_MSLogJson::GetMSXttJson(int engineType,const std::string& strXttFilePath)
    {
#ifdef USE_BOOST_JSON
        try
        {
            boost::property_tree::ptree ptreeJson;
            ptreeJson.put("Type",(int)TSEControlXttType);
            ptreeJson.put("EngineType",engineType);
            ptreeJson.put("XttFilePath",strXttFilePath);

            std::stringstream sstream;
            boost::property_tree::json_parser::write_json(sstream, ptreeJson);
            std::string strRequestParams = sstream.str();
            return strRequestParams;
        }
        catch (boost::property_tree::json_parser::json_parser_error& )
        {
            //LogE_F("parser Json(%s) Fail(%s)\n",strJson.c_str(),e.what());   
        }
        catch (boost::property_tree::ptree_bad_path& )
        {
            //LogE_F("(%s\n",e.what());
        }
        catch (boost::property_tree::ptree_bad_data& )
        {
            // LogE_F("%s\n",e.what());
        }   
        return "";
#else
        return JGW_GetFormatString("{\"Type\":%d,\"EngineType\":%d,\"XttFilePath\":\"%s\"}",(int)TSEControlXttType,engineType,strXttFilePath.c_str());
#endif      
    }

    bool CTSE_MSLogJson::GetMCResponseJsonToJson(const std::string& strJson,S_MC_RSP_JSON& smcRspJson)
    {
        boost::property_tree::ptree jsonTree;
        std::stringstream msg_ss(strJson);
        try 
        {
            read_json(msg_ss, jsonTree);
            smcRspJson.Type = (TSEControlTypeEnum)jsonTree.get<int>("Type");
            smcRspJson.Result = jsonTree.get<bool>("Result");
            smcRspJson.Error = jsonTree.get<std::string>("Error");
            return true;
        }
        catch (boost::property_tree::json_parser::json_parser_error& )
        {
            //LogE_F("parser Json(%s) Fail(%s)\n",strJson.c_str(),e.what());   
        }
        catch (boost::property_tree::ptree_bad_path& )
        {
            //LogE_F("(%s\n",e.what());
        }
        catch (boost::property_tree::ptree_bad_data& )
        {
            // LogE_F("%s\n",e.what());
        }
        return false;
    }

    std::string CTSE_MSLogJson::GetMSGlobalVariableJson(bool bSet,const std::string& strKey,const std::string& strValue)
    {
#ifdef USE_BOOST_JSON
        try
        {
            boost::property_tree::ptree ptreeJson;
            ptreeJson.put("Type",(int)TSEControlGlobalVariableType);
            ptreeJson.put("IsSet",bSet);
            ptreeJson.put("Key",strKey);
            ptreeJson.put("Value",strValue);

            std::stringstream sstream;
            boost::property_tree::json_parser::write_json(sstream, ptreeJson);
            std::string strRequestParams = sstream.str();
            return strRequestParams;
        }
        catch (boost::property_tree::json_parser::json_parser_error& )
        {
            //LogE_F("parser Json(%s) Fail(%s)\n",strJson.c_str(),e.what());   
        }
        catch (boost::property_tree::ptree_bad_path& )
        {
            //LogE_F("(%s\n",e.what());
        }
        catch (boost::property_tree::ptree_bad_data& )
        {
            // LogE_F("%s\n",e.what());
        }   
        return "";
#else
        return JGW_GetFormatString("{\"Type\":%d,\"IsSet\":%d,\"Key\":\"%s\",\"Value\":\"%s\"}",(int)TSEControlGlobalVariableType,bSet ? 1 : 0,strKey.c_str(),strValue.c_str());
#endif    
    }

}
