#pragma once
#include "TSE_TestStatusListener.h"

namespace JGW
{
    class CTSE_TestStatusServiceImpl
    {
    public:
        virtual void TestIndexChange(size_t index,size_t subIndex = -1) = 0;
        virtual void TestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus) = 0;
    };
}