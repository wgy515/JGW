#pragma once
#include <TSE_SequenceManagerPlugin/TSE_TestStatusListener.h>
#include <TSE_SequenceManagerPlugin/TSE_Listener.h>
#include <TSE_SequenceManagerPlugin/TSE_TestStatusServiceImpl.h>
#include <vector>

namespace JGW
{
    class CTSE_TestStatusService:public CTSE_Listener<CTSE_TestStatusListener>,public CTSE_TestStatusServiceImpl
    {
    public:
        void TestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus);
        void TestIndexChange(size_t index,size_t subIndex = -1);
    };
}