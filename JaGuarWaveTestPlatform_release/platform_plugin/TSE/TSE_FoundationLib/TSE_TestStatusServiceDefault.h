#pragma once
#include <TSE_SequenceManagerPlugin/TSE_TestStatusServiceImpl.h>
namespace JGW
{
    class CTSE_TestStatusServiceDefault : public CTSE_TestStatusServiceImpl
    {
    public:
        void TestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus);
        void TestIndexChange(size_t index,size_t subIndex  = -1 );
    };
}


