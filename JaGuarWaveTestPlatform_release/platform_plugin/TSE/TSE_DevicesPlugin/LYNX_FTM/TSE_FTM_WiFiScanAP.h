#pragma once
#include "TSE_FTM_TestBase.h"

namespace JGW
{
    class CTSE_FTM_WiFiScanAP : public CTSE_FTM_TestBase
    {
    public:
        CTSE_FTM_WiFiScanAP(void);
        ~CTSE_FTM_WiFiScanAP(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool mb11AD;
        int mnMinRSSI;
        //! size_t mIntervalTimeMS;
        size_t mminAPCounts; //!     
    };
}