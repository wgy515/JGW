#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <vector>
namespace JGW
{
    class CTSE_SetUpDutTxDetails : CTSE_TestBase
    {
    public:
        CTSE_SetUpDutTxDetails(void);
        ~CTSE_SetUpDutTxDetails(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        //! Channel Loop_20
        int m_channel;
        //! Chain loop Á´Â·
        WLAN_CHAIN_MASK2 m_txChainMask;
        //! Tx Rate Loop
        WLAN_Gen6_ChannelBondingState m_cbState;
        WLAN_DataRate m_rate;
        //! default param
        TxPowerMode m_pwrMode;
        WLAN_TX_Mode m_txMode;
        int m_powerLevel;
        int m_gain;
        int m_digitalGain;
        int m_PAcfg;
        int m_payLoadSize;
        bool m_short11b_nGuard;
        bool m_ldpcRate;
        bool m_stbcMode;
        int m_aggregation;
        DPDSetting m_dpdMode;
        HCSetting m_HC_Enable;
        int m_ifs;
        int m_dutyCycle;
        TxDataPattern m_nPattern;
        ///int m_pattern;
        std::vector<byte> m_pattern;
        int m_nPacket;
        bool m_broadcastMode;
        int m_AIFSN;
        int m_dutTxSettlingDelayMs;
        bool m_scramblerDisable;
        int m_channel2;
    };

    //! Channel Loop_20
    //! std::vector<int> mvChannel;
    //! Chain loop Á´Â·
    //! std::vector<WLAN_CHAIN_MASK2> mvWlanChainMask2;
    //! Tx Rate Loop
}


