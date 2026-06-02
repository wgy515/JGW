#pragma once
#include <string>
#include "CJGW_CSharpBaseRequstJson.h"
#include "CJGW_FTM_Json.h"
namespace JGW
{
    class CCJGW_CheckSNStatusRequstJson : public CCJGW_CSharpBaseRequstJson
    {
    public:
        CCJGW_CheckSNStatusRequstJson(void);
        ~CCJGW_CheckSNStatusRequstJson(void);
    public:
        bool FromJosn(CCJGW_FTM_Json& msgJson);
    public:
        bool IsSMTMoNo;
        std::string SN;
        std::string OperateID;
    };
}


