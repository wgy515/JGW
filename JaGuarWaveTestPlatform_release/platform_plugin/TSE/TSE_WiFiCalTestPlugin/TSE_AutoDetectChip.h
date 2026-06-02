#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_AutoDetectChip : public CTSE_TestBase
    {
    public:
        CTSE_AutoDetectChip(void);
        ~CTSE_AutoDetectChip(void);
    private:
        bool TSE_Run();
    };

}

