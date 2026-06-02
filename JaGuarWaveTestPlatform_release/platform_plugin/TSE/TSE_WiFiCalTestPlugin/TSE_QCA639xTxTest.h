#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "JGW_Phone.h"
namespace JGW
{
    typedef struct
    {
        int nBandWidth;//! WLAN_UniversalRateBW
        int mcs;//! WLAN_UniversalDataRate
        int nss; //! WLAN_UniversalNSS
        float gint;
        int ltft;
        std::string link;//! The WLAN signal link direction
        std::string strPPDU;//! The WLAN signal PLCP Protocol Data Unit (PPDU) type
        std::string strFEC;//! The WLAN signal forward error correction type: false = BCC, true = LPDC
        int dcm ;
        int nRefPower;//! ¶ÁÈ¡ÅäÖÃ²ÎÊý
        int trigLev;//! The offset of the vsaTriggerLevelDbm to the expected signal average power level from the DUT port in dBr
        int averCount;//! The number of samples to average from the signal burst
        std::string strWlanStand;
    }S_SCPI_PARAM;

    class CTSE_QCA639xTxTest : public CTSE_TestBase
    {
    public:
        CTSE_QCA639xTxTest(void);
        ~CTSE_QCA639xTxTest(void);
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
    public:
        static bool ConvertBandWidthMCSStanderToRata(WLAN_UniversalRateBW rateBW,WLAN_UniversalDataRate dataRate,WLAN_Gen6_ChannelBondingState cbState,S_SCPI_PARAM& sScpiPara,PPDU_TYPE& ePpduType);
    private:
        //! 5G TRUE  2.4G FALSE
        bool mbldpcRate;
        LINK_DIRECTION meLinkDirection;
        PPDU_TYPE mePpduType;

        WLAN_UniversalPhyID mePhyID;
        //! channel -> mode -> nss -> mcs -> chain mask -> power
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


        bool mbInitParam;
        bool mbTestEvm;
        bool mbTestMask;
        int mnSingleTestErrorRetryCount;
        int mnNumAverages;
        size_t mnPowerLevel;
        double mfExpectPower;//! == nRefPower
        int mnTestIntervals;
        //std::vector<size_t> mvChannel;
        //std::vector<WLAN_CHAIN_MASK2> mvTxChainMask;
        //std::vector<WLAN_DataRate> mvDataRate;
        //std::vector<WLAN_RATE> mvWlanRate;
        //std::vector<WLAN_Gen6_ChannelBondingState> mvChannelBondingState;
        float mfEvmLL;
        float mfEvmUL;
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

