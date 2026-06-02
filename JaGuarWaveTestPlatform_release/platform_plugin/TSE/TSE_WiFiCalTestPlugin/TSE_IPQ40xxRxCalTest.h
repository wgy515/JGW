#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_QMSLFuncPlugin/CCJGW_WCNTestDefine.h>
namespace JGW
{
    class CTSE_IPQ40xxRxCalTest : public CTSE_TestBase
    {
    public:
        CTSE_IPQ40xxRxCalTest(void);
        ~CTSE_IPQ40xxRxCalTest(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Init();
        bool TSE_Run();
    private:
        int mnSingleTestErrorRetryCount;
        size_t mnPacketCount;
        //float mfStartPower;
        //float mfStopPower;
        double mfSignalLeveldBm;
        //! Rx Chain_loop
        std::vector<int> mvRxChainMask;
        //! Rx Channel_loop
        std::vector<int> mvChannel;
        //! Rx Rate_loop
        std::vector<int> mvChannelBondingState;
        std::vector<int> mvRate;
        std::vector<float> mvPERUL;
        //std::vector<float> mvPERPowerUL;
        std::vector<float> mvRssiLL;
        //std::vector<float> mvRssiUL;
        S_RxCalTestParam msRxCalTestParam;
    };
}


