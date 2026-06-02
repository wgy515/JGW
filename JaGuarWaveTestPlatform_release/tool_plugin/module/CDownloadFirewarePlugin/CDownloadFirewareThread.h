#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <UiLib/CAsyncMsgDialog.h>
#include <JGW_WindowsFuncPlugin/CJGW_ProcessPipe.h>
#include "CDownloadFireware_Define.h"

namespace JGW
{
    class CCDownloadFirewareThread:public CCMessageThread
    {
    public:
        CCDownloadFirewareThread(void);
        ~CCDownloadFirewareThread(void);
    private:
        void OnInitThread( WPARAM wParam,LPARAM lParam );
        void OnRunThread( WPARAM wParam,LPARAM lParam );
    private:
        void SendTestMsgToWnd(const wchar_t* strMsg);

        bool FindPerasoComPort();

        bool ExecuteCmdApplication();

        bool ReadDownloadFirewareResult();

        bool CheckDownloadFirewareIsOk();


        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        CCJGW_AsyncWndMessage* mpAsyncWndMessage;
        LPS_DOWNLOAD_FIREWARE_PARAM mpsDownloadFirewareParam;
        std::wstring mstrReadTestResult;
        CCJGW_ProcessPipe            mcProecessPide;
    };
}


