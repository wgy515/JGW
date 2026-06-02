#include "StdAfx.h"
#include "CJGW_RequestConnectWlanJson.h"

namespace JGW
{
	CCJGW_RequestConnectWlanJson::CCJGW_RequestConnectWlanJson(void)
	{
	}


	CCJGW_RequestConnectWlanJson::~CCJGW_RequestConnectWlanJson(void)
	{
	}

	bool CCJGW_RequestConnectWlanJson::FromJosn(CCJGW_FTM_Json& msgJson)
	{
		 msgJson.GetJsonValueToKey<std::string>("ssidname",ssidname);
		 msgJson.GetJsonValueToKey<std::string>("passwrod",passwrod);
		return true;
	}
}
