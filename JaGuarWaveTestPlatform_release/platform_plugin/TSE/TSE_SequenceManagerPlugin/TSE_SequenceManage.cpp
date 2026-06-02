#include "StdAfx.h"
#include "TSE_SequenceManage.h"
#include "TSE_SequenceTestFactory.h"
namespace JGW
{
    CTSE_SequenceManage::CTSE_SequenceManage(unsigned long tokenID) : mTokenID(tokenID),mpSequenceTest(NULL)
        /*,mSequenceTest(mLogServices,mTestStatusService,mGlobalEnvironment,tokenID)*/
    {

    }

    CTSE_SequenceManage::~CTSE_SequenceManage()
    {

    }

    unsigned long CTSE_SequenceManage::GetTokenID()
    {
        return mTokenID;
    }

    void CTSE_SequenceManage::SetOnTestStatusListener(CTSE_TestStatusListener* pListener)
    {
        mTestStatusService.AddListener(pListener);
    }

    void CTSE_SequenceManage::RemoveOnTestStatusListener(CTSE_TestStatusListener* pListener)
    {
        mTestStatusService.RemoveListener(pListener);
    }

    void CTSE_SequenceManage::SetOnLogListener(CTSE_LogListener* pListener)
    {
        mLogServices.AddListener(pListener);
    }

    void CTSE_SequenceManage::RemoveOnLogListener(CTSE_LogListener* pListener)
    {
        mLogServices.RemoveListener(pListener);
    }

    CTSE_GlobalEnvironmentImpl* CTSE_SequenceManage::GetGlobalEnvironmentImpl()
    {
        return &mGlobalEnvironment;
    }

    CTSE_SequenceTestImpl* CTSE_SequenceManage::GetSequenceTestImpl()
    {
        if (NULL == mpSequenceTest)
        {
            mpSequenceTest = new CTSE_SequenceTestFactory(mLogServices,mTestStatusService,mGlobalEnvironment,mTokenID);
        }
        return mpSequenceTest;
    }
}
