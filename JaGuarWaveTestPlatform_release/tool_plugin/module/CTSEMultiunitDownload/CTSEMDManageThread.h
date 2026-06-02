#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JGW_FoundationFunc/queue_buffer.hpp>
#include "CTSEMultiunitDonwloadDefine.h"
#include <JGW_DevicePool/DevicePortDefine.h>
#include <JGW_QMSLFuncPlugin/CJGW_UsbModeHandoverThread.h>
#include <JGW_WindowsFuncPlugin/CJGW_DownloadPortSort.h>
#include "CTSEMDThread.h"
namespace JGW
{
    class CCTSEMDManageThread:public CCMessageThread
    {
    public:
        CCTSEMDManageThread(std::wstring& strDownloadPortSortConfigFilePath);
        ~CCTSEMDManageThread(void);
    public:
        //! 初始化线程参数
        void            InitThreadParam(HWND mainHwnd,LPS_TSE_MULTIUNIT_DOWNLOAD_CONFIG psTseMultiunitDownloadConfig,queue_buffer<SDeviceMsg_Info*>*    pQueue_buffer);
        //! 清除排序信息
        bool            ClearSortInfo();
        //! 是否已经开始下载
        bool            IsStartDownload();
        //! 获取下载信息
        void            GetDownloadInfo(int nIndex,S_DOWNLOAD_PROGRESS_INFO** psDownloadProgressInfo);
        //! 退出下载管理线程
        void            ExitSFManageThread(); 
    private:
        void            RunThread(void);
        //! 发送切换模式命令
        bool			OnSendSwitchModeMsg(int nMsgID);
        //! 准备下载
        bool            OnDownLoadMode();

        int             GetThreadWrokID();
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        //! 管理者线程是否关闭
        bool                    m_bManageClose;
        //! 当前下载进度条索引 从1开始
        int                     m_nBindShowIndex;
        //! 窗口句柄
        HWND                    m_hMainWnd;
        //! 设备消息队列
        JGW::queue_buffer<SDeviceMsg_Info*>* mpQueue_buffer;
        //! 当前设备消息
        SDeviceMsg_Info* m_lpsDeviceMsgInfo;
        //!
        std::wstring& mstrDownloadPortSortConfigFilePath;
        //! 
        CCTSEMDThread           mpMDThreads[MAX_DOWNLOAD_THREAD_COUNT];
        //! 排序
        CCJGW_DownloadPortSort m_cSortFast;
        //! key sort索引 value是界面索引
        std::map<int,int> mmapsort;
        CCJGW_UsbModeHandoverThread mUsbModeHandoverThread;
    };
}