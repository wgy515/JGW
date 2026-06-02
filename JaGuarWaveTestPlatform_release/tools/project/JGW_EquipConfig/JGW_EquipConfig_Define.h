#pragma once
#include <string>

namespace JGW
{
    enum E_EQUIP_TYPE
    {
        E_QCOMM_CAL = 0, //! 高通校准XML配置
        E_QSEQ, //! 高通QSEQ校准配置方式
        E_EQUIP_CONFIG, //! 自定义校准
        E_QCOMM_WLAN, //! QCOMM WLAN
    };

    typedef struct 
    {
        BYTE mcEquipType;
        int mnDeviceID;
        std::wstring mstrGroupName;
        std::wstring mstrTestName;
        std::wstring mstrModuleFolder;
        std::wstring mstrConfigFileName;
    }S_EQUIP_PROJECT_CONFIG,*LPS_EQUIP_PROJECT_CONFIG;

    enum
    {
        NI_GPIB_TYPE = 0,
        AGILENT_GPIB_TYPE
    };

    enum
    {
        E_GPIB_HIVE,//! GPIB连接 又分为NI和Agen
        E_TCP_IP_HIVE, //! TCP IP连接
        E_VISA_HIVE, //! VISA
        E_SERIAL_HIVE //! Serial连接
    };

    struct S_NIGPIB_INSTRUMENT_INFO
    {
        BYTE                m_cNIGPIBType;
        DWORD				m_nGpibInterface;
        DWORD				m_nPrimaryAddress;
        DWORD               m_nSecAddress;
    };

    struct S_VISA_INSTRUMENT_INFO
    {
        std::string mstrVisaAddress;
    };

    struct S_TCPIP_INSTRUMENT_INFO
    {
        size_t mnSubInstrument;
        size_t mnSocketPort;
        std::string mstrSourceAddress;
        std::string mstrIPAddress;
    };
    //! MONITOR MODEL PRIMARY_ADDRESS GPIB_INTERFACE
    //! GPIB TCP IP SERIAL
    struct  S_MONITOR_INFO
    {
        BYTE                m_cInterfaceType;
        int mnDeviceID;
        std::wstring        m_strModelName;
        S_NIGPIB_INSTRUMENT_INFO msNIGPIBInstrumentInfo;
        S_VISA_INSTRUMENT_INFO msVISAInstrumentInfo;
        S_TCPIP_INSTRUMENT_INFO msTCPIPInstrumentInfo;
    };
}