#pragma once
#include <string>
#include <CSTSE_FoundationLib/CSTSE_ActionDefine.h>
#include <TSE_SequenceManagerPlugin/TSE_TestStatusListener.h>

namespace JGW
{
    class CCSTSE_TestStatusChangeJson
    {
    public:
        CCSTSE_TestStatusChangeJson(void);
        ~CCSTSE_TestStatusChangeJson(void);
    public:
        std::string GetTestStatusChangeJson();
        bool ParseTestStatusChangeJson(const std::string& strJson);
    public:
        ECSTSE_ACTION CSTSEAction;
        E_TEST_STATUS TestStatus;
        std::string TestName;
    };

}

