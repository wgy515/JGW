#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_IPQ807xTxTest : public CTSE_TestBase
    {
    public:
        CTSE_IPQ807xTxTest(void);
        ~CTSE_IPQ807xTxTest(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        PhyID mePhyID;
        WLAN_UniversalRateBW meRateBw;
        WLAN_Gen6_ChannelBondingState meChannelBondingState;
        bool mbWlanTxEvmTest;
        bool mbWlanTxVerifyMaskTestSegmentMargins;
        uint muPowerLevel;
        uint muExpectPower;
        std::string mstrTestLabelName;
        std::wstring mstrTxTestParamPathName;
        std::wstring mstrTxTestingStandardPathName;
        //! chainmask channel -  rate
        std::vector<WLAN_CHAIN_MASK2> mvChainMask;
        std::vector<uint> mvChannel;
        std::vector<WLAN_UniversalDataRate> mvUniversalDataRate;
        //! TestLabelName,
        //std::map<std::string,>
    };
}

