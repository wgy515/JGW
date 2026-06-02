#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    //! ”√“«∆˜≤‚ ‘π¶¬ 
    class CTSE_WlanTxVerifyPowerTest : public CTSE_TestBase
    {
    public:
        CTSE_WlanTxVerifyPowerTest(void);
        ~CTSE_WlanTxVerifyPowerTest(void);
    private:     
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        size_t m_channel;
        int m_powerLevel;
        WLAN_RATE m_wlanRate;
        int m_numOfMIMOStream;
        int m_vsaTriggerLevel;
        bool m_shortGuardInterval;
        int m_numAverages;
        WLAN_CHAIN_MASK2 m_txChainMask;
        //! input val
        double m_txPowerAvg[2];
        double m_txPowerAvgDelta[2];
        double m_txPowerMax[2];
        double m_txPowerMin[2];
        double m_txPowerPeak[2];
        double m_txPowerPeakMax[2];
        double m_txPowerPeakMin[2];
        double m_cableLoss[2];
    };
}

