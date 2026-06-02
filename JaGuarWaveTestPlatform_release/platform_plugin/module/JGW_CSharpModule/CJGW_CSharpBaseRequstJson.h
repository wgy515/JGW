#pragma once
#include <string>
#include "CJGW_CSharpModuleDefine.h"
#include "CJGW_FTM_Json.h"

namespace JGW
{
    class CCJGW_CSharpBaseRequstJson
    {
    public:
        CCJGW_CSharpBaseRequstJson(void);
        ~CCJGW_CSharpBaseRequstJson(void);
    public:
         virtual bool FromJosn(CCJGW_FTM_Json& msgJson);
    public:
        E_CSM_REQ_ID RequestID;
    };
}
