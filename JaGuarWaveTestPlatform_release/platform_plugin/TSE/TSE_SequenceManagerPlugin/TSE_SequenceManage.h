#pragma once
#include <TSE_SequenceManagerPlugin/TSE_SequenceManageImpl.h>
#include "TSE_LogServices.h"
#include "TSE_TestStatusService.h"
#include "TSE_GlobalEnvironment.h"
#include "TSE_SequenceTest.h"
namespace JGW
{
    class CTSE_SequenceManage : public CTSE_SequenceManageImpl
    {
    public:
        CTSE_SequenceManage(unsigned long tokenID);
        ~CTSE_SequenceManage(void);
    private:
        unsigned long GetTokenID();

        void SetOnTestStatusListener(CTSE_TestStatusListener* pListener);

        void RemoveOnTestStatusListener(CTSE_TestStatusListener* pListener);

        void SetOnLogListener(CTSE_LogListener* pListener);

        void RemoveOnLogListener(CTSE_LogListener* pListener);

        CTSE_GlobalEnvironmentImpl* GetGlobalEnvironmentImpl();

        CTSE_SequenceTestImpl* GetSequenceTestImpl();
    private:
        unsigned long mTokenID;
        CTSE_LogServices mLogServices;
        CTSE_TestStatusService mTestStatusService;
        CTSE_GlobalEnvironment mGlobalEnvironment;
        CTSE_SequenceTestImpl* mpSequenceTest;
    };
}