#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include "CTSEMultipleTestDefine.h"
#include <JGW_WindowsFuncPlugin/CJGW_ICMPSocket.h>
namespace JGW
{
    class CCTSEMultipleTestManageThread : public CCMessageThread
    {
    public:
        CCTSEMultipleTestManageThread(std::vector<S_TSE_MULTIPLE_TEST_THREAD_PARAM>& vsMultipleTestThreadParam);
        ~CCTSEMultipleTestManageThread(void);
    public:
        void StartTSEMultipleTestManageThread(PS_MULTIPLE_CONFIG psMultipleConfig);
        void StopTSEMultipleTestManageThread();
    private:
        virtual void RunThread();
    private:
        void NetCardManageThread(PS_TSE_MULTIPLE_TEST_THREAD_PARAM psMultipleTestThreadParam);
    private:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        bool mbManageThread;
        PS_MULTIPLE_CONFIG mpsMultipleConfig;
        CCJGW_ICMPSocket mICMPSocket;
        std::vector<S_TSE_MULTIPLE_TEST_THREAD_PARAM>& mvsMultipleTestThreadParam;
    };

}

