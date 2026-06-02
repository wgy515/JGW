#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "JGW_Phone.h"

namespace JGW
{
    class CTSE_IPQ40xxGuLossCal : public CTSE_TestBase
    {
    public:
        CTSE_IPQ40xxGuLossCal(void);
        ~CTSE_IPQ40xxGuLossCal(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
        bool TSE_Init();
    private:
        bool CheckAvgTxPower(double val,double fExpectPower);
    private:
        //! 是否保存金机校准数据
        bool mbSaveGuTestData;
        bool mbInitParam;
        bool mbTestEvm;
        bool mbTestMask;
        int mnSingleTestErrorRetryCount;
        int mnNumAverages;
        size_t mnPowerLevel;
        double mfExpectPower;
        std::vector<size_t> mvChannel;
        std::vector<WLAN_CHAIN_MASK2> mvTxChainMask;
        std::vector<WLAN_DataRate> mvDataRate;
        std::vector<WLAN_RATE> mvWlanRate;
        std::vector<WLAN_Gen6_ChannelBondingState> mvChannelBondingState;
        //! 
        std::wstring mstrTxTestParamPathName;
        //!
        std::wstring mstrTxTestingStandardPathName;
        //!
        std::wstring mstrSNEnvrionment;
        //!
        std::wstring mstrSaveGuCalDataFileEnv;
        //! 
        S_TxTestParam msTxTestParam;
        //!
        S_TX_TestingStandard msTxTestingStandard;
    };

}

