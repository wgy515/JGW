#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include <JGW_NIGPIBControllImpl/CMW500_Montior.h>

#include "../../../platform_include/JGW_WIFI_Define.h"

namespace JGW
{
    //! MONITOR MODEL PRIMARY_ADDRESS GPIB_INTERFACE
    //! GPIB TCP IP SERIAL
    struct  S_MONITOR_INFO
    {
        BYTE                m_cInterfaceType;
        BYTE                m_cNIGPIBType;
        DWORD				m_nGpibInterface;
        DWORD				m_nPrimaryAddress;
        DWORD               m_nSecAddress;
        std::wstring        m_strModelName;
        std::wstring        m_strIPAddress;
        std::wstring        m_strSerial;
    };

    const wchar_t gszWlanModeName[][20] = {L"B",L"G",L"N",L"AC"};
#if 0
    /*
    [b]
    TxChain=
    RfPort=
    BandWidth=
    Channel=
    RxLoss=
    */
    typedef struct
    {
        E_WLAN_MODE meWlanMode;   

        std::vector<int> mvChannel;//! mvChannel与mvfRxLoss 数组大小必须一致
        std::vector<float> mvfRxLoss;

        std::vector<float> mvfPower;
        std::vector<int> mvPowerRange;

        std::vector<int> mvBandWidth;

        std::vector<int> mvTxChain;//! ()Tx链路  mvTxChain与mvRfPort 数组大小必须一致
        std::vector<int> mvRfPort;

        //! std::vector<float> mvfMaxPower;
        //! std::vector<float> mvfMinPower;


    }S_WLAN_TX_MODE_CONFIG,*PS_WLAN_TX_MODE_CONFIG;

    typedef  struct
    {
        S_MONITOR_INFO msMonitorInfo;
        std::string mstrBoardDataFilePath;
        std::wstring mstrWlanBatFilePath;
        std::wstring mstrWlanBatDir;
        std::vector<S_WLAN_TX_MODE_CONFIG> mvWlanModeConfig;
    }S_WiFiTest_Config,*PS_WiFiTest_Config;
#endif

    struct S_WLAN_TX_TEST_ITEM_KPI 
    {
        float mfPower;
        float mfExpectPower;
        float mfPowerRange;
    };

    struct S_WLAN_RX_TEST_ITEM_KPI 
    {
        float mfRssi;
        float mfRssiRange;
        float mfPER;
    };

    struct S_WLAN_TEST_ITEM_KPI
    {
        bool mbTxTest;
        bool mbRxTest;
        float mfRxLoss;
        float mfTxLoss;

        S_WLAN_TX_TEST_ITEM_KPI msWlanTxTestItemKpi;
        S_WLAN_RX_TEST_ITEM_KPI msWlanRxTestItemKpi;
    };
    /*
    tx chain 
    Channel
    TX PARAM
    */
    typedef struct  
    {
        //! E_WLAN_MODE meWlanMode;
        //!std::vector<int> mvTxChain;//! mvTxChain大小 == mvRfPort大小
        //!std::vector<int> mvRfPort;
        int mnRfPort;
        int mnTxChain;
        std::vector<E_WLAN_MODE>         mveWlanMode;
        std::vector<E_WLAN_TX_DATA_RATE> mvWlanTxDataRate;
        std::vector<E_WLAN_RX_DATA_RATE> mvWlanRxDataRate;
        std::vector<float> mvChannel; 
        std::vector<S_WLAN_TEST_ITEM_KPI> mvWlanTestItemKPI; //! size = mvTxChain.size * mvBandWidth.size * mvChannel.size;
    }S_WLAN_MODE_CONFIG,*PS_WLAN_MODE_CONFIG;

    typedef struct
    {
        int mnErrorReTest;
        std::string mstrBoardDataFilePath;
        std::wstring mstrWlanBatFilePath;
        std::wstring mstrWlanBatDir;
        S_MONITOR_INFO msMonitorInfo;
        std::vector<S_WLAN_MODE_CONFIG> mvsWlanModeConfig;
    }S_WLAN_CONFIG,*PS_WLAN_CONFIG;
}
