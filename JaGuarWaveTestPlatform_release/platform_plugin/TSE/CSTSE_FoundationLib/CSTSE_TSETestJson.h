#pragma once
#include <CSTSE_FoundationLib/CSTSE_BaseJson.h>

namespace JGW
{
    class CCSTSE_TSETestJson : public CCSTSE_BaseJson
    {
    public:
        CCSTSE_TSETestJson(void);
        ~CCSTSE_TSETestJson(void);
    public:
        std::string GetTSETestJson();
        bool ParseTSETestJson(const std::string& strJson);
    public:
        int SuiteIndex; //! ²âÊÔÐòÁÐË÷Òý -1 : test all
        ECSTSE_TSE_TEST_TYPE SuiteTestType; //! 0 : auto 1: config 2: normal
    };
}


