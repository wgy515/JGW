#include "StdAfx.h"
#include "CTSEAPSMultiUpgreadManageThread.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#include "../../../../platform_include/JGW_MSG_ID_Define.h"
namespace JGW
{
    CCTSEAPSMultiUpgreadManageThread::CCTSEAPSMultiUpgreadManageThread(void) : mpsNetCardParam(NULL)
    {
        SetThreadSleepTimeInterval(500);
    }


    CCTSEAPSMultiUpgreadManageThread::~CCTSEAPSMultiUpgreadManageThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCTSEAPSMultiUpgreadManageThread,CCMessageThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCTSEAPSMultiUpgreadManageThread::OnInitManageThread(LPS_NET_CARD_PARAM psNetCardParam,LPS_APS_MULTI_UPGRADE_CONFIG psMultiUpgradeConfig)
    {
        mpsNetCardParam = psNetCardParam;
        mpsMultiUpgradeConfig = psMultiUpgradeConfig;
    }

    void CCTSEAPSMultiUpgreadManageThread::RunThread()
    {
        if (!mpsNetCardParam) return;
        for (int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
        {
            if (/*ENETCARD_CONNECT_STATUS != mpsNetCardParam[i].meNetCardStatus || */E_BUSY_DOWNLOAD == mpsNetCardParam[i].meDownloadStatus || !mpsNetCardParam[i].mbNetCardStatusUpdateFlag) continue;
            PDLOG4WW_F(L"ManageThread Description: %s,IPAddr:%s",mpsNetCardParam[i].mstrDescription.c_str(),mpsNetCardParam[i].mstrIPAddr.c_str());
            OnUpgradeThread(i);
        }
    }

    void CCTSEAPSMultiUpgreadManageThread::OnUpgradeThread(int nIndex)
    {
        //! 如果当前下载线程正在运行则直接返回
        if (E_BUSY_DOWNLOAD == mpsNetCardParam[nIndex].meDownloadStatus)
        {
            PDLOG4WW(L"OnUpgradeThread Download Thread Is Busy");
            mpsNetCardParam[nIndex].mbNetCardStatusUpdateFlag = false;
            return ;
        }
        //! 检测IP,如果没有PING上则表示当前网络连接状态处于未连接
        if (!mICMPSocket.Ping(mpsMultiUpgradeConfig->mstrHostIPAddrAnsi,JGW_W2A(mpsNetCardParam[nIndex].mstrIPAddr).c_str()))
        {
            PELOG4WW_F(L"OnUpgradeThread Ping %s Source %s Fail",mpsMultiUpgradeConfig->mstrHostIPAddr.c_str(),mpsNetCardParam[nIndex].mstrIPAddr.c_str());
            mpsNetCardParam[nIndex].meNetCardStatus = ENETCARD_DISCONNCT_STATUS;
            return ;
        }
        
        if (!mUpgradeThreads[nIndex].IsThreadRun())
        {
            mUpgradeThreads[nIndex].CreateMessageThread(0,0,true);
            mUpgradeThreads[nIndex].InitDownloadParam(mpsMultiUpgradeConfig,&mpsNetCardParam[nIndex]);
        }
        mpsNetCardParam[nIndex].meNetCardStatus = ENETCARD_CONNECT_STATUS;
        mpsNetCardParam[nIndex].meDownloadStatus = E_BUSY_DOWNLOAD;
        mpsNetCardParam[nIndex].mbNetCardStatusUpdateFlag = false;
        mUpgradeThreads[nIndex].PostThreadMessage(DOWN_LOAD_THREAD_RUN_MSG);
    }
}