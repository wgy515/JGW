#include "StdAfx.h"
#include "TSE_SequenceResourceManage.h"

namespace JGW
{
    CTSE_SequenceResourceManage::CTSE_SequenceResourceManage(void)
    {
    }


    CTSE_SequenceResourceManage::~CTSE_SequenceResourceManage(void)
    {
    }

    void CTSE_SequenceResourceManage::SetOnTestStatusListener(CTSE_TestStatusListener* pListener)
    {
        mTestStatusService.AddListener(pListener);
    }

    void CTSE_SequenceResourceManage::RemoveOnTestStatusListener(CTSE_TestStatusListener* pListener)
    {
        mTestStatusService.RemoveListener(pListener);
    }

    void CTSE_SequenceResourceManage::SetOnLogListener(CTSE_LogListener* pListener)
    {
        mLogServices.AddListener(pListener);
    }

    void CTSE_SequenceResourceManage::RemoveOnLogListener(CTSE_LogListener* pListener)
    {
        mLogServices.RemoveListener(pListener);
    }

    CTSE_GlobalEnvironment& CTSE_SequenceResourceManage::GetGlobalEnvironment()
    {
        return mGlobalEnvironment;
    }

    CTSE_LogServices& CTSE_SequenceResourceManage::GetLogServices()
    {
        return mLogServices;
    }

    CTSE_TestStatusService& CTSE_SequenceResourceManage::GetTestStatusService()
    {
        return mTestStatusService;
    }
}