#pragma once
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace JGW
{
	class CCJGW_ResponseBaseJson
	{
	public:
		CCJGW_ResponseBaseJson(void);
		~CCJGW_ResponseBaseJson(void);

		std::string BuildResponseBaseJson();
	public:
		bool bStatus;
		//! 响应消息类型
		int responseType;
		//! 返回消息
		std::string returnMessage;
		//! 
		boost::property_tree::ptree mJsonTree;
	};
}