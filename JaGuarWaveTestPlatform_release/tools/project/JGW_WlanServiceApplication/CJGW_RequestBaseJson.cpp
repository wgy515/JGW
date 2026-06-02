#include "StdAfx.h"
#include "CJGW_RequestBaseJson.h"

namespace JGW
{
	CCJGW_RequestBaseJson::CCJGW_RequestBaseJson(void) : requestType(E_WLAN_UNKNOW_TYPE)
	{
	}


	CCJGW_RequestBaseJson::~CCJGW_RequestBaseJson(void)
	{
	}

	bool CCJGW_RequestBaseJson::FromJosn(const std::string& strJson)
	{
		//! std::stringstream msg_ss(strJson);
		CCJGW_FTM_Json ftmJson;
		if (!ftmJson.FromJsonToString(strJson)) return false;;
		return FromJosn(ftmJson);
	}

	bool CCJGW_RequestBaseJson::FromJosn(CCJGW_FTM_Json& msgJson)
	{
		msgJson.GetJsonValueToKey<int>("requestType",requestType);
		return true;
	}
}