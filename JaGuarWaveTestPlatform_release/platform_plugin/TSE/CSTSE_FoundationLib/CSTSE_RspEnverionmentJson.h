#pragma once
#include "CSTSE_RsponseTSEInfoJson.h"
#include <map>
namespace JGW
{
    class CCSTSE_RspEnverionmentJson : public CCSTSE_ResponseBaseJson
    {
    public:
        CCSTSE_RspEnverionmentJson(void);
        ~CCSTSE_RspEnverionmentJson(void);
    public:
        std::string GetRspEnverionmentJson();
        bool ParseRspEnverionmentJson(const std::string& strJson);
    public:
        std::map<std::string,std::string> EnvironmentVariables;
    };
}


