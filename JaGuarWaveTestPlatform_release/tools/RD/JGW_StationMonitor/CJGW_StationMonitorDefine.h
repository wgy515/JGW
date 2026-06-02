#pragma once
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <string>
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>
//! 设备信息上报
#define REQ_CODE_STATUS_REPORT 1001
//! iperf3 测试上报
#define REQ_CODE_IPERF_REPORT 1004
//! ping测试上报
#define REQ_CODE_PING_REPORT 1005
//! 设备信息上报
#define REQ_CODE_SYSTEM_REPORT 1006
//! 测试完成上报
#define REQ_CODE_RESULT_DONE 1007
//!
#define WEB_MAX_REQUEST_ERROR_COUNT 5
//!
#define STATION_SOCKET_RESPONSE_CODE 1101
namespace JGW
{
    typedef struct  
    {
        bool mbConnectPeraso;
        float mfTxDropRate;
        float mfRxDropRate;

        ULONGLONG mulUploadSpeed;
        ULONGLONG mulDownloadSpeed;

        std::string mstrNetCardConnectName;


        std::string mstrUploadSpeed;
        std::string mstrDownloadSpeed;
        CCJGW_CriticalSectionLock mTrafficSpeedCriticalSectionLock;

        std::string mstrStationStatusJson;
        CCJGW_CriticalSectionLock mStationStatusJsonCriticalSectionLock;

        std::wstring mstrTxSector;
        std::wstring mstrRxSector;

        std::wstring mstrTxMcs;
        std::wstring mstrRxMcs;

        std::wstring mstrIPAddress;
        CCJGW_CriticalSectionLock mIPAddressCriticalSectionLock;
        std::wstring mstrGatewayAddress;

        std::wstring mstrMacAddress;
        std::string mstrAnsiMacAddress;
        CCJGW_CriticalSectionLock mMacAddressCriticalSectionLock;

        std::wstring mstrRssi;
    }S_STATION_MONITOR_UI_INFO,*PS_STATION_MONITOR_UI_INFO;

    typedef struct  
    {
        bool mbThreadIsWorking;
        bool mbStopReqThreadWork;
        LPVOID mpMessageThread;
        std::string mstrCmd;
        std::string mstrSocketAddres;
       // std::string mstrJson;
        boost::shared_ptr<boost::asio::ip::tcp::socket> mClientSocket;
    }S_STATION_HANDLE_REQ_THREAD_PARAM,*PS_STATION_HANDLE_REQ_THREAD_PARAM;

    extern std::string gulp(std::istream &in);
}