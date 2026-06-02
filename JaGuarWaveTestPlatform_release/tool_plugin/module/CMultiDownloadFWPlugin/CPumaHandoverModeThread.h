#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JGW_WindowsFuncPlugin/CJGW_CSerialComPort.h>

namespace JGW
{
    class CCPumaHandoverModeThread :
        public CCMessageThread
    {
    public:
        CCPumaHandoverModeThread(void);
        ~CCPumaHandoverModeThread(void);

    private:
        void OnHandoverDownloadMode(WPARAM wParam,LPARAM lParam);
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP();
    private:
        CCJGW_CSerialComPort mcscp;
    };
}


