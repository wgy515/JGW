#pragma once
#include "TSEServerBaseJson.h"

namespace JGW
{
    class CTSETestIndexChangeJson : public CTSEServerBaseJson
    {
    public:
        CTSETestIndexChangeJson(void);
        ~CTSETestIndexChangeJson(void);
    public:
        bool FromJosn(CCJGW_FTM_Json& msgJson);
    public:
        int TestIndex;
        int SubIndex;
    };

}

