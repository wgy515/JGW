#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <UiLib/CAsyncMsgDialog.h>
#include "CJGW_WlanTestDevToolDefine.h"

namespace JGW
{
    class CCJGW_WlanTestDevThread : public CCMessageThread
    {
    public:
        CCJGW_WlanTestDevThread(void);
        ~CCJGW_WlanTestDevThread(void);
    private:
        void OnStartExecuteQsprCsvThread(WPARAM wParam,LPARAM lParam);
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP();
    private:
        LPS_WIFI_TEST_TEMPLATE_PARAM mpsWiFiTestTemplateParam;
        CCJGW_AsyncWndMessage* mpAsyncWndMessage;
    };

}
