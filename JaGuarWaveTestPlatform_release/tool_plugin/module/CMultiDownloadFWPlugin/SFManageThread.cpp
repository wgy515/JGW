#include "StdAfx.h"
#include "SFManageThread.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
namespace JGW
{

	size_t GetProcessorsSize(void) 
	{
		static DWORD dwNumberOfProcessors = 0;
		if (0 == dwNumberOfProcessors) 
		{
			SYSTEM_INFO system_info;
			GetSystemInfo (&system_info);
			dwNumberOfProcessors = system_info.dwNumberOfProcessors;
		}
		return dwNumberOfProcessors;
	}

	CSFManageThread::CSFManageThread(std::wstring& strDownloadPortSortConfigFilePath) : m_bManageClose(false)
		,mdeadline_(mios,boost::posix_time::seconds(100))
        ,mpsPumaDownloadInfo(NULL)
        ,m_nWorkThreadExitCount(0)
        ,mstrDownloadPortSortConfigFilePath(strDownloadPortSortConfigFilePath)
	{
		for (int i = 0;i < DOWNLOAD_THREAD_MAX;i ++) mpDownloadPumaAspClasss[i] = new CCJGW_DownloadPumaAsioSerialPort(mios,i);
		mdeadline_.async_wait(boost::bind(&CSFManageThread::empty_deadline_func,this));
	}

    BEGIN_MYTHREAD_MESSAGE_MAP(CSFManageThread,CCMessageThread)
    END_MYTHREAD_MESSAGE_MAP()

    CSFManageThread::~CSFManageThread(void)
    {
        ExitSFManageThread();
    }

    void CSFManageThread::ExitSFManageThread()
    {
        m_bManageClose = true;
        for (int i = 0;i < DOWNLOAD_THREAD_MAX;i ++) delete mpDownloadPumaAspClasss[i];
    }

    void CSFManageThread::GetDownloadInfo(int nIndex,const S_DOWNLOAD_PROGRESS_INFO** psDownloadProgressInfo)
    {
		*psDownloadProgressInfo = &mpDownloadPumaAspClasss[nIndex]->GetDownladProgressInfo();
    }

    bool CSFManageThread::IsStartDownload()
    {
        return !mmapsort.empty();
    }

    bool CSFManageThread::ClearSortInfo()
    {
        if (0 == mmapsort.size())
        {
            mDownloadPortSort.InitDownloadPortSortConfig(mstrDownloadPortSortConfigFilePath);
            mDownloadPortSort.ClearSortRecorder();
            return true;
        }
        ::MessageBox(m_hMainWnd,GetString(E_CPE_DOWNLOAD_NOT_ALLOW_CHANGE_CONFIG),GetString(E_PROMPT_TYPE),MB_ICONERROR);
        return false;
    }

	void CSFManageThread::empty_deadline_func()
	{
		mdeadline_.expires_from_now(boost::posix_time::seconds(1000));
		mdeadline_.async_wait(boost::bind(&CSFManageThread::empty_deadline_func,this));
	}

    void CSFManageThread::InitThreadParam(HWND mainHwnd,PS_PUMA_DOWNLOAD_INFO psPumaDownloadInfo,JGW::queue_buffer<SDeviceMsg_Info*>* pQueue_buffer)
    {
        m_hMainWnd = mainHwnd;
        mpsPumaDownloadInfo = psPumaDownloadInfo;
        mpQueue_buffer = pQueue_buffer;
        m_bManageClose = false;
        mDownloadPortSort.InitDownloadPortSortConfig(mstrDownloadPortSortConfigFilePath);
    }

    void CSFManageThread::RunThread()
    {
        if( m_bManageClose ) return;
        if( !mpQueue_buffer || mpQueue_buffer->is_empty() ) return;

        bool bResult = false;
        m_lpsDeviceMsgInfo = mpQueue_buffer->get();
        Log4WD_F(L"[OnStartManageThread] %s",m_lpsDeviceMsgInfo->m_strDbcc_name.c_str());

        //! 如果错误 则继续进行处理
        for (int i = 0;i < 3;i ++)
        {
			m_bAddAsioThread = false;
            switch(m_lpsDeviceMsgInfo->m_nDeviceMode)
            {
#if 1
            case E_DIAG_MODE:
                bResult = OnSendSwitchModeMsg(SWITCH_EDL_MODE_THREAD_MSG);
                break;
            case E_FASTBOOT_MODE:
                bResult = OnSendSwitchModeMsg(SWITCH_DOWNLOAD_MODE_THREAD_MSG);
                break;
            case E_DOWNLOADER_MODE:
                bResult = OnDownLoadMode();
                //! if (bResult) m_lpsDeviceMsgInfo->Dec();
                break;
#endif
            default:
                bResult = true;
                //m_lpsDeviceMsgInfo->Dec();
                break;
            }
            if (bResult) { m_lpsDeviceMsgInfo->Dec(); return;}
            //如果错误 则继续进行处理
            if (2 == i) m_lpsDeviceMsgInfo->Dec();
        }
    }

	void CSFManageThread::asio_work()
	{
		try
		{
			mios.run();
		}
		catch (boost::system::system_error& se)
		{
            InterlockedIncrement( (LPLONG)&m_nWorkThreadExitCount );
			Log4WE(JGW_A2W_A(se.what()).c_str());        
		}
	}

	void CSFManageThread::add_download_work(int npos)
	{
		try
		{
			g_strDownloadDubugLog[npos] = L"";
			mpDownloadPumaAspClasss[npos]->StartDownloadPumaImgae(mpsPumaDownloadInfo);
		}
		catch (boost::system::system_error& se)
		{
			g_strDownloadDubugLog[npos] += JGW_A2W_A(se.what());
			Log4WE(JGW_A2W_A(se.what()).c_str());  
			mpDownloadPumaAspClasss[npos]->GetDownladProgressInfo().meDownloadStatus = E_ERROR_DOWNLOAD;
		}
	}

    bool CSFManageThread::OnDownLoadMode()
    {
		//! 如果当前下载数量超过线程数量则报错
        if (GetThreadWrokID() >= DOWNLOAD_THREAD_MAX)
        {
            ::MessageBox(m_hMainWnd,GetString(E_CPE_DOWNLOAD_INDEX_OUTOFRANGE_CHECK_ISSORT),_T("error"),MB_ICONERROR);
            return false;
        }

        //! 如果当前线程正在运行，则直接返回false
        if (E_BUSY_DOWNLOAD == mpDownloadPumaAspClasss[m_nBindShowIndex - 1]->GetDownladProgressInfo().meDownloadStatus) 
        {
            Log4WE(L"Current Download Thread is run");
            return false;
        }
#if 1
        //! 添加下载任务
		{
			std::string strPort;
			JGW_FormatString(strPort,"COM%d",m_lpsDeviceMsgInfo->m_nComPort);
			mpDownloadPumaAspClasss[m_nBindShowIndex - 1]->GetDownladProgressInfo().meDownloadStatus = E_BUSY_DOWNLOAD;
			mpDownloadPumaAspClasss[m_nBindShowIndex - 1]->GetDownladProgressInfo().mstrComPort = strPort;
			mios.post(boost::bind(&CSFManageThread::add_download_work,this,m_nBindShowIndex - 1));
		}
        //! 多线程下载  开启的ASIO RUN线程 TDD: 多线程竞争问题,一般来说线程是不会异常退出的
		{
			if (mgroup.size() < GetProcessorsSize() && (m_bAddAsioThread || m_nWorkThreadExitCount > 0))
			{
				mgroup.create_thread(boost::bind(&CSFManageThread::asio_work,this));
                if (m_nWorkThreadExitCount > 0) InterlockedDecrement( (LPLONG)&m_nWorkThreadExitCount);
			}
		}
#endif
        return true;
    }

    //获取线程索引
    int CSFManageThread::GetThreadWrokID()
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

        m_nBindShowIndex = mDownloadPortSort.GetFastPortShowIndex(m_lpsDeviceMsgInfo->m_nSortIndex);
        mDownloadPortSort.WriteFastPortRecordFile(m_nBindShowIndex,m_lpsDeviceMsgInfo->m_nSortIndex);
        mmapsort.insert(std::map<int,int>::value_type(m_lpsDeviceMsgInfo->m_nSortIndex,m_nBindShowIndex));
		m_bAddAsioThread = true;

        return m_nBindShowIndex;
    }

    bool CSFManageThread::OnSendSwitchModeMsg(int nMsgID)
    {
#if 1
        if ( !m_cSwithThread.IsThreadRun() )
        {
            m_cSwithThread.CreateMessageThread();
            Sleep(250);
        }

        m_cSwithThread.PostThreadMessage(nMsgID,WPARAM(m_lpsDeviceMsgInfo),LPARAM(mpsPumaDownloadInfo));
#endif 
        return true;
    }
}
