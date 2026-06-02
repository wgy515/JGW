#pragma once
#include "CTSEAPS_UpgradeDefine.h"
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JGW_WindowsFuncPlugin/CJGW_ProcessPipe.h>
#include <JGW_SSHPlugin/CJGW_SSHShell.h>
namespace JGW
{
    class CCTSE_UpgradeThread : public CCMessageThread
    {
    public:
        CCTSE_UpgradeThread(void);
        ~CCTSE_UpgradeThread(void);
    public:
        void InitDownloadParam(LPS_APS_MULTI_UPGRADE_CONFIG psMultiUpgradeConfig,LPS_NET_CARD_PARAM psNetCardParam);
    private:
        //! 运行下载线程
        void OnRunDownloadThread(WPARAM wParam,LPARAM lParam);
        //! 开启下载进程
        bool StartDownloadProcess();
        //!
        bool RecvTestInfo();
        //! 
        bool CheckUpgradeVersion();
        //!
        void SaveDownloadLog(bool bTestResult);
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        //! 
        LPS_APS_MULTI_UPGRADE_CONFIG mpsMultiUpgradeConfig;
        //! 
        LPS_NET_CARD_PARAM mpsNetCardParam;
        //!
        std::wstring mstrSourceIP;
        //!
        //CCJGW_SSHShell mSSHShell;
        //! CMD管道
        CCJGW_ProcessPipe m_pDownloadPipe;
    };
}