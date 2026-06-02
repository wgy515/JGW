#include "StdAfx.h"
#include "CJGW_DutTxDetailSettings.h"

namespace JGW
{
    CCJGW_DutTxDetailSettings::CCJGW_DutTxDetailSettings(void)
    {
        phyId = 0u;
        channel = 2412u;
        cbState = WLAN_Gen6_ChannelBondingState_none;
        rate = RATE_54Mbps;
        pwrMode = TxPowerAuto;
        txMode = ContTx99;
        txChainMask = WLAN_CHAIN_1;
        powerLevel = 10.0;
        gain = 10u;
        digitalGain = 0;
        PASetting = 0u;
        payLoadSize = 1500u;
        short11b_nGuard = false;
        ldpcRate = false;
        bSTBCmode = false;
        aggregation = 1u;
        dpdMode = DPD_Enabled;
        HC_Enable = HCS_Enabled;
        ifs = 0;
        dutyCycle = 90u;
        nPattern = PN9_PATTERN;
        broadcastMode = true;
        nPacket = 1000u;
        iAIFSN = 0u;
        scramblerDisable = false;
        channel2 = 0u;
        antenna = 0u;
        phy165Mode = 0u;
    }


    CCJGW_DutTxDetailSettings::~CCJGW_DutTxDetailSettings(void)
    {
    }
}

