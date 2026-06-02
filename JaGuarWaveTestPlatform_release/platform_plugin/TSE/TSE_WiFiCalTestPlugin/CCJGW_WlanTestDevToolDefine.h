#pragma once
#include <vector>
#include <string>
#include <map>
#include <JGW_QMSLFuncPlugin/CCJGW_WCNTestDefine.h>
#include <QMSL_inc/QLib.h>
#include <QMSL_inc/QLib_Defines.h>

typedef unsigned int uint;
namespace JGW
{
    typedef enum
    {
        E_EXCEL_ROW = 0,
        E_EXCEL_COL = 1
    }E_EXCEL_ROW_COL;
    //! 信道，链路以及MCS
    typedef struct  
    {
        //! 
        uint muChannel;
        //! 
        WLAN_CHAIN_MASK2 meChainMask;
        //!
        WLAN_UniversalDataRate meDataRate;
        //! RX测试小区功率最小值
        int muRxStartPower;
        //! 灵敏度范围
        uint mnRxPerRange;
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
    }S_CHANNEL_EXCEL_INFO;

    typedef struct  
    { 
        //!
        WLAN_UniversalRateBW meRateBw;
        //!
        uint muPowerLevel;
        //!
        uint muChainIntervalPos;
        //! 
        std::string mstrWlanConfigTxParamName;
        //!
        std::string mstrWlanConfigRxParamName;
        //! channel vector
        std::vector<uint> mvChannel;
        //! rate vector
        std::vector<WLAN_UniversalDataRate> mveRate;
        //! chain mask vector
        std::vector<WLAN_CHAIN_MASK2> mvChainMask;
        //! 链路对应的EXCEL相关信息
        std::vector<S_CHANNEL_EXCEL_INFO> mvChainExcelInfo;
        //std::map<WLAN_CHAIN_MASK2,std::vector<S_CHANNEL_EXCEL_INFO>> mmapChainInfo;
    }S_WLAN_PROTOCOL_INFO;

    typedef struct  
    {
        //! 
        std::vector<std::wstring> mvTestLabelList;
        //! 
        std::vector<std::wstring> mvSheetNameList;
        //! SheetNameList name,
        std::map<std::wstring,S_WLAN_PROTOCOL_INFO> mmapWlanProtocolInfo;
    }S_WlanTestConfigInfo;



}