#pragma once
#include <string>

#define IPQ_LOG_FOLDER "/root"
//! 时延
#define WM_TIME_DELAY_MSG WM_HWND_TSE_INDEX_MSG_RUN_COMPLETE
//! GPS SPEED消息
#define WM_GPS_SPEED_MSG  WM_HWND_TSE_INDEX_MSG
//! 手机网络通讯消息
#define WM_MPHONE_SOCKET_REMOTE_MSG WM_HWND_RESET_TSE_COMPLETE
//! 吞吐率
#define WM_THROUGHPUT_RATE_MSG WM_TEST_THREAD_TSE_INDEX_MSG_RUN
//! 检查11AD是否已经连接
#define WM_11AD_CONNECT_STATUS_MSG WM_TEST_THREAD_RESET_TSE
//! 是否开始记录测试信息
#define WM_START_RECORD_TEST_INFO WM_HWND_UPDATE_DONGLES_INFO
//! 是否开始记录测试信息
#define WM_STOP_RECORD_TEST_INFO WM_HWND_INIT_TSE_TEST_FAIL_MSG
//! 
#define  PHONE_SOCKET_PORT "30010"
//!
#define PHONE_SOCKET_RX_MAX_LENGTH 1024

#define IPQ_PROC_KMSG_COMMAND_FORMAT "cat /proc/kmsg > %s &\n"
#define IPQ_LOGREAD_COMMAND_FORMAT "logread -f > %s &\n"
#define IPQ_GET_CAT_PROC_KMSG_PID "ps |grep cat |grep /proc/kmsg\n"
#define IPQ_GET_LOGREAD_PID "ps |grep logread |grep \" logread -f\"\n"

namespace JGW
{
    typedef enum 
    {
        E_SERVICE_TYPE = 0,
        E_CLIENT_TYPE
    }E_SOCKET_TERIMALTYPE;

    typedef enum
    {
        E_TCP_TYPE = 0,
        E_UDP_TYPE,
        E_PING_TYPE
    }E_PROTOCOL_TYPE;

    typedef struct
    {
        //bool mbTestDelayTime;//! 是否测试时延,true : Iperf测试过程中测试时延 ,false : Iperf测试过程中不测试时延
        bool mbIsUseCellphoneIP;
        bool mbIsSaveIPQLog;
        E_SOCKET_TERIMALTYPE meSocketTerimalType;
        E_PROTOCOL_TYPE meProtocolType;
        size_t mnTestTimes;
        size_t mnThreadCount;
        size_t mnTestIndex;
        size_t mnSettingMaxListCount;
		size_t mnCheckSignalTimeIntervalMS;
        size_t mnTimeStamp;
        //! iw wlan0 station dump 检查11AD连接命令
        std::string mstrCheck11ADConnectCommand;
        //! 11AD连接成功的查找字符串
        std::string mstr11ADConnectedFindString;
        std::wstring mstrIperfIPAddr;
		std::string mstrIperfIPAddrA;
        std::wstring mstrSSHIPAddr;
        std::wstring mstrPhoneIPAddr;
        std::wstring mstrExtraParameter;
        std::wstring mstrIperfCommand;
        std::wstring mstrIperfTestLogPath;
        std::string mstrUserNameSSH;
        std::string mstrPasswordSSH;
        std::wstring mstrHtmlFile;
        std::wstring mstrIperfRelativePath;
        std::string mstrSignalConfig;
        std::string mstrMcsConfig;
        //! LOG_PREFIX=20KM_30KM;
        std::wstring mstrLogPrefix;   
        std::wstring mstrUploadPhoneSocketMessage;
        //!
        std::wstring mstrLogFolder;
    }S_TestParams;

    //! {"latitude":22.580678878200533,"longitude":113.94094368440628,"speed":0.0}
    typedef struct  
    {
        //! time,connect,throughput rate(Mbits/sec),time delay(ms),Latitude,Longitude,speed(km/h)
        ////! speed per hour时速
        //float mfSpeedPerHour;
        ////! 纬度
        //double mfLatitude;
        ////! 经度
        //double mfLongitude;
        bool mbIs11adConnected; //! 11AD 是否已经连接
        size_t muUnixTestLogTimeStamp;
        int mnRssi;
        int mnMcs;
        int mnTxSector;
        int mnRxSector;
        std::wstring mstrTime;
        std::wstring mstrSpeedPerHour;
        std::wstring mstrLatitude;
        std::wstring mstrLongitude;
        std::wstring mstrThroughputRate;
        std::wstring mstrPingTimeDelay;
		std::wstring mstrDistance;
		std::wstring mstrRemoteLatitude;
		std::wstring mstrRemoteLongitude;
		std::wstring mstrRemoteSocketMsg;
    } S_LogInfo;
}