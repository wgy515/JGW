#pragma once
typedef unsigned int uint;
#include <string>
#include <vector>
#include <JGW_QMSLFuncPlugin/CCJGW_WCNTestDefine.h>
#include <JGW_Device_WCNTester_CommonSCPIWLAN/WCNTester_CommonSCPIWLAN.h>

namespace JGW
{
    typedef struct  
    {
        WLAN_TpcCalScheme meTpcCalScheme;
        WLAN_INSTANCE meInstance;
        WLAN_UniversalPhyID meUniversalPhyID;
        std::string mstrWlanNoiseFloorCalChannel;
        std::vector<size_t> mvChainToCalibrate;
    }S_WLAN_CAL_PARAM;

    typedef struct
    {
        //! 波特率 默认1500000
        int mnnBaudRate;
        //! 串口线COM口地址
        std::wstring mstrSerailComPort;
        //！用户名 root
        std::string mstrUserName;
        //! 用户密码 20171015
        std::string mstrPassword;
        //! wifi校准串口命令
        std::string mstrWiFiCalSerailCommand;
        //! 当前主机IP地址
        std::string mstrCurrentHostIPAddr;
        //! 
        std::string mstrServiceIPAddr;
        //! bdwlan.b290 路径
        std::string mstreepFName;
        //!
        std::string mstrWlan5GMac;
        //!
        std::string mstrWlan2_4GMac;
        //!
        S_WLAN_CAL_PARAM ms5GCalParam;
        //!
        S_WLAN_CAL_PARAM ms2GCalParam;
        //！
        std::vector<S_STATION_CAL_PATH> mvsStationCalPath;
    }S_WIFI_CAL_CONFIG,*LPS_WIFI_CAL_CONFIG;

    typedef struct
    {
        std::wstring mstrTestProject;
        std::wstring mstrMinValue;
        std::wstring mstrValue;
        std::wstring mstrMaxValue;
        std::wstring mstrTestResult;
    }S_TEST_LIST_MSG;
}