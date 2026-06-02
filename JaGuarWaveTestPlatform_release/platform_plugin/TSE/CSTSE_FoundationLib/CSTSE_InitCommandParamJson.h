#pragma once
#include <map>
#include <string>
#include <CSTSE_FoundationLib/CSTSE_BaseJson.h>

namespace JGW
{
    class CCSTSE_InitCommandParamJson : public CCSTSE_BaseJson
    {
    public:
        CCSTSE_InitCommandParamJson(void);
        ~CCSTSE_InitCommandParamJson(void);
    public:
        bool ParseInitCommandParamJson(const std::string& strJson);
        std::string GetInitCommandParamJson();
    public:
        //! init tse param
        std::map<std::string,std::string> Params;
    };
}


