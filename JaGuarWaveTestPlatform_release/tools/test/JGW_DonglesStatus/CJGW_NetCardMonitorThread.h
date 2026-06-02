#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <pcap.h>

namespace JGW
{
    class CCJGW_NetCardMonitorThread:public CCMessageThread
    {
    public:
        CCJGW_NetCardMonitorThread(std::wstring& strNetCardInfo);
        ~CCJGW_NetCardMonitorThread(void);
    public:
        void OnBreakLoop();
    private:
        void GetLocalIPAddress();
        void OnMonitorThread(WPARAM wParam,LPARAM lParam);
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        pcap_t *mpsPcap_t;
        CCJGW_AsyncWndMessage* mpAsyncWndMessage;
        std::wstring& mstrNetCardInfo;
    };
}


