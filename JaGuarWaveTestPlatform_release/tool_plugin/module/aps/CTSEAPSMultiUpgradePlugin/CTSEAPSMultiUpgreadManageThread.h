#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include "CTSEAPS_UpgradeDefine.h"
#include "CTSE_UpgradeThread.h"
#include <JGW_WindowsFuncPlugin/CJGW_ICMPSocket.h>
namespace JGW
{
    class CCTSEAPSMultiUpgreadManageThread : public CCMessageThread
    {
    public:
        CCTSEAPSMultiUpgreadManageThread(void);
        ~CCTSEAPSMultiUpgreadManageThread(void);
    public:
        void OnInitManageThread(LPS_NET_CARD_PARAM psNetCardParam,LPS_APS_MULTI_UPGRADE_CONFIG psMultiUpgradeConfig);
    private:
        //! 运行线程
        void RunThread();
        //!
        void OnUpgradeThread(int nIndex);
        //! net icmp检测  检测是否已经建立连接
    private:
        LPS_APS_MULTI_UPGRADE_CONFIG mpsMultiUpgradeConfig;
        LPS_NET_CARD_PARAM mpsNetCardParam;
        CCTSE_UpgradeThread mUpgradeThreads[DOWNLOAD_THREAD_MAX];
        CCJGW_ICMPSocket mICMPSocket;
    private:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    };
}