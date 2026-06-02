#pragma once
#include <JGW_QMSLFuncPlugin\JGW_QMSLFuncPlugin_Define.h>
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include "CJGW_UsbModeHandover.h"

namespace JGW
{
    class JGWQMSLFUNCPLUGIN_API CCJGW_UsbModeHandoverThread  : public CCMessageThread
    {
    public:
        CCJGW_UsbModeHandoverThread(void);
        ~CCJGW_UsbModeHandoverThread(void);
    protected:
        void OnQcommUsbHandoverEdl(WPARAM wParam,LPARAM lParam);
        DECLARE_MYTHREAD_MESSAGE_MAP();
    private:
        CCJGW_UsbModeHandover mUsbModeHandover;
    };
}