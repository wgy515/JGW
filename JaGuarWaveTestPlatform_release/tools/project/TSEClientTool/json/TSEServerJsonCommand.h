#pragma once
#include "JsonEnumDefine.h"
#include <string>
#include <TSE_SequenceManagerPlugin/TSE_TestStatusListener.h>
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
namespace JGW
{
    class CTSEServerJsonCommand
    {
    public:
        CTSEServerJsonCommand(void);
        ~CTSEServerJsonCommand(void);
    public:
        //! 
        static std::string GetBaseJsonCommand(ActionJsonEnum actionJsonEnum = Unknown_AJE);
        //!
        static std::string GetFlagsJsonCommand(const std::string& strFlags);
        //! {"ActionEnumID":2,"TestStatusEnumID":1,"TestName":""}
        static std::string GetTestStatusChangeJsonCommand(E_TEST_STATUS testStatusEnum,const std::string& strTestName);
        //! int TestIndex = 0;int SubIndex
        static std::string GetTestIndexChangeJsonCommand(int testIndex, int subIndex = -1);
        //! LogTypeEnum LogTypeEnumID,string TestLog,UserDefineLogTypeEnum UserDefineID
        static std::string GetTestLogJsonCommand(TSELogTypeEnum tseLogTypeEnum,const std::string& strTestLog,E_USER_DEFINE_LOG_TYPE userDefineID);
        //! TestGuid std::map<std::string,std::string> Params;
        static std::string GetTestControlCmdJson(const std::string& strTestGuid);
    };
}


