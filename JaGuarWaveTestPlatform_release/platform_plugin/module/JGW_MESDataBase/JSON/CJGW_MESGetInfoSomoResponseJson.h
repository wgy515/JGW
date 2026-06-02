#pragma once
#include "CJGW_MESResponseBaseJson.h"

namespace JGW
{
    class CCJGW_MESGetInfoSomoResponseJson : public CCJGW_MESResponseBaseJson
    {
    public:
        CCJGW_MESGetInfoSomoResponseJson(void);
        ~CCJGW_MESGetInfoSomoResponseJson(void);

        bool FromJosn(CCJGW_FTM_Json& msgJson);
    public:
        std::string mstrTaskNumber;
    };
}