#include "StdAfx.h"
#include "CPEMultiDownloadManageThread.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "../../../../platform_include/JGW_MSG_ID_Define.h"
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
namespace JGW
{
    CCPEMultiDownloadManageThread::CCPEMultiDownloadManageThread(CCPEMultiDownloadSortConfig& multiDownloadSortConfig) : mMultiDownloadSortConfig(multiDownloadSortConfig),m_bManageClose(false),mpQueue_buffer(NULL),mbCheckTFPT32ProcessRuning(false)
    {
        //! SetThreadSleepTimeInterval(500);
    }


    CCPEMultiDownloadManageThread::~CCPEMultiDownloadManageThread(void)
    {
        ExitManageThread();
    }

    void CCPEMultiDownloadManageThread::ExitManageThread()
    {
        m_bManageClose = true;
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCPEMultiDownloadManageThread,CCMessageThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCPEMultiDownloadManageThread::OnInitManageThread(LPS_MULTI_DOWNLOAD_VIEW_PARAM psMultiDownloadViewParam,LPS_MULTI_DOWNLOAD_CONFIG psMultiDownloadConfig,queue_buffer<SDeviceMsg_Info *>* pQueue_buffer)
    {
        mpsMultiDownloadViewParam = psMultiDownloadViewParam;
        mpsMultiDownloadConfig = psMultiDownloadConfig;
        mpQueue_buffer = pQueue_buffer;
        m_bManageClose = false;
    }

    void CCPEMultiDownloadManageThread::RunThread()
    {
#if 0
        if (!mpsMultiDownloadViewParam) return;
        for (int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
        {
            if (ENETCARD_CONNECT_STATUS != mpsMultiDownloadViewParam[i].meNetCardStatus || !mpsMultiDownloadViewParam[i].mbNetCardStatusUpdateFlag) continue;
            PDLOG4WW_F(L"ManageThread Description: %s,IPAddr:%s",mpsMultiDownloadViewParam[i].mstrDescription.c_str(),mpsMultiDownloadViewParam[i].mstrIPAddr.c_str());
            OnUpgradeThread(i);
        }
#else
        if( m_bManageClose ) return;
        if( !mpQueue_buffer || mpQueue_buffer->is_empty() ) return;

        bool bResult = false;
        m_lpsDeviceMsgInfo = mpQueue_buffer->get();
        Log4WD_F(L"[OnStartManageThread] %s",m_lpsDeviceMsgInfo->m_strDbcc_name.c_str());
        if (!mbCheckTFPT32ProcessRuning)
        {
            StartTFTP32Process();
        }
        //! 如果错误 则继续进行处理
        for (int i = 0;i < 3 && !bResult;i ++)
        {
            switch(m_lpsDeviceMsgInfo->m_nDeviceMode)
            {
#if 0
            case E_ADB_MODE:
                bResult = OnSendSwitchModeMsg(SWITCH_EDL_MODE_THREAD_MSG);
                break;

            case E_FASTBOOT_MODE:
                bResult = OnSendSwitchModeMsg(SWITCH_DOWNLOAD_MODE_THREAD_MSG);
                break;
#endif
            case E_DOWNLOADER_MODE:
                bResult = OnDownLoadMode();
                if (bResult) m_lpsDeviceMsgInfo->Dec();
                break;
            default:
                bResult = true;
                m_lpsDeviceMsgInfo->Dec();
                break;
            }
            //如果错误 则继续进行处理
            if (2 == i) m_lpsDeviceMsgInfo->Dec();
        }
#endif
    }

    int CCPEMultiDownloadManageThread::GetDownloadIndexToDeviceMsgInfo()
    {
        return mMultiDownloadSortConfig.GetMultiDownloadSortIndexToDeviceMsg(m_lpsDeviceMsgInfo);
    }

    bool CCPEMultiDownloadManageThread::OnDownLoadMode()
    {
        int nDownloadIndex = GetDownloadIndexToDeviceMsgInfo() - 1;
        if (nDownloadIndex < 0 || nDownloadIndex > 7)
        {
            ::MessageBox(m_hMainWnd,GetString(E_CPE_DOWNLOAD_INDEX_OUTOFRANGE_CHECK_ISSORT),_T("error"),MB_ICONERROR);
            return false;
        }

        //! 如果当前下载线程正在运行则直接返回
        if (E_BUSY_DOWNLOAD == mpsMultiDownloadViewParam[nDownloadIndex].meDownloadStatus)
        {
            PDLOG4WW(L"OnUpgradeThread Download Thread Is Busy");
            //mpsMultiDownloadViewParam[nDownloadIndex].mbNetCardStatusUpdateFlag = false;
            return true;
        }
        //! 检测当前下载线程是否已经开始运行
        if (!mMultiDownloadThreads[nDownloadIndex].IsThreadRun())
        {
            mMultiDownloadThreads[nDownloadIndex].CreateMessageThread(0,0,true);
            mMultiDownloadThreads[nDownloadIndex].InitDownloadParam(mpsMultiDownloadConfig,&mpsMultiDownloadViewParam[nDownloadIndex]);
        }
        //! 重置当前下载线程状态
        mpsMultiDownloadViewParam[nDownloadIndex].meDownloadStatus = E_BUSY_DOWNLOAD;
        mMultiDownloadThreads[nDownloadIndex].PostThreadMessage(DOWN_LOAD_THREAD_RUN_MSG);
        return true;
    }

#if 0
    void CCPEMultiDownloadManageThread::OnUpgradeThread(int nIndex)
    {

        //! 如果当前下载线程正在运行则直接返回
        if (E_BUSY_DOWNLOAD == mpsMultiDownloadViewParam[nIndex].meDownloadStatus)
        {
            PDLOG4WW(L"OnUpgradeThread Download Thread Is Busy");
            mpsMultiDownloadViewParam[nIndex].mbNetCardStatusUpdateFlag = false;
            return ;
        }
        //! 检测IP
        if (!mICMPSocket.Ping(mpsMultiDownloadConfig->mstrHostIPAddrAnsi,JGW_W2A(mpsMultiDownloadViewParam[nIndex].mstrIPAddr).c_str()))
        {
            PELOG4WW_F(L"OnUpgradeThread Ping %s Source %s Fail",mpsMultiDownloadConfig->mstrHostIPAddr.c_str(),mpsMultiDownloadViewParam[nIndex].mstrIPAddr.c_str());
            return ;
        }

        if (!mMultiDownloadThreads[nIndex].IsThreadRun())
        {
            mMultiDownloadThreads[nIndex].CreateMessageThread(0,0,true);
            mMultiDownloadThreads[nIndex].InitDownloadParam(mpsMultiDownloadConfig,&mpsMultiDownloadViewParam[nIndex]);
        }

        mpsMultiDownloadViewParam[nIndex].meDownloadStatus = E_BUSY_DOWNLOAD;
        mMultiDownloadThreads[nIndex].PostThreadMessage(DOWN_LOAD_THREAD_RUN_MSG);
    }
#endif

    void CCPEMultiDownloadManageThread::StartTFTP32Process()
    {
        DWORD processID = JGW_GetProcessID2Name(L"tftpd32.exe");
        if (processID != -1)
        {
            mbCheckTFPT32ProcessRuning = true;
            return ;
        }

        std::wstring strTFTP32ExecFilePath = JGW_RealativePathToAbsPath(L"Tftpd32\\tftpd32.exe");
        if (JGW_PathFileExists(strTFTP32ExecFilePath.c_str()))
        {
            JGW_CreateProcess(strTFTP32ExecFilePath.c_str(),JGW_GetPathOfFile(strTFTP32ExecFilePath.c_str()).c_str());
        }
        mbCheckTFPT32ProcessRuning = true;
    }
}

