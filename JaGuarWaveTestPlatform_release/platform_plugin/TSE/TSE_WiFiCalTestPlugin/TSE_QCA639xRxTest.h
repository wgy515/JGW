#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_QCA639xRxTest : public CTSE_TestBase
    {
    public:
        CTSE_QCA639xRxTest(void);
        ~CTSE_QCA639xRxTest(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Init();
        bool TSE_Run();
    private:
        bool mbldpcRate;
        LINK_DIRECTION meLinkDirection;
        PPDU_TYPE mePpduType;

        WLAN_UniversalPhyID mePhyID;
        //! channel -> mode -> nss -> mcs -> chain mask
        std::vector<size_t> mvChannel;
        //! mode
        std::vector<WLAN_UniversalRateBW> mvUniversalRateBW;
        std::vector<WLAN_Gen6_ChannelBondingState> mvChannelBondingState;
        //! nss
        std::vector<WLAN_UniversalNSS> mvUniversalNSS;
        //! mcs
        std::vector<WLAN_UniversalDataRate> mvUniversalDataRate;
        //! chain mask
        std::vector<WLAN_CHAIN_MASK2> mvChainMask2;
        //!
        int mnSingleTestErrorRetryCount;
        //!
        int mnPacketCount;
        //!
        float mfSignalLeveldBm;
        //! 
        std::vector<float> mvPERUL;
        //std::vector<float> mvPERPowerUL;
        std::vector<float> mvRssiLL;
    };
}
