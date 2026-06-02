#pragma once
#include "TSE_FTM_TestBase.h"

namespace JGW
{
    /**
     * OTG пео╒
     */
//     public final static int REQ_OTG_CODE = 0x13;
// 
// private boolean otgMounted;
//     private long otgTotalBytes;
//     private long otgFreeBytes;
    class CTSE_FTM_OTGTest : public CTSE_FTM_TestBase
    {
    public:
        CTSE_FTM_OTGTest(void);
        ~CTSE_FTM_OTGTest(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_Run();
    };
}


