#pragma once
#include "MultiDownloadDefine.h"
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JGW_WindowsFuncPlugin/CJGW_ProcessPipe.h>

namespace JGW
{
	class CCPEMultiDownloadThread : public CCMessageThread
	{
	public:
		CCPEMultiDownloadThread(void);
		~CCPEMultiDownloadThread(void);
	public:
		void InitDownloadParam(LPS_MULTI_DOWNLOAD_CONFIG psMultiUpgradeConfig,LPS_MULTI_DOWNLOAD_VIEW_PARAM psMultiDownloadParam);

		LPS_MULTI_DOWNLOAD_VIEW_PARAM GetMultiDownloadViewParam();
	private:
		//! 运行下载线程
		void OnRunDownloadThread(WPARAM wParam,LPARAM lParam);
		//! 开启下载进程
		bool StartDownloadProcess();
		//!
		bool RecvTestInfo();
	protected:
		DECLARE_MYTHREAD_MESSAGE_MAP()
	private:
		//! 
		int mnDonwloadComPort;
		//! 
		int mnSerialComPort;
		//! 
		LPS_MULTI_DOWNLOAD_SORT_CONFIG mpsMultiDownloadSortConfig;
		//! 
		LPS_MULTI_DOWNLOAD_CONFIG mpsMultiDownloadConfig;
		//! 
		LPS_MULTI_DOWNLOAD_VIEW_PARAM mpsMultiDownloadParam;
		//! CMD管道
		CCJGW_ProcessPipe m_pDownloadPipe;
	};
}


