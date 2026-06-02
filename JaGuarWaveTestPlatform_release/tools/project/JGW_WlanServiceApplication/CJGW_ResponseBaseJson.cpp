#include "StdAfx.h"
#include "CJGW_ResponseBaseJson.h"

namespace JGW
{
	CCJGW_ResponseBaseJson::CCJGW_ResponseBaseJson(void) : bStatus(false),responseType(E_WLAN_UNKNOW_TYPE),returnMessage("")
	{
	}


	CCJGW_ResponseBaseJson::~CCJGW_ResponseBaseJson(void)
	{
	}

	std::string CCJGW_ResponseBaseJson::BuildResponseBaseJson()
	{
		mJsonTree.put("bStatus",bStatus);
		mJsonTree.put("responseType",responseType);
		mJsonTree.put("returnMessage",returnMessage);
		std::stringstream s2;  
		write_json(s2, mJsonTree);  
		return s2.str();  
	}
}