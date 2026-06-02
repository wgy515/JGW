#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include "CTSEMultipleTestDefine.h"
#include <JGW_WindowsFuncPlugin/CJGW_ProcessPipe.h>

namespace JGW
{
    class CCTSEMultipleTestThread : public CCMessageThread
    {
    public:
        CCTSEMultipleTestThread(void);
        ~CCTSEMultipleTestThread(void);
    public:
        void InitDownloadParam(PS_TSE_MULTIPLE_TEST_THREAD_PARAM psTSEMultipleTestParam);
    private:
        //! 运行下载线程
        void OnRunDownloadThread(WPARAM wParam,LPARAM lParam);
        //! 开启下载进程
        bool StartDownloadProcess();
        //!
        bool StartNetCardTSEProcess();
        //! 
        bool StartUSBDeviceTSEProcess();
        //!
        bool RecvTestInfo();
        //!
        void SaveDownloadLog(bool bTestResult);
        //!
        std::wstring GetTSETestParam();
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        std::wstring mstrSourceIP;
        PS_TSE_MULTIPLE_TEST_THREAD_PARAM mpsTSEMultipleTestParam;
        CCJGW_ProcessPipe m_pDownloadPipe;
    };

}

