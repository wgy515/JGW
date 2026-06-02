#pragma once
#include <CSTSE_FoundationLib/CSTSE_ResponseBaseJson.h>

namespace JGW
{
    class CCSTSE_RspTSETestJson : public CCSTSE_ResponseBaseJson
    {
    public:
        CCSTSE_RspTSETestJson(void);
        ~CCSTSE_RspTSETestJson(void);
    public:
        int SuiteIndex; //! ²âÊÔÐòÁÐË÷Òý -1 : test all
    };
}