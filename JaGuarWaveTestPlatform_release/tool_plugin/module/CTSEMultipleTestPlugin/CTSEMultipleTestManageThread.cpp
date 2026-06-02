#include "StdAfx.h"
#include "CTSEMultipleTestManageThread.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "CTSEMultipleTestThread.h"

#include "../../../platform_include/JGW_MSG_ID_Define.h"
namespace JGW
{
    CCTSEMultipleTestManageThread::CCTSEMultipleTestManageThread(std::vector<S_TSE_MULTIPLE_TEST_THREAD_PARAM>& vsMultipleTestThreadParam) : mvsMultipleTestThreadParam(vsMultipleTestThreadParam),mbManageThread(false)
    {
    }


    CCTSEMultipleTestManageThread::~CCTSEMultipleTestManageThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCTSEMultipleTestManageThread,CCMessageThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCTSEMultipleTestManageThread::StartTSEMultipleTestManageThread(PS_MULTIPLE_CONFIG psMultipleConfig)
    {
        mbManageThread = true;
        mpsMultipleConfig = psMultipleConfig;
    }

    void CCTSEMultipleTestManageThread::StopTSEMultipleTestManageThread()
    {
        mbManageThread = false;
    }

    void CCTSEMultipleTestManageThread::RunThread()
    {
        if (!mbManageThread || NULL == mpsMultipleConfig) return;
        for (size_t i = 0;mbManageThread && i < mvsMultipleTestThreadParam.size();i ++)
        {
            if (!mpsMultipleConfig->mbIsUsbDevice)
            {
                //! 线程暂时未绑定网卡信息
                if (NULL == mvsMultipleTestThreadParam[i].mpsBindNetCardInfo) continue;
                //! 当前线程处于E_TSEMP_BUSY状态则不进行处理
                //! if (E_TSEMP_BUSY == mvsMultipleTestThreadParam[i].meTSEMPTestStatus) continue;
                //！当前线程处于E_TSEMP_INST状态则检查网络连接等相关状态
                if (E_TSEMP_INST != mvsMultipleTestThreadParam[i].meTSEMPTestStatus) continue;

                /*if (E_TSEMP_INST == mvsMultipleTestThreadParam[i].meTSEMPTestStatus && E_NETCARD_PHYSICAL_DISCONNECT == mvsMultipleTestThreadParam[i].mpsBindNetCardInfo->meNetCardPhysicalState)
                {
                mvsMultipleTestThreadParam[i].meTSEMPTestStatus = E_TSEMP_IDLE;
                }*/
                //! 当前绑定网卡线程处于运行状态时，默认是连接状态
                /*if (NULL != mvsMultipleTestThreadParam[i].mpsBindNetCardInfo && E_TSEMP_BUSY == mvsMultipleTestThreadParam[i].meTSEMPTestStatus)
                {
                mvsMultipleTestThreadParam[i].mpsBindNetCardInfo->meNetCardPhysicalState = E_NETCARD_PHYSICAL_CONNECT;
                }*/
                //if (NULL == mvsMultipleTestThreadParam[i].mpsBindNetCardInfo || E_TSEMP_BUSY == mvsMultipleTestThreadParam[i].meTSEMPTestStatus || !mvsMultipleTestThreadParam[i].mbNetCardStatusUpdateFlag) continue;

                PDLOG4WW_F(L"ManageThread Description: %s,IPAddr:%s",mvsMultipleTestThreadParam[i].mpsBindNetCardInfo->mstrDescription.c_str(),mvsMultipleTestThreadParam[i].mpsBindNetCardInfo->mstrIPAddr.c_str());
                NetCardManageThread(&mvsMultipleTestThreadParam[i]);
            } 
        }
    }

    void CCTSEMultipleTestManageThread::NetCardManageThread(PS_TSE_MULTIPLE_TEST_THREAD_PARAM psMultipleTestThreadParam)
    {
        //! 如果当前下载线程正在运行则直接返回
        //if (E_TSEMP_BUSY == psMultipleTestThreadParam->meTSEMPTestStatus)
        //{
        //    PDLOG4WW(L"OnUpgradeThread Download Thread Is Busy");
        //    //psMultipleTestThreadParam->mbNetCardStatusUpdateFlag = false;
        //    return ;
        //}
   
        //! 检测IP,如果没有PING上则表示当前网络连接状态处于未连接
        if (!mICMPSocket.Ping(mpsMultipleConfig->msNetCardConfig.mstrHostIPAddrAnsi,JGW_W2A(psMultipleTestThreadParam->mpsBindNetCardInfo->mstrIPAddr).c_str()))
        {
            PELOG4WW_F(L"OnUpgradeThread Ping %s Source %s Fail",mpsMultipleConfig->msNetCardConfig.mstrHostIPAddr.c_str(),psMultipleTestThreadParam->mpsBindNetCardInfo->mstrIPAddr.c_str());
            /*if (psMultipleTestThreadParam->mpsBindNetCardInfo->meNetCardPhysicalState == E_NETCARD_PHYSICAL_CONNECT)
            {
                psMultipleTestThreadParam->meTSEMPTestStatus = E_TSEMP_INST;
            }*/
            //psMultipleTestThreadParam->mpsBindNetCardInfo->meNetCardPhysicalState = E_NETCARD_PHYSICAL_DISCONNECT;
            return ;
        }
        //! 
        CCTSEMultipleTestThread* pTSEMultipleTestThread = reinterpret_cast<CCTSEMultipleTestThread*>(psMultipleTestThreadParam->mpMultipleTestThread);
        if (NULL == pTSEMultipleTestThread)
        {
            pTSEMultipleTestThread = new CCTSEMultipleTestThread;
            psMultipleTestThreadParam->mpMultipleTestThread = pTSEMultipleTestThread;
        }
        pTSEMultipleTestThread->InitDownloadParam(psMultipleTestThreadParam);
        if (!pTSEMultipleTestThread->IsThreadRun())
        {
            pTSEMultipleTestThread->CreateMessageThread(0,0,true);
        }
        //psMultipleTestThreadParam->mstrTSEMultipleTestLog.ClearText();
        psMultipleTestThreadParam->mstrTSESuiteConfigFilePath = mpsMultipleConfig->mstrTSESuiteConfigFilePath;
        psMultipleTestThreadParam->mpsBindNetCardInfo->meNetCardPhysicalState = E_NETCARD_PHYSICAL_CONNECT;
        psMultipleTestThreadParam->meTSEMPTestStatus = E_TSEMP_BUSY;
        //psMultipleTestThreadParam->mbNetCardStatusUpdateFlag = false;
        pTSEMultipleTestThread->PostThreadMessage(DOWN_LOAD_THREAD_RUN_MSG);
    }
}
