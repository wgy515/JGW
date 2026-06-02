#pragma once
#include "GlobalEnvironment\TSE_GlobalEnvironment.h"
#include "LogService\TSE_LogServices.h"
#include "TestStatusService\TSE_TestStatusService.h"

namespace JGW
{
    class CTSE_SequenceResourceManage
    {
    public:
        CTSE_SequenceResourceManage(void);
        ~CTSE_SequenceResourceManage(void);
    public:
        void SetOnTestStatusListener(CTSE_TestStatusListener* pListener);

        void RemoveOnTestStatusListener(CTSE_TestStatusListener* pListener);

        void SetOnLogListener(CTSE_LogListener* pListener);

        void RemoveOnLogListener(CTSE_LogListener* pListener);

        CTSE_GlobalEnvironment& GetGlobalEnvironment();

        CTSE_LogServices& GetLogServices();

        CTSE_TestStatusService& GetTestStatusService();
    private:
        CTSE_LogServices mLogServices;
        CTSE_TestStatusService mTestStatusService;
        CTSE_GlobalEnvironment mGlobalEnvironment;
    };
}