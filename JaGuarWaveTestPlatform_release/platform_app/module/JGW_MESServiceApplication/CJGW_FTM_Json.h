#pragma once
#include <sstream>
#include <vector>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
	extern void SendWindowMsg(const std::wstring& strMessage);
	extern void SendWindowMsgFormat(const wchar_t* szFormat,...);

    class CCJGW_FTM_Json
    {
    public:
        CCJGW_FTM_Json(void);
        ~CCJGW_FTM_Json(void);
        //! 将字符串解析成Json
        bool FromJsonToString(const std::string& strJson);
        //! 获取JSON
        boost::property_tree::iptree& GetJsonTree();

		void GetJsonKeyValueToChildKey(const std::string& strKey,std::map<std::string,std::string>& mmapJsonKeyValue)
		{
			mmapJsonKeyValue.clear();
			if(mJsonTree.count(strKey))
			{
				boost::property_tree::iptree ptChildRead = mJsonTree.get_child(strKey);
				for(BOOST_AUTO(pos, ptChildRead.begin()); pos != ptChildRead.end(); ++pos)
				{
					mmapJsonKeyValue.insert(std::make_pair<std::string,std::string>(pos->first,pos->second.get_value<std::string>()));
				}
				//for(iptree::iterator msg_it1 = ptChildRead.begin(); msg_it1 != ptChildRead.end(); ++msg_it1)
				//{
				//	//std::cout << msg_it1->first << " : " << msg_it1->second.get_value<string>(msg_it1->first) << std::endl;
				//}
			}
			else
			{
				SendWindowMsgFormat(L"Not Fount Json Key(%s)",JGW_A2W(strKey).c_str());
			}
		}

        int GetJsonValueToKey(const std::string& strKey,std::vector<std::string>& jobID)
        {
            int ret = 0;
            jobID.clear();
            if(mJsonTree.count(strKey))
            {
                boost::property_tree::iptree ptChildRead = mJsonTree.get_child(strKey);
                for(BOOST_AUTO(pos, ptChildRead.begin()); pos != ptChildRead.end(); ++pos)
                {
                    jobID.push_back(pos->second.get_value<std::string>());
                }
            }
            else
            {
                SendWindowMsgFormat(L"Not Fount Json Key(%s)",JGW_A2W(strKey).c_str());
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
            catch (boost::property_tree::ptree_bad_path& e)
            {
                SendWindowMsgFormat(L"%s",JGW_A2W_A(e.what()).c_str());
            }
            catch (boost::property_tree::ptree_bad_data& e)
            {
                SendWindowMsgFormat(L"%s",JGW_A2W_A(e.what()).c_str());
            }
            return false;
        }
        //! t
        template <typename T>
        bool GetJsonValueToKey(boost::property_tree::iptree treeJson,const std::string& strKey,T& strValue)
        {
            try
            {
                strValue = treeJson.get<T>(strKey);
                return true;
            }
            catch (boost::property_tree::ptree_bad_path& e)
            {
                SendWindowMsgFormat(L"%s",JGW_A2W_A(e.what()).c_str());
            }
            catch (boost::property_tree::ptree_bad_data& e)
            {
                SendWindowMsgFormat(L"%s",JGW_A2W_A(e.what()).c_str());
            }
            
            return false;
        }
    protected:
        boost::property_tree::iptree mJsonTree;
    };
}


