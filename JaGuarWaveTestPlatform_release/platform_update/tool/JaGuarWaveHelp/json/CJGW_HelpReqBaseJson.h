#pragma once
#include "CJGW_FTM_Json.h"

namespace JGW
{
    class CCJGW_HelpReqBaseJson
    {
    public:
        CCJGW_HelpReqBaseJson(void);
        ~CCJGW_HelpReqBaseJson(void);
    public:
        virtual bool FromJosn(const std::string& strJson);
        virtual bool FromJosn(CCJGW_FTM_Json& msgJson);
    public:
        int helpReqType;
    };
}