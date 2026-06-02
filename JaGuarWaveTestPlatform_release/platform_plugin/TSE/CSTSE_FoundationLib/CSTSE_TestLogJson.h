#pragma once
#include <CSTSE_FoundationLib/CSTSE_ActionDefine.h>
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
#include <string>
namespace JGW
{
    class CCSTSE_TestLogJson
    {
    public:
        CCSTSE_TestLogJson(void);
        ~CCSTSE_TestLogJson(void);
    public:
        std::string GetTestLogJson();
        bool ParseTestLogJson(const std::string& strJson);
    public: 
        ECSTSE_ACTION CSTSEAction;
        ECSTSE_TSE_TEST_LOG_TYPE LogType;
        E_USER_DEFINE_LOG_TYPE UserDefineType;
        std::string Log;
    };
}


