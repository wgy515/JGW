#pragma once
#include <sstream>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/typeof/typeof.hpp>
namespace JGW
{
    class CCJGW_FTM_Json
    {
    public:
        CCJGW_FTM_Json(void);
        ~CCJGW_FTM_Json(void);
        //! 将字符串解析成Json
        bool FromJsonToString(const std::string& strJson);
        //! 获取JSON
        boost::property_tree::ptree& GetJsonTree();

        int GetJsonValueToKey(const std::string& strKey,std::vector<std::string>& jobID)
        {
            int ret = 0;
            jobID.clear();
            boost::property_tree::ptree ptChild;
            if(mJsonTree.count(strKey))
            {
                boost::property_tree::ptree ptChildRead = mJsonTree.get_child(strKey);
                for(BOOST_AUTO(pos, ptChildRead.begin()); pos != ptChildRead.end(); ++pos)
                {
                    jobID.push_back(pos->second.get_value<std::string>());
                }
            }
            else
            {
                ///PELOG4WA_F("Not Fount Json Key(%s)",strKey.c_str());
            }
            return ret;
        }

        //!
        template <typename T>
        bool GetJsonValueToKey(const std::string& strKey,T& strValue)
        {
            try
            {
                strValue = mJsonTree.get<T>(strKey);
                return true;
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
        //! t
        template <typename T>
        bool GetJsonValueToKey(boost::property_tree::ptree treeJson,const std::string& strKey,T& strValue)
        {
            try
            {
                strValue = treeJson.get<T>(strKey);
                return true;
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
    protected:
        boost::property_tree::ptree mJsonTree;
    };
}


