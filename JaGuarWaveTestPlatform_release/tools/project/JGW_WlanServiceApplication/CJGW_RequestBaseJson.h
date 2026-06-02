#pragma once
#include "CJGW_FTM_Json.h"

namespace JGW
{
	class CCJGW_RequestBaseJson
	{
	public:
		CCJGW_RequestBaseJson(void);
		~CCJGW_RequestBaseJson(void);
	public:
		bool FromJosn(const std::string& strJson);
		bool FromJosn(CCJGW_FTM_Json& msgJson);
	public:
		int requestType;
	};
}