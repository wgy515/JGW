#pragma once
#include "TSE_FTM_TestBase.h"

namespace JGW
{
    class CTSE_FTM_ADScanAP : public CTSE_FTM_TestBase
    {
    public:
        CTSE_FTM_ADScanAP(void);
        ~CTSE_FTM_ADScanAP(void);
    private:
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        size_t mminADCounts; //!
        int mnMinRSSI;
    };
}