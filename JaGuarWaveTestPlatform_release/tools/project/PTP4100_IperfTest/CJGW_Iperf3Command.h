#pragma once
#include "RedirectConsole.h"
#include <vector>
#include <string>
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
namespace JGW
{
    // unit : Mbits/sec
    typedef void (*Iperf3TestInfoFunc)(float fSpeedMbitssec);
    class CCJGW_Iperf3Command : public CCMessageThread
    {
    public:
        CCJGW_Iperf3Command(void);
        ~CCJGW_Iperf3Command(void);
    public: 
        // iperf3 server default iperf3.exe -s --logfile xx.log -i 1 -f m
        // -f : m : Mbits,k : Kbits,K : KBytes,M : MBytes
        bool StartIper3Server(const std::wstring& strIperf3Path = L"tool\\iperf3.exe");
    public:
        // iperf3 client default iperf3.exe -c 127.0.0.1 -t 10 -P 4 --logfile xx.log -i 1 -f m -w 8M -l 65000
        bool StartIperf3Client(const std::wstring& strIPAddress,size_t nTestTimeSec,size_t nThreadCount,const std::wstring& strExtParam = L"-w 8M -l 65000",const std::wstring& strIperf3Path = L"tool\\iperf3.exe");
    public:
        void RegisterCallbackIperf3TestInfo(Iperf3TestInfoFunc func);
        bool CheckIper3IsRunning();
        void StopIperf3();
    public:
        bool GetIperf3Speed(float& fSpeed);
    private:
        void RunThread();
        void BuildIperfTestLogPath();
        DECLARE_MYTHREAD_MESSAGE_MAP();
    private:
        bool mbMultiThread;// multi thread
        bool mbIperfRunning;
        int mnConnectedCount;
        std::streamoff mnSeekgLog;
        Iperf3TestInfoFunc mpIperf3TestInfoFunc;
        std::wstring mstrIperfTestLogPath;
        std::vector<char> mvEofChars;
        std::vector<std::string> mArgs;
        RedirectConsole mRedirectConsole;
    };
}
