#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include "CMultipleTestPluginDefine.h"
#include <JGW_WindowsFuncPlugin/CJGW_ProcessPipe.h>

namespace JGW
{
    class CCMultipleTestThread : public CCMessageThread
    {
    public:
        CCMultipleTestThread(void);
        ~CCMultipleTestThread(void);
    public:
        void InitTestParam(PS_TSE_MULTIPLE_TEST_THREAD_PARAM psTSEMultipleTestParam);
    private:
        //! 运行下载线程
        void OnRunTestThread(WPARAM wParam,LPARAM lParam);
    private:
        bool StartTSEProcess();
        //! general
        std::wstring GetTSEGeneralTestParam();
        //!
        void SaveDownloadLog(bool bTestResult);
        //!
        bool RecvTestInfo();
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        PS_TSE_MULTIPLE_TEST_THREAD_PARAM mpsTSEMultipleTestParam;
        CCJGW_ProcessPipe mcDownloadPipe;
    };

}

