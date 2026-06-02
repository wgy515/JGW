#include "StdAfx.h"
#include "TSEServerJsonCommand.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CTSEServerJsonCommand::CTSEServerJsonCommand(void)
    {
    }


    CTSEServerJsonCommand::~CTSEServerJsonCommand(void)
    {
    }
    //! {"ActionEnumID":-1}
    std::string CTSEServerJsonCommand::GetBaseJsonCommand(ActionJsonEnum actionJsonEnum /* = Unknown_AJE */)
    {
        return JGW_GetFormatString("{\"ActionEnumID\":%d}",(int)actionJsonEnum);
    }
    //! {"ActionEnumID":1,"Flags":"11111111"}
    std::string CTSEServerJsonCommand::GetFlagsJsonCommand(const std::string& strFlags)
    {
        return JGW_GetFormatString("{\"ActionEnumID\":%d,\"Flags\":\"%s\"}",(int)Flags_AJE,strFlags.c_str());
    }
    //! {"ActionEnumID":2,"TestStatusEnumID":1,"TestName":""}
    std::string CTSEServerJsonCommand::GetTestStatusChangeJsonCommand(E_TEST_STATUS testStatusEnum,const std::string& strTestName)
    {
        return JGW_GetFormatString("{\"ActionEnumID\":%d,\"TestStatusEnumID\":%d,\"TestName\":\"%s\"}",(int)TestStatusChange_AJE,(int)testStatusEnum,strTestName.c_str());
    }
    //! {"ActionEnumID":3,"TestIndex":1,"SubIndex":1}
    std::string CTSEServerJsonCommand::GetTestIndexChangeJsonCommand(int testIndex, int subIndex /* = -1 */)
    {
        return JGW_GetFormatString("{\"ActionEnumID\":%d,\"TestIndex\":%d,\"SubIndex\":%d}",(int)TestIndexChange_AJE,testIndex,subIndex);
    }
    //! {"ActionEnumID":4,"LogTypeEnumID":1,"TestLog":"","UserDefineID":1}
    std::string CTSEServerJsonCommand::GetTestLogJsonCommand(TSELogTypeEnum tseLogTypeEnum,const std::string& strTestLog,E_USER_DEFINE_LOG_TYPE userDefineID)
    {
        return JGW_GetFormatString("{\"ActionEnumID\":%d,\"LogTypeEnumID\":%d,\"TestLog\":\"%s\",\"UserDefineID\":%d}",(int)TestLog_AJE,(int)tseLogTypeEnum,strTestLog.c_str(),(int)userDefineID);
    }
    //! {"ActionEnumID":4,"TestGuid":"","Params":""}
    std::string CTSEServerJsonCommand::GetTestControlCmdJson(const std::string& strTestGuid)
    {
        return JGW_GetFormatString("{\"ActionEnumID\":%d,\"TestGuid\":\"\",\"Params\":\"\"}",(int)ControlCmd_AJE,strTestGuid.c_str());
    }
}

