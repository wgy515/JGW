#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JGW_FoundationFunc/queue_buffer.hpp>
#include "MultiDownloadDefine.h"
#include <JGW_DevicePool/DevicePortDefine.h>

#include "CPEMultiDownloadThread.h"
#include "CPEMultiDownloadSortConfig.h"

namespace JGW
{
	class CCPEMultiDownloadManageThread : public CCMessageThread
	{
	public:
		CCPEMultiDownloadManageThread(CCPEMultiDownloadSortConfig& multiDownloadSortConfig);
		~CCPEMultiDownloadManageThread(void);
	public:
		//! 初始化管理线程
		void OnInitManageThread(LPS_MULTI_DOWNLOAD_VIEW_PARAM psMultiDownloadViewParam,LPS_MULTI_DOWNLOAD_CONFIG psMultiDownloadConfig,queue_buffer<SDeviceMsg_Info*>* pQueue_buffer);
		//! 退出管理线程
		void ExitManageThread();
	private:
		//! 运行线程
		void RunThread();
		//! 准备下载
		bool OnDownLoadMode();
		//! 获取当前线程索引
		int GetDownloadIndexToDeviceMsgInfo();
        //!
        void StartTFTP32Process();
	private:
        bool mbCheckTFPT32ProcessRuning;
		//! 当前下载进度条索引 从1开始
		int                     m_nBindShowIndex;
		//! 窗口句柄
		HWND                    m_hMainWnd;
		//! 设备消息队列
		queue_buffer<SDeviceMsg_Info*>* mpQueue_buffer;
		//! 当前设备消息
		SDeviceMsg_Info* m_lpsDeviceMsgInfo;
		//! 管理者线程是否关闭
		bool                    m_bManageClose;

		LPS_MULTI_DOWNLOAD_CONFIG mpsMultiDownloadConfig;

		LPS_MULTI_DOWNLOAD_VIEW_PARAM mpsMultiDownloadViewParam;

		CCPEMultiDownloadSortConfig& mMultiDownloadSortConfig;

		CCPEMultiDownloadThread mMultiDownloadThreads[DOWNLOAD_THREAD_MAX];
	private:
		DECLARE_MYTHREAD_MESSAGE_MAP()
	};
}

