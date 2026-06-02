#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include "CMultipleTestPluginDefine.h"

namespace JGW
{
    class CCMultipleTestManageThread : public CCMessageThread
    {
    public:
        CCMultipleTestManageThread(std::vector<S_TSE_MULTIPLE_TEST_THREAD_PARAM>& vsMultipleTestThreadParam);;
        ~CCMultipleTestManageThread(void);
    public:
        void StartTSEMultipleTestManageThread(PS_MULTIPLE_CONFIG psMultipleConfig);
        void StopTSEMultipleTestManageThread();
    private:
        void RunThread();
        //! Æô¶¯tftpd32.exe
        void StartTFTP32Process();
    private:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        bool mbManageThread;
        bool mbCheckTFPT32ProcessRuning;
        PS_MULTIPLE_CONFIG mpsMultipleConfig;
        std::vector<S_TSE_MULTIPLE_TEST_THREAD_PARAM>& mvsMultipleTestThreadParam;
    };

}

