#pragma once
#include <string>
#include <CSTSE_FoundationLib/CSTSE_BaseJson.h>

namespace JGW
{
    class CCSTSE_GetTSEInfoJson : public CCSTSE_BaseJson
    {
    public:
        CCSTSE_GetTSEInfoJson(void);
        ~CCSTSE_GetTSEInfoJson(void);
    public:
        std::string GetTSEInfoJson();
    };
}


