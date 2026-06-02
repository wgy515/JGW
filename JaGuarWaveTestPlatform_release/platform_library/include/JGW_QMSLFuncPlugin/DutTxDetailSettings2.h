#pragma once
#include "jgw_device_api_define.h"
#include "CCJGW_WCNTestDefine.h"

namespace JGW
{
    class DutTxDetailSettings2
    {
    public:
        uint wifiStandard;

        uint phyId;

        uint channel2;

        uint channel;

        uint nss;

        WLAN_Gen6_ChannelBondingState cbState;

        WLAN_UniversalDataRate rate;

        WLAN_UniversalRateBW rateBw;

        TxPowerMode pwrMode;

        WLAN_TX_Mode txMode;

        uint txChainMask;

        double powerLevel;

        uint gain;

        int digitalGain;

        uint PASetting;

        uint payLoadSize;

        bool short11b_nGuard;

        bool ldpcRate;

        bool bSTBCmode;

        uint aggregation;

        DPDSetting dpdMode;

        HCSetting HC_Enable;

        int ifs;

        uint dutyCycle;

        TxDataPattern nPattern;

        char* pattern;

        bool broadcastMode;

        uint nPacket;

        uint iAIFSN;

        bool scramblerDisable;

        bool ackRequest;

        WLAN_UniversalGuardInterval guardInterval;
    public:
        DutTxDetailSettings2()
        {
            wifiStandard = 0u;
            phyId = 0u;
            channel = 2412u;
            cbState = primary20;
            rate = RATE_MCS_0;
            rateBw = RateBW_11N_HT20;
            pwrMode = TxPowerAuto;
            txMode = ContTx99;
            txChainMask = 1u;
            powerLevel = 10.0;
            gain = 10u;
            digitalGain = 0;
            PASetting = 0u;
            payLoadSize = 1500u;
            short11b_nGuard = false;
            ldpcRate = false;
            bSTBCmode = false;
            aggregation = 1u;
            dpdMode = DPD_Disabled;
            HC_Enable = HCS_Disabled;
            ifs = 0;
            dutyCycle = 90;
            nPattern = PN9_PATTERN;
            broadcastMode = true;
            nPacket = 1000;
            iAIFSN = 0;
            scramblerDisable = false;
            channel2 = 0;
            nss = 1;
            ackRequest = false;
            guardInterval = LTF_Mode0_GI_800;
        }
    };
}