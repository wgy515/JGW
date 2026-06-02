//
// xxx.h
//
// $Id: xxx.h#3 $
//
// Library: 
// Package: 
// Module:  
//
// TDO:
//
// Copyright (c) 2021-2021
//
// SPDX-License-Identifier:	
//
#pragma once
#define MAX_DMG_CHANNEL 6
#include <vector>
namespace JGW
{
    // S1234567891234,1-2010-15.3,1-1643.5-22.65
    // S1234567891234,1,2412,22.3
    // S1234567891234,1,2442,22.3
    // S1234567891234,1,2472,22.3
    // S1234567891234,1,5180,22.3
    // S1234567891234,1,5520,22.3
    // S1234567891234,2,2412,22.3
    // S1234567891234,2,2442,22.3
    // S1234567891234,2,2472,22.3
    // S1234567891234,2,5180,22.3
    // S1234567891234,2,5520,22.3
    // S1234567891234,4,2412,22.3
    // S1234567891234,4,2442,22.3
    // S1234567891234,4,2472,22.3
    // S1234567891234,4,5180,22.3
    // S1234567891234,4,5520,22.3
    // S1234567891234,8,2412,22.3
    // S1234567891234,8,2442,22.3
    // S1234567891234,8,2472,22.3
    // S1234567891234,8,5180,22.3
    // S1234567891234,8,5520,22.3
    typedef struct  
    {
        // 
        size_t mnBand;
        // 功率
        float mstrPower;
        // SN
        std::wstring mstrBarcode;
        // 频率
        std::wstring mstrFreq;
    }S_GU_DATA,*LPS_GU_DATA;

    enum E_TSE_CAL_TYPE
    {
        E_TSE_CAL_DMG = 0,
        E_TSE_CAL_IPQ40XXWIFI = 1,
        E_TSE_CAL_RF = 2,
        E_TSE_QIA_WLAN = 3
    };

    typedef struct
    {
        //! TSE校准线损类型
        E_TSE_CAL_TYPE meTSECalType;
        //! 是否初始化TSE测试框架成功
        bool mbInitSequenceTestOk;
        //！窗口句柄(用于接收或者处理测试消息)
        HWND mhMainWnd; //! 12345
        //! 金机在工厂环境下使用的线损，一般在XML配置中可以定义<TSE_GU_LOSS>xx.xx</TSE_GU_LOSS>
        float mfGuStartLoss;
        //！TSE测试框架的测试文件配置 
        std::wstring mstrSuiteXmlConfigPath;
        // 金机数据集合
        std::vector<S_GU_DATA> mvsGuData;
    }S_TSE_CALCONFIG_TEST_CONFIG_PARAM,*PS_TSE_CALCONFIG_TEST_CONFIG_PARAM;

    

    enum E_TSE_GuCal_Algorithm
    {
        E_TSE_GuCal_AVG = 0, //! 平均算法
        E_TSE_GuCal_MIN_LIMIT = 1,//！最小值下限算法
        E_TSE_GuCal_RMINMAX_AVG = 2, //！去掉最大最小值算法
        E_TSE_GuCal_RMINMAX_MIN_LIMIT //！第二小值下限算法
    };

    typedef struct
    {
        E_TSE_GuCal_Algorithm meAlgorithm;
        int mnLimit;//! 范围
        std::wstring mstrGuTestDataFile;
        std::wstring mstrDutTxGuRxRssiEnvironment; //! GU RX DUT TX RSSI配置信息
        std::wstring mstrDutRxGuTxRssiEnvironment; //! GU TX DUT RX RSSI配置信息
        std::wstring mstrCalSuiteXmlFilePath; //! TSE需要保存配置文件路径，加载TSE配置文件，然后查找是否有全局配置文件，在加载全局配置文件，然后全局配置文件为空的情况的下，更新全局配置文件相关配置
    }S_DMG_CAL_CONFIG,*PS_DMG_CAL_CONFIG; 
}
