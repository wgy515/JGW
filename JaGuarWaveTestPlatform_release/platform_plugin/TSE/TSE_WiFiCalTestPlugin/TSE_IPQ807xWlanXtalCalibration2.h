#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_IPQ807xWlanXtalCalibration2 : public CTSE_TestBase
    {
    public:
        CTSE_IPQ807xWlanXtalCalibration2(void);
        ~CTSE_IPQ807xWlanXtalCalibration2(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        PhyID mePhyId;
        size_t mnChannel;
        WLAN_SINGLE_CHAIN_MASK meTxChainMask;
        size_t mnGainIndex;
        int mnDacGain;
        int mnPaCfg;
        size_t mnNumAverages;
    };
}

