#include "StdAfx.h"
#include "CJGW_ResponseScanWlanJSON.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
	CCJGW_ResponseScanWlanJSON::CCJGW_ResponseScanWlanJSON(void)
	{
	}


	CCJGW_ResponseScanWlanJSON::~CCJGW_ResponseScanWlanJSON(void)
	{
	}

	std::string CCJGW_ResponseScanWlanJSON::BuildScanWlanJson(std::map<std::string, int>& mapSSID)
	{
		if (!bStatus)
		{
			return BuildResponseBaseJson();
		}
		else
		{
			BuildResponseBaseJson();
			boost::property_tree::ptree ptreeSSID;
			for (std::map<std::string, int>::iterator it = mapSSID.begin();
				it != mapSSID.end();
				++ it)
			{
				boost::property_tree::ptree ptemptree;
				ptemptree.put_value(it->first);
				ptreeSSID.push_back(std::make_pair("", ptemptree));
				//ptreeSSID.put_child("",array1);
			}
			mJsonTree.put_child("ssidArray",ptreeSSID);

			std::stringstream s2;  
			write_json(s2, mJsonTree);  
			return s2.str();
		}
	}
}