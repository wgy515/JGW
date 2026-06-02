#pragma once
#include <TSE_SequenceManagerPlugin/TSE_TestStatusListener.h>

namespace JGW
{
    class CCSTSE_TestStatusListenerEmpty : public CTSE_TestStatusListener
    {
    public:
        static CCSTSE_TestStatusListenerEmpty* GetTestStatusListenerEmpty();
    private:
        CCSTSE_TestStatusListenerEmpty(void);
        ~CCSTSE_TestStatusListenerEmpty(void);
    private:
        //virtual void OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus);
        //virtual void OnTestIndexChange(size_t index,int subIndex = -1);
    };

}

