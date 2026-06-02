#pragma once
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/threadpool.hpp>

#include "PumaDownload_Define.h"
#include "CJGW_DownloadPumaAsioSerialPort.h"
#include "CPumaHandoverModeThread.h"
#include <JGW_WindowsFuncPlugin/CJGW_DownloadPortSort.h>

#include <JGW_DevicePool/DevicePortDefine.h>

#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JGW_FoundationFunc/queue_buffer.hpp>

namespace JGW
{
	class CSFManageThread:public CCMessageThread
	{
	public:
		CSFManageThread(std::wstring& strDownloadPortSortConfigFilePath);
		~CSFManageThread(void);
		//! 初始化线程参数
		void            InitThreadParam(HWND mainHwnd,PS_PUMA_DOWNLOAD_INFO psPumaDownloadInfo,queue_buffer<SDeviceMsg_Info*>* pQueue_buffer);
		//! 清除排序信息
		bool            ClearSortInfo();
		//! 是否已经开始下载
		bool            IsStartDownload();
		//! 获取下载信息
		void            GetDownloadInfo(int nIndex,const S_DOWNLOAD_PROGRESS_INFO** psDownloadProgressInfo);
		//! 退出下载管理线程
		void            ExitSFManageThread(); 
	private:
		//! 运行线程
		void			RunThread();
		//! 发送切换模式命令
		bool			OnSendSwitchModeMsg(int nMsgID);
		//! 准备下载
		bool            OnDownLoadMode();
		//获取线程索引
		int             GetThreadWrokID();
		//! 空的异步延迟函数，使asio不退出
		void			empty_deadline_func();
		//! 添加下载任务到asio
		void			add_download_work(int npos);
		//! 运行 asio任务
		void            asio_work(/*std::string strPort,int nBindShowIndex*/);
	private:
		//! 是否新增下载新城
		bool					m_bAddAsioThread;
        //! 是否有线程退出
        long                    m_nWorkThreadExitCount;
		//! 管理者线程是否关闭
		bool                    m_bManageClose;
		//! 当前下载进度条索引 从1开始
		int                     m_nBindShowIndex;
		//! 窗口句柄
		HWND                    m_hMainWnd;
		//! 设备消息队列
		queue_buffer<SDeviceMsg_Info*>* mpQueue_buffer;
		//! firehose下载协议
        PS_PUMA_DOWNLOAD_INFO mpsPumaDownloadInfo;
		//! 当前设备消息
		SDeviceMsg_Info* m_lpsDeviceMsgInfo;
		//! asio下载实例类
		CCJGW_DownloadPumaAsioSerialPort* mpDownloadPumaAspClasss[DOWNLOAD_THREAD_MAX];
        //! 下载端口排序索引配置文件路径
        std::wstring& mstrDownloadPortSortConfigFilePath;
		//! 切换模式线程
		CCPumaHandoverModeThread m_cSwithThread;
		//! 排序
		CCJGW_DownloadPortSort mDownloadPortSort;
		//! 线程组
		boost::thread_group mgroup ;
		//!
		boost::asio::io_service mios;
		//! 定时器
		boost::asio::deadline_timer mdeadline_;
		//! key sort索引 value是界面索引
		std::map<int,int> mmapsort;

		DECLARE_MYTHREAD_MESSAGE_MAP();
	};
}
