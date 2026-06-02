#pragma once
#include <string>
#include <map>
#include <vector>
#include <QMSL_inc/QLib.h>
#include <QMSL_inc/QLib_Defines.h>
#include <JGW_QMSLFuncPlugin/CCJGW_WCNTestDefine.h>

namespace JGW
{
    typedef struct  
    {
        bool mbSemResult;
        WLAN_UniversalDataRate meDataRate;
        WLAN_CHAIN_MASK2 meChainMask;
        WLAN_UniversalRateBW meRateBw;
        WLAN_UniversalPhyID mePhyID;
        WLAN_Gen6_ChannelBondingState meChannelBondingState;
        uint muChannel;

        int mnPERPower;

        float mfRssi;
        float mfPacketErrorRatio;
        double mfEvm;
        double mfAmplErr;
        double mfDataRateMB;
        double mfSymbolClockError;
        double mfFreqErr;
        double mfPhaseErr;
        double mfAvgTxPower;
        double mfLoLeakager;   
    }S_TRX_TEST_DATA,*LPS_TRX_TEST_DATA;
    //! serail number => tx test data
    typedef std::map<std::string,std::vector<S_TRX_TEST_DATA>> _mapTestLabelvTxTestData;

    typedef enum
    {
        E_EXCEL_ROW = 0,
        E_EXCEL_COL = 1
    }E_EXCEL_ROW_COL;

    typedef struct  
    {
        WLAN_UniversalDataRate meDataRate;
        WLAN_CHAIN_MASK2 meChainMask;
        WLAN_UniversalRateBW meRateBw;
        WLAN_UniversalPhyID mePhyID;
        WLAN_Gen6_ChannelBondingState meChannelBondingState;
        uint muChannel;

        //! row col 发射功率
        uint muTxPowerPos[2];
        //! 频谱模板
        uint muTxSemPos[2];
        //! 中心频率误差
        uint muTxFreqErrorPos[2];
        //! EVM矢量误差
        uint muTxEvmPos[2];
        //! 接收灵敏度
        uint muRxPerPos[2];
        //! row col 发射功率 高温
        uint muTxHighTmpPowerPos[2];
        //! 中心频率误差 高温
        uint muTxHighTmpFreqErrorPos[2];
        //! row col 发射功率 低温
        uint muTxLowTmpPowerPos[2];
        //! 中心频率误差 低温
        uint muTxLowTmpFreqErrorPos[2];

        std::string mstrSheetName;
    }S_EXCEL_WIFI_POS;

    //typedef struct  
    //{
    //    WLAN_UniversalDataRate meDataRate;
    //    WLAN_CHAIN_MASK2 meChainMask;
    //    WLAN_UniversalRateBW meRateBw;
    //    PhyID mePhyID;
    //    WLAN_Gen6_ChannelBondingState meChannelBondingState;
    //    uint muChannel;

    //    int mnPower;
    //    float mfRssi;
    //    float mfPacketErrorRatio;
    //}S_RX_TEST_DATA;

    //typedef std::map<std::string,std::vector<S_RX_TEST_DATA>> _mapTestLabelvRxTestData;

}