#pragma once
#include <JGW_QMSLFuncPlugin/CCJGW_WCNTestDefine.h>
#include <vector>
namespace JGW
{
    class CCJGW_DutTxDetailSettings
    {
    public:
        CCJGW_DutTxDetailSettings(void);
        ~CCJGW_DutTxDetailSettings(void);
    public:
        // Token: 0x040002FC RID: 764
         uint phyId;

        // Token: 0x040002FD RID: 765
         uint channel2;

        // Token: 0x040002FE RID: 766
         uint channel;

        // Token: 0x040002FF RID: 767
         WLAN_Gen6_ChannelBondingState cbState;

        // Token: 0x04000300 RID: 768
         WLAN_DataRate rate;

        // Token: 0x04000301 RID: 769
         TxPowerMode pwrMode;

        // Token: 0x04000302 RID: 770
         WLAN_TX_Mode txMode;

        // Token: 0x04000303 RID: 771
         WLAN_CHAIN_MASK2 txChainMask;

        // Token: 0x04000304 RID: 772
         double powerLevel;

        // Token: 0x04000305 RID: 773
         uint gain;

        // Token: 0x04000306 RID: 774
         int digitalGain;

        // Token: 0x04000307 RID: 775
         uint PASetting;

        // Token: 0x04000308 RID: 776
         uint payLoadSize;

        // Token: 0x04000309 RID: 777
         bool short11b_nGuard;

        // Token: 0x0400030A RID: 778
         bool ldpcRate;

        // Token: 0x0400030B RID: 779
         bool bSTBCmode;

        // Token: 0x0400030C RID: 780
         uint aggregation;
         //! Enabled=0;Disabled=1
        // Token: 0x0400030D RID: 781
         DPDSetting dpdMode;
         //! Enabled=0;Disabled=1
        // Token: 0x0400030E RID: 782
         HCSetting HC_Enable;

        // Token: 0x0400030F RID: 783
         int ifs;

        // Token: 0x04000310 RID: 784
         uint dutyCycle;

        // Token: 0x04000311 RID: 785
         TxDataPattern nPattern;

        // Token: 0x04000312 RID: 786
         ///byte[] pattern;
         std::vector<byte> pattern;

        // Token: 0x04000313 RID: 787
         bool broadcastMode;

        // Token: 0x04000314 RID: 788
         uint nPacket;

        // Token: 0x04000315 RID: 789
         uint iAIFSN;

        // Token: 0x04000316 RID: 790
         bool scramblerDisable;

        // Token: 0x04000317 RID: 791
         uint antenna;

        // Token: 0x04000318 RID: 792
         uint phy165Mode;
    };
}


