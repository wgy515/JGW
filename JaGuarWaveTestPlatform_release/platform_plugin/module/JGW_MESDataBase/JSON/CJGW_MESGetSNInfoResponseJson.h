#pragma once
#include "CJGW_MESResponseBaseJson.h"

namespace JGW
{
	class CCJGW_MESGetSNInfoResponseJson : public CCJGW_MESResponseBaseJson
	{
	public:
		CCJGW_MESGetSNInfoResponseJson(void);
		~CCJGW_MESGetSNInfoResponseJson(void);
	public:
		bool FromJosn(CCJGW_FTM_Json& msgJson);
	public:
        std::string mstrMONOType;
		std::string mstrTaskNumber;
		std::string mstrWorkOrderNumber;
	};
}