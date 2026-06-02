#pragma once
#include "TSEServerBaseJson.h"
namespace JGW
{
    class CTSEServerFlagsJson : public CTSEServerBaseJson
    {
    public:
        CTSEServerFlagsJson(void);
        ~CTSEServerFlagsJson(void);
    public:
        bool FromJosn(CCJGW_FTM_Json& msgJson);
    public:
        std::string Flags;
    };
}