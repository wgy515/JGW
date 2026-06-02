#pragma once
#include "TSEServerBaseJson.h"
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>

namespace JGW
{
    class CTSETestLogJson : public CTSEServerBaseJson
    {
    public:
        CTSETestLogJson(void);
        ~CTSETestLogJson(void);
    public:    
        bool FromJosn(CCJGW_FTM_Json& msgJson);
    public:
        TSELogTypeEnum LogTypeEnumID;
        std::string TestLog;
        E_USER_DEFINE_LOG_TYPE UserDefineID;
    };
}


