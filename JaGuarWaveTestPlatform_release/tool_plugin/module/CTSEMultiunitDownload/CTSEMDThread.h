#pragma once
#include "CTSEMultiunitDonwloadDefine.h"
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JGW_WindowsFuncPlugin/CJGW_ProcessPipe.h>

namespace JGW
{
    class CCTSEMDThread : public CCMessageThread
    {
    public:
        CCTSEMDThread(void);
        ~CCTSEMDThread(void);
    public:
         S_DOWNLOAD_PROGRESS_INFO& GetDownladProgressInfo() { return msDownloadProgressInfo;}
    private:
        //! 运行下载线程
        void OnRunDownloadThread(WPARAM wParam,LPARAM lParam);
        //! 开启下载进程
        bool StartDownloadProcess();
        //!
        bool RecvTestInfo();
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        int mnSrcComPort;
        //! 下载进度信息
        S_DOWNLOAD_PROGRESS_INFO msDownloadProgressInfo;
        //! CMD管道
        CCJGW_ProcessPipe m_pDownloadPipe;
    };
}