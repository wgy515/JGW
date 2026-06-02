#pragma once
#include "TSE_FTM_TestBase.h"

namespace JGW
{
    class CTSE_HandoverADMode : public CTSE_FTM_TestBase
    {
    public:
        CTSE_HandoverADMode(void);
        ~CTSE_HandoverADMode(void);
    private:
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Run();
    private:
        int mADMode;
    };
}