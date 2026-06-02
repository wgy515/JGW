#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_IPQ807xDisConnectDut : public CTSE_TestBase
    {
    public:
        CTSE_IPQ807xDisConnectDut(void);
        ~CTSE_IPQ807xDisConnectDut(void);
    private:
        bool TSE_Run();
    };

}

