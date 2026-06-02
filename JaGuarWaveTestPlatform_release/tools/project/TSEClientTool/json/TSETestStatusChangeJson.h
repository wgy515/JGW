#pragma once
#include "TSEServerBaseJson.h"
#include <TSE_SequenceManagerPlugin/TSE_TestStatusListener.h>
namespace JGW
{
    class CTSETestStatusChangeJson : public CTSEServerBaseJson
    {
    public:
        CTSETestStatusChangeJson(void);
        ~CTSETestStatusChangeJson(void);
    public:
        bool FromJosn(CCJGW_FTM_Json& msgJson);
    public:
        E_TEST_STATUS TestStatusEnumID;
        std::string TestName;
    };
}

