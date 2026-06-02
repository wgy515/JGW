#pragma once
#include "CJGW_FTM_Json.h"
#include "CJGW_CSharpBaseRequstJson.h"

namespace JGW
{
    class CCJGW_UpdateLogReqJson : CCJGW_CSharpBaseRequstJson
    {
    public:
        CCJGW_UpdateLogReqJson(void);
        ~CCJGW_UpdateLogReqJson(void);
    public:
        virtual bool FromJosn(CCJGW_FTM_Json& msgJson);
    public:
        int Level;
        std::string Log;
        std::wstring strwLog;
    };
}

