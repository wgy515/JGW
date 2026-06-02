#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_WlanTxEvmTest : public CTSE_TestBase
    {
    public:
        CTSE_WlanTxEvmTest(void);
        ~CTSE_WlanTxEvmTest(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        size_t m_channel;
        int m_powerLevel;
        WLAN_RATE m_wlanRate;
        ChannelEstimationOption m_chEstimationMethod;
        int m_numOfMIMOStream;
        int m_numAverages;
        int m_vsaTriggerLevel;
        bool m_shortGuardInterval;
        WLAN_CHAIN_MASK2 m_txChainMask;

        double m_evm[2];
        double m_amplErr[2];
        double m_dataRateMB[2];
        double m_symbolClockError[2];
        double m_freqErr[2];
        double m_phaseErr[2];
        double m_avgTxPower[2];
        double m_avgTxPowerDelta[2];
        double m_loLeakage[2];
        double m_minEvm[2];
        double m_maxEvm[2];
    };
}


