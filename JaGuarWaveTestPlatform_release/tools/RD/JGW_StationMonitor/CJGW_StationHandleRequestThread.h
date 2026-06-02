#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include "CJGW_StationMonitorDefine.h"
#include "json/CJGW_StationSocketRequestJson.h"
#include "json\CJGW_ReqSystemInfoJson.h"
#include <JGW_WindowsFuncPlugin/CCJGW_CPUMemDiskStatusImp.h>
#include <JGW_WindowsFuncPlugin/CJGW_ProcessPipe.h>
#include <JGW_WindowsFuncPlugin/CJGW_ICMPSocket.h>
#include <fstream>
namespace JGW
{
    typedef struct  
    {
        std::string mstrIperfLogFilePath;
    }S_IPERF_LOG_INFO;

    class CCJGW_StationHandleRequestThread : public CCMessageThread
    {
    public:
        CCJGW_StationHandleRequestThread(void);
        ~CCJGW_StationHandleRequestThread(void);
    private:
        void OnStartHandleReq(WPARAM wParam,LPARAM lParam);
    private:
        void OnHandleStationSocketCmd(const std::string& strCmd);
        void OnHandlePingRequest(const std::string& strCmd);
        void OnHandleIperfServiceRequest(const std::string& strCmd);
        void OnHandleIperfClientRequest(const std::string& strCmd);
    private:
        void OnStartIperfPrcessToCmd(const std::string& strCmd,bool bIperfServerCmd = false);
        void OnReportPingJsonInfo(const std::string& strIPAddress);
        void OnReportStatusJsonInfo();
        void OnReportSystemJsonInfo();
        void OnReportLogFileJsonInfo();
    private:
        bool CheckIperfProcesssIsRun();
    private:
        void BuildStationLogFile();
        bool UploadLogFileToSocket(const std::string& strSocketAddress,const std::string& strLogFilePath);
        void WritePingLogFileToPingLog(const std::string& strPingLog);
        void WriteStatusLogFileToStatusLog(const std::string& strStatusLog);
        void WriteStationStreamLogToFile(std::ofstream& stationStream,const std::string& strFilePath);
    private:
        std::string BuildPingLogFilePath();
        std::string BuildStationLogFilePath();
        std::string BuildStatusLogFilePath();


        std::string BuildRspPingJson(float fLatency);
        bool ReadCMDProcessPipeEOF(CCJGW_ProcessPipe* pProcessPipe,std::wstring& strRead,size_t nTimeOutSec = 10);
        CCJGW_ProcessPipe* GetProcessCMDPipe();
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        CCJGW_ProcessPipe* m_pCMDPipe;
        PS_STATION_HANDLE_REQ_THREAD_PARAM mpsHandleReqThreadParam;
        PS_STATION_MONITOR_UI_INFO mpsStationMonitorUiInfo;
        CCJGW_CPUMemDiskStatusImp* mpCPUMemDiskStatusImp;

        size_t mnStatusTime;
        std::string mstrPingLogFilePath;
        std::string mstrTestLogFilePath;
        std::string mstrStatusLogFilePath;
        CCJGW_ReqSystemInfoJson mReqSystemInfoJson;
        CCJGW_ICMPSocket mICMPSocket;
        std::vector<std::wstring> mvFullCounterPath;
        std::vector<S_IPERF_LOG_INFO> mvIperfLogInfo;
        std::vector<double> mvDoubleCPUUsed;
    };
}