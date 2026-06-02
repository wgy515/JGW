#pragma once
#include <string>
namespace JGW
{
    class CCJGW_StationSocketRequestJson
    {
    public:
        CCJGW_StationSocketRequestJson(void);
        ~CCJGW_StationSocketRequestJson(void);

    public:
        bool FromJsonToString(const std::string& strJson);

    public:
        int responseCode;
        std::string cmdStr;
        std::string ip;
        std::string time;
    };
}


