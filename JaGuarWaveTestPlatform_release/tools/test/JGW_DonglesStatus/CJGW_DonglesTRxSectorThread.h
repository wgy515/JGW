#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JGW_WindowsFuncPlugin/CJGW_DonglesComPort.h>
#include "CJGW_DonglesMonitor_Define.h"
#include <fstream>
#include <queue>

namespace JGW
{
    class CCJGW_DonglesTRxSectorThread:public CCMessageThread
    {
    public:
        CCJGW_DonglesTRxSectorThread(void);
        ~CCJGW_DonglesTRxSectorThread(void);
    public:
        //! 发送命令道Dongles
        void SendCommandToDonglesPort(const char* strCommand);
        //! 关闭dongles端口线程
        void ExitDonglesPortThread();
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        //! 开始响应上报TRX Sector信息
        void OnRunTRxSectorThread(WPARAM wParam,LPARAM lParam);
    private:
        //! 打开Dongles端口
        bool OpenPerasoComPort();
        //! 响应上报TRxSector信息
        void OnReportTRxSectorInfo();
        bool ReadDonglesInfo();
        void SendCommandMsg();
    private:
        bool        mbReRead;
        bool        mbExitThread;
        CCJGW_AsyncWndMessage* mpAsyncWndMessage;
        LPS_DONGLES_MONITOR_PARAM mpsDonglesMonitroParam;
        CCJGW_DonglesComPort mcSerialComPort;
        //!std::wofstream mFileStream;
        std::queue<std::string> mqueue_buffer;
    };
}

