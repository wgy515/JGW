#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "JGW_Phone.h"
namespace JGW
{


    class CTSE_IPQ40xxTxCalTest :public CTSE_TestBase
    {
    public:
        CTSE_IPQ40xxTxCalTest(void);
        ~CTSE_IPQ40xxTxCalTest(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
        bool TSE_Init();
    private:
        bool CheckEVM(double val);
        bool CheckAmplErr(double val);
        bool CheckDataRateMB(double val);
        bool CheckSymbolClockError(double val);
        bool CheckFreqErr(double val);
        bool CheckPhaseErr(double val);
        bool CheckAvgTxPower(double val,double fExpectPower);
        //bool CheckAvgTxPowerDelta(double val);
        bool CheckLoLeakager(double val);
    private:
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
        std::vector<float> mvfEvmLL;
        std::vector<float> mvfEvmUL;
        //! 
        std::wstring mstrTxTestParamPathName;
        //!
        std::wstring mstrTxTestingStandardPathName;
        //! 
        S_TxTestParam msTxTestParam;
        //!
        S_TX_TestingStandard msTxTestingStandard;
    };
}

