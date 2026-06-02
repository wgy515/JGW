#pragma once
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace JGW
{
    class CCJGW_FTM_Json
    {
    public:
        CCJGW_FTM_Json(void);
        ~CCJGW_FTM_Json(void);
        //! 将字符串解析成Json
        bool FromJosn(const std::string& strJson);
        //! 获取JSON
        boost::property_tree::ptree& GetJsonTree();
        //!
        template <typename T>
        bool GetJsonValueToKey(const std::string& strKey,T& strValue)
        {
            try
            {
                strValue = mJsonTree.get<T>(strKey);
                return true;
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
        //! t
        template <typename T>
        bool GetJsonValueToKey(boost::property_tree::ptree treeJson,const std::string& strKey,T& strValue)
        {
            try
            {
                strValue = treeJson.get<T>(strKey);
                return true;
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
    protected:
        boost::property_tree::ptree mJsonTree;
    };
}


