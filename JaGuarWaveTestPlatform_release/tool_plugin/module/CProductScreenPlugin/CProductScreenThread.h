#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <UiLib/CAsyncMsgDialog.h>
#include <JGW_WindowsFuncPlugin/CJGW_ProcessPipe.h>
#include "CProductScreen_Define.h"

namespace JGW
{
    class CCProductScreenThread:public CCMessageThread
    {
    public:
        CCProductScreenThread(void);
        ~CCProductScreenThread(void);
    private:
        void OnInitThread(WPARAM wParam,LPARAM lParam);
        void OnRunThread(WPARAM wParam,LPARAM lParam);

        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        void SendTestMsgToWnd(const wchar_t* strMsg);

        bool FindPerasoComPort();

        bool InitProductFolderPath();

        bool ExecuteProductScreenTest();

        bool ReadProductScreenTestResult();

        bool GetSerailNumberToResultText();

        bool CheckTestIsOk();

        bool SaveTestLogFile();
    private: 
        bool                        mbInitLoadPDBImage;
        CCJGW_AsyncWndMessage*      mpAsyncWndMessage;
        LPS_PRODUCT_SCREEN_PARAM    mpsProductScreenParam;
        std::wstring                mstrSerialNumber;
        std::wstring                mstrReadTestResult;
        CCJGW_ProcessPipe           mcProecessPide;
        char*                       mszTestResultBuf;
    };
}
