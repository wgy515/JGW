#pragma once
#include "CJGW_RequestBaseJson.h"

namespace JGW
{
	class CCJGW_RequestConnectWlanJson : public CCJGW_RequestBaseJson
	{
	public:
		CCJGW_RequestConnectWlanJson(void);
		~CCJGW_RequestConnectWlanJson(void);

		bool FromJosn(CCJGW_FTM_Json& msgJson);
	public:
		std::string ssidname;
		std::string passwrod;
	};
}