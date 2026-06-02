#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JGW_WindowsFuncPlugin/CJGW_CSerialComPort.h>
#include <JGW_WindowsFuncPlugin/CCJGW_PerasoSocImp.h>
#include <JGW_SSHPlugin/CJGW_SSHShell.h>
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

        int GetRssi() { return mnPrevRssi;}

        int GetMcs() { return mnPrevMcs;}
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
        void UpdateRssiInfo();
        void UpdateMcsInfo();
        void SendCommandMsg();
        void CreateLogFile();
    private:
        int         mnPrevMcs;
        int         mnPrevRssi;
        int         mnPrevLocalRssi;
        bool        mbReRead;
        bool        mbExitThread;
        CCJGW_AsyncWndMessage* mpAsyncWndMessage;
        LPS_DONGLES_MONITOR_PARAM mpsDonglesMonitroParam;
        CCJGW_CSerialComPort mcSerialComPort;
        CCJGW_SSHShell mcSSHShell;
        CCJGW_PerasoSocImp* mpPerasoSocImp; 
        //! CCJGW_DonglesComPort mcDonglesComPort;
        std::wofstream mFileStream;
        std::queue<std::string> mqueue_buffer;
    };
}

