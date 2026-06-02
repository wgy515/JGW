#pragma once
#include "CJGW_FTM_Json.h"
#include "JsonEnumDefine.h"

namespace JGW
{
    class CTSEServerBaseJson
    {
    public:
        CTSEServerBaseJson(void);
        ~CTSEServerBaseJson(void);
    public:
        virtual bool FromJosn(CCJGW_FTM_Json& msgJson);
    public:
        ActionJsonEnum ActionEnumID;
    };
}

