#pragma once
#include <string>

namespace JGW
{
    class CCJGW_GetIMEIInfoRequstJson
    {
    public:
        CCJGW_GetIMEIInfoRequstJson(void);
        ~CCJGW_GetIMEIInfoRequstJson(void);
    public:
        std::string IMEI;
        std::string OperationID;
        std::string SN;
    };
}


