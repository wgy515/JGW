#include "StdAfx.h"
#include "TSE_TestStatusService.h"

namespace JGW
{
    void CTSE_TestStatusService::TestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus)
    {
        AddAwaitListener();
        std::vector<CTSE_TestStatusListener*>::iterator it = mvListener.begin();
        for (;it != mvListener.end();it ++)
        {
            it[0]->OnTestStatusChange(strTestName,eTestStatus);
        }
    }

    void CTSE_TestStatusService::TestIndexChange(size_t index,size_t subIndex)
    {
        AddAwaitListener();
        std::vector<CTSE_TestStatusListener*>::iterator it = mvListener.begin();
        for (;it != mvListener.end();it ++)
        {
            it[0]->OnTestIndexChange(index,subIndex);
        }
    }
}