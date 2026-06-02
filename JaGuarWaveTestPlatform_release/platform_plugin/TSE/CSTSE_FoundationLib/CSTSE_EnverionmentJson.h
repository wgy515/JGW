#pragma once
#include <string>
#include <CSTSE_FoundationLib/CSTSE_BaseJson.h>
#include <map>

namespace JGW
{
    class CCSTSE_EnverionmentJson : public CCSTSE_BaseJson
    {
    public:
        CCSTSE_EnverionmentJson(void);
        ~CCSTSE_EnverionmentJson(void);
    public:
        //! 
        std::string GetEnverionmentJson();
        //! 
        bool ParseEnverionmentJson(const std::string& strJson);
    public:
        //! ÊÇ·ñ¸üÐÂ
        bool IsUpdate;
        //! env key - value
        std::map<std::string,std::string> EnvironmentVariables;
    };
}


