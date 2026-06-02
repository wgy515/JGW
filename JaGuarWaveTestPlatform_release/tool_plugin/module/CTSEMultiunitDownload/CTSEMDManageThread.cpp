#include "StdAfx.h"
#include "CTSEMDManageThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCTSEMDManageThread::CCTSEMDManageThread(std::wstring& strDownloadPortSortConfigFilePath) : mstrDownloadPortSortConfigFilePath(strDownloadPortSortConfigFilePath)
    {
        for (int i = 0;i < MAX_DOWNLOAD_THREAD_COUNT;i ++) mpMDThreads[i].GetDownladProgressInfo().mnIndex = i;
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCTSEMDManageThread,CCMessageThread)
    END_MYTHREAD_MESSAGE_MAP()

    CCTSEMDManageThread::~CCTSEMDManageThread(void)
    {
        ExitSFManageThread();
    }

    void CCTSEMDManageThread::ExitSFManageThread()
    {
        m_bManageClose = true;
    }

    void CCTSEMDManageThread::GetDownloadInfo(int nIndex,S_DOWNLOAD_PROGRESS_INFO** psDownloadProgressInfo)
    {
       *psDownloadProgressInfo = &(mpMDThreads[nIndex].GetDownladProgressInfo());
    }

    bool CCTSEMDManageThread::IsStartDownload()
    {
        return !mmapsort.empty();
    }

    bool CCTSEMDManageThread::ClearSortInfo()
    {
        if (0 == mmapsort.size())
        {
            m_cSortFast.InitDownloadPortSortConfig(mstrDownloadPortSortConfigFilePath);
            m_cSortFast.ClearSortRecorder();
            return true;
        }
        ::MessageBox(m_hMainWnd,L"下载线程正在运行，请重新打开工具,在未下载的情况下点击清空排序",L"非法操作",MB_ICONERROR);
        return false;
    }


    void CCTSEMDManageThread::InitThreadParam(HWND mainHwnd,LPS_TSE_MULTIUNIT_DOWNLOAD_CONFIG psTseMultiunitDownloadConfig,queue_buffer<SDeviceMsg_Info *>* pQueue_buffer)
    {
        m_hMainWnd = mainHwnd;
        for (int i = 0;i < MAX_DOWNLOAD_THREAD_COUNT;i ++) mpMDThreads[i].GetDownladProgressInfo().mpsTSEMDConfig = psTseMultiunitDownloadConfig;
        mpQueue_buffer = pQueue_buffer;
        m_bManageClose = false;
        m_cSortFast.InitDownloadPortSortConfig(mstrDownloadPortSortConfigFilePath);
    }

    void CCTSEMDManageThread::RunThread(void)
    {
        if( m_bManageClose ) return;
        if( !mpQueue_buffer || mpQueue_buffer->is_empty() ) return;

        bool bResult = false;
        m_lpsDeviceMsgInfo = mpQueue_buffer->get();
        Log4WD_F(L"[OnStartManageThread] %s",m_lpsDeviceMsgInfo->m_strDbcc_name.c_str());

        //! 如果错误 则继续进行处理
        for (int i = 0;i < 3 && !bResult;i ++)
        {
            switch(m_lpsDeviceMsgInfo->m_nDeviceMode)
            {
            case E_ADB_MODE:
                bResult = OnSendSwitchModeMsg(SWITCH_EDL_MODE_THREAD_MSG);
                break;
#if 0
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
    }

    //获取线程索引
    int CCTSEMDManageThread::GetThreadWrokID()
    {
        //获取端口索引
        if( -1 == m_lpsDeviceMsgInfo->m_nSortIndex )
        {
            //! m_nBindShowIndex = (m_lpsDeviceMsgInfo->m_nSortIndex + 1);
            Log4WE(L"[GetThreadWrokID]  E_GET_INSTACNCEID_ERROR");
            return E_GET_INSTACNCEID_ERROR;
        }
        //! 如果已经排序  则直接返回排序索引
        std::map<int,int>::iterator it = mmapsort.find(m_lpsDeviceMsgInfo->m_nSortIndex);
        if (mmapsort.end() != it)
        {
            m_nBindShowIndex = it->second;
            return m_nBindShowIndex;
        }

        m_nBindShowIndex = m_cSortFast.GetFastPortShowIndex(m_lpsDeviceMsgInfo->m_nSortIndex);
        m_cSortFast.WriteFastPortRecordFile(m_nBindShowIndex,m_lpsDeviceMsgInfo->m_nSortIndex);
        mmapsort.insert(std::map<int,int>::value_type(m_lpsDeviceMsgInfo->m_nSortIndex,m_nBindShowIndex));

        return m_nBindShowIndex;
    }

    bool CCTSEMDManageThread::OnDownLoadMode()
    {
        //! 如果当前下载数量超过线程数量则报错
        if (GetThreadWrokID() >= MAX_DOWNLOAD_THREAD_COUNT)
        {
            ::MessageBox(m_hMainWnd,_T("当前线程索引超过范围,请检查当前端口是否未进行排序"),_T("error"),MB_ICONERROR);
            return false;
        }

        //! 如果当前线程正在运行，则直接返回false
        if (E_BUSY_DOWNLOAD == mpMDThreads[m_nBindShowIndex - 1].GetDownladProgressInfo().meDownloadStatus) 
        {
            Log4WE(L"Current Download Thread is run");
            return false;
        }
#if 1

        //! 添加下载任务
        {
            if (!mpMDThreads[m_nBindShowIndex - 1].IsThreadRun())
            {
                mpMDThreads[m_nBindShowIndex - 1].CreateMessageThread();
                Sleep(250);
            }
            mpMDThreads[m_nBindShowIndex - 1].GetDownladProgressInfo().mnPos = 1;
            mpMDThreads[m_nBindShowIndex - 1].GetDownladProgressInfo().meDownloadStatus = E_BUSY_DOWNLOAD;   
            mpMDThreads[m_nBindShowIndex - 1].GetDownladProgressInfo().mnComPort = m_lpsDeviceMsgInfo->m_nComPort;
            mpMDThreads[m_nBindShowIndex - 1].PostThreadMessage(DOWN_LOAD_THREAD_RUN_MSG);
            //!mios.post(boost::bind(&CSFManageThread::add_download_work,this,m_nBindShowIndex - 1));
        }
        //! 多线程下载  开启的ASIO RUN线程 TDD: 多线程竞争问题,一般来说线程是不会异常退出的
    /*    {
            if (mgroup.size() < getprocessorssize() && (m_bAddAsioThread || m_nWorkThreadExitCount > 0))
            {
                mgroup.create_thread(boost::bind(&CSFManageThread::asio_work,this));
                if (m_nWorkThreadExitCount > 0) InterlockedDecrement( (LPLONG)&m_nWorkThreadExitCount);
            }
        }*/
#endif
        return true;
    }

    bool CCTSEMDManageThread::OnSendSwitchModeMsg(int nMsgID)
    {
        if (!mUsbModeHandoverThread.IsThreadRun())
        {
            mUsbModeHandoverThread.CreateMessageThread();
            Sleep(250);
        }
        return mUsbModeHandoverThread.PostThreadMessage(nMsgID,WPARAM(m_lpsDeviceMsgInfo),NULL);
    }
}