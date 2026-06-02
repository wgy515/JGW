#pragma once
#include <CSTSE_FoundationLib/CSTSE_ResponseBaseJson.h>
namespace JGW
{
    class CCSTSE_RsponseTSEInfoJson : public CCSTSE_ResponseBaseJson
    {
    public:
        CCSTSE_RsponseTSEInfoJson(void);
        ~CCSTSE_RsponseTSEInfoJson(void);
    public:
        bool ParseRsponseTSEInfoJson(const std::string& strJson);
    public:
        int SuiteTestCount;
    };
}


