#pragma once
#include <map>
#include "CJGW_ResponseBaseJson.h"
namespace JGW
{
	class CCJGW_ResponseScanWlanJSON : public CCJGW_ResponseBaseJson
	{
	public:
		CCJGW_ResponseScanWlanJSON();
		~CCJGW_ResponseScanWlanJSON(void);

		std::string BuildScanWlanJson(std::map<std::string,int>& mapSSID);
	};
}