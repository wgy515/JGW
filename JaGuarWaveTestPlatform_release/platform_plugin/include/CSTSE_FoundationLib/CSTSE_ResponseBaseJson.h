#pragma once
#include <string>
#include "CSTSE_BaseJson.h"

namespace JGW
{
    class CCSTSE_ResponseBaseJson : public CCSTSE_BaseJson
    {
    public:
        CCSTSE_ResponseBaseJson(ECSTSE_ACTION action);
        ~CCSTSE_ResponseBaseJson(void);
    public:
        virtual bool ParseResponseBaseJson(const std::string& strJson);
        std::string GetResponseBaseJson();
    public:
        //! result
        bool Result;
        //! error msg
        std::string ErrorMsg;
    };
}


