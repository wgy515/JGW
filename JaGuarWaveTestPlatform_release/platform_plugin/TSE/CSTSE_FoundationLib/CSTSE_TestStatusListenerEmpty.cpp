#include "StdAfx.h"
#include "CSTSE_TestStatusListenerEmpty.h"

namespace JGW
{
    CCSTSE_TestStatusListenerEmpty::CCSTSE_TestStatusListenerEmpty(void)
    {
    }


    CCSTSE_TestStatusListenerEmpty::~CCSTSE_TestStatusListenerEmpty(void)
    {
    }

    CCSTSE_TestStatusListenerEmpty* CCSTSE_TestStatusListenerEmpty::GetTestStatusListenerEmpty()
    {
        static CCSTSE_TestStatusListenerEmpty testStatusListenerEmpty;
        return &testStatusListenerEmpty;
    }

    //void CCSTSE_TestStatusListenerEmpty::OnTestIndexChange(size_t index,int subIndex /* = -1 */)
    //{

    //}

    //void CCSTSE_TestStatusListenerEmpty::OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus)
    //{

    //}
}
