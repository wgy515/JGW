#pragma once
#include "CJGW_RequestBaseJson.h"

namespace JGW
{
    class CCJGW_ReqPumasCommadJson : public CCJGW_RequestBaseJson
    {
    public:
        CCJGW_ReqPumasCommadJson(void);
        ~CCJGW_ReqPumasCommadJson(void);
    public:
        bool FromJosn(CCJGW_FTM_Json& msgJson);
    public:
        std::string command;
    };
}