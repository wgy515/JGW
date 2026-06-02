#pragma once
#include <TSE_SequenceManagerPlugin/TSE_TestStatusListener.h>
namespace JGW
{
    class CTSE_TestStatusLog : public CTSE_TestStatusListener
    {
    public:
        virtual void OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus);
        virtual void OnTestIndexChange(size_t index,int subIndex = -1);
    };
}