#pragma once
#include <string>
#include <vector>
#include <JGW_PropertyConfigPlugin/CJGW_PropertyExport.h>
#define PER_ANGLE_TURN 10000
#define TURNTABLE_NODE_ID 32
#define PROPERTY_FILE_NAME L"throughput.test.property.config"
#define GET_PROPERTY GetFilePropertyToFileName(PROPERTY_FILE_NAME)

namespace JGW
{

    enum E_EQUIP_TYPE
    {
        E_QCOMM_CAL = 0, //! 高通校准XML配置
        E_QSEQ, //! 高通QSEQ校准配置方式
        E_EQUIP_CONFIG //! 自定义校准
    };

    typedef struct 
    {
        BYTE mcEquipType;
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
        std::string mstrIPAddress;
    };
    //! MONITOR MODEL PRIMARY_ADDRESS GPIB_INTERFACE
    //! GPIB TCP IP SERIAL
    struct  S_MONITOR_INFO
    {
        BYTE                m_cInterfaceType;
        std::wstring        m_strModelName;
        S_NIGPIB_INSTRUMENT_INFO msNIGPIBInstrumentInfo;
        S_VISA_INSTRUMENT_INFO msVISAInstrumentInfo;
        S_TCPIP_INSTRUMENT_INFO msTCPIPInstrumentInfo;
    };

    enum E_SOCKET_TERIMALTYPE
    {
        E_SERVICE_TYPE = 0,
        E_CLIENT_TYPE
    };

    typedef struct
    {
        double mfVelocity; //! 周转速度
        double mfAcceleration; //! 加速度
        double mfDeceleration; //! 减速度

        int mTurnTableStep; //! 转台步进
        int mTurnTableAngleStart; //! 转台起始角度
        int mTurnTableAngleEnd; //! 转台结束角度

        int mTurnTableAdjTime; //! 转台调整时间
    }S_TRUNTABLE_PARAM; //! 转台测试参数

    typedef struct  
    {
        float mfAttenutorStep;
        float mfAttenutorStart;
        float mfAttenutorEnd;

        int mAttenutorAdjTime; //! 转台调整时间
    }S_ATTENUATOR_PARAM; //! 衰减器测试参数
    //! Client
    //! iperf3.exe -c 192.168.133.32 -t 10
    //! Server
    //! iperf3.exe -s
    typedef struct  
    {
        bool mbEnableTrunTable; //! 是否启用转台
        bool mbEnableAttenutor; //! 是否启用衰减器
        bool mbReadOmniRSSI; //! 是否读取OMNIRSSI
        bool mbStartTestStatus; //! 开始测试状态 true 开始测试 false 停止测试

        BYTE mnTerminalType;//! 终端类型 -> -s（Server）-c (Client)  0 : Server 1 : Client
        BYTE mnProtocolType; //! 协议类型 -u UDP 为空则是TCP 0 : TCP 1 : UDP
        int mnTestTime; //! 每个周期测试时间
        int mnTestTimeIndex; //! 测试时间索引
        size_t mnChannel;//! 1 2 3 4
        bool mbMultThread; //! 是否是多路TCP测试
        std::streamoff mnSeekgLog;
        std::wstring mstrIPAddress;//! 服务端IP地址
        std::wstring mstrTestTime;//! 测试时间 s
        std::wstring mstrBandWidth;//! 带宽  限定速率
        std::wstring mstrExtraParameterEdt; //! 附加参数
        std::wstring mstrThreadCount;
        std::wstring mstrHtmlFile; //! 速率显示HTML文件
        std::wstring mstrIperfCommand; //! IPERF命令
        std::wstring mstrIperfExeFilePath; //! Iperf程序目录
        std::wstring mstrSaveOutPutLog; //! Iperf测试保存log
        std::wstring mstrTurntableAngles; //! 转台角度
        std::wstring mstrAttenuatordbm; //! 衰减器值

        std::wstring mstrCurrentTxSector;
        std::wstring mstrCurrentRxSector;

        std::wstring mstrConfigTxSectors;
        std::wstring mstrConfigRxSectors;

        std::vector<std::wstring> mvTxSector;
        std::vector<std::wstring> mvRxSector;

        S_TRUNTABLE_PARAM msTrunTableParam;
        S_ATTENUATOR_PARAM msAttenutorParam;

        S_MONITOR_INFO msMonitorInfo;
    }S_IperfToolConfig,*PS_IperfToolConfig;

    typedef struct
    {
        size_t nIndex;
        double speed;
        std::wstring angle;
    }S_IperfBPSData,*PS_IperfBPSData;
}