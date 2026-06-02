#pragma once
#include "../../../platform_include/CJGWUiLibViewResolverImpl.h"
#include <UiLib/UIlib.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include "../../../platform_include/CJGWUiLibViewResolver_Define.h"
#include "SFManageThread.h"
#include "CJGW_DownloadConfig.h"

class CSFManageThread;
namespace JGW
{
    class CCBoostDownloadView:public CCJGWUiLibViewResolverImpl
    {
    public:
        CCBoostDownloadView(void);
        ~CCBoostDownloadView(void);
    protected://! 继承CCJGWUiLibViewResolverImpl 接口类
        bool InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam);

        void OnNotifySubWndPlugin(LPVOID pMsg);

        void CloseSubWndPlugin();
    private:
        void UpdateDownloadInfo(int nIndex,const S_DOWNLOAD_PROGRESS_INFO* psDownloadProgressInfo);

        //! 响应设备插入消息
        void OnResponseDeviceMsg( const WPARAM& wParam,const LPARAM& lParam );
        //! 响应定时器消息
        void OnResponseTimer( const WPARAM& wParam,const LPARAM& lParam );
        //! 获取DUI控件指针
        bool GetDuiContronlClass();
        //! 保存配置文件
        void SaveIniConfigFile();
        //! 加载配置文件
        void LoadIniConfigFile();
        //! 更新UI显示
        void UpdateUIShow();
        //! 响应改变下载路径选择按钮
        void OnChangeDownloadPathChooseBtn();
        //! 开启下载管理线程
        void StartMagagerThread();
    protected:
        //! LPS_VIEW_RESOLVER_PARAM mpViewResolverParam;
        CPaintManagerUI* mpPluginPaintManagerUI;

        HWND				mMainHwnd;
        bool				m_bIsInitDownLoadPathOk;
        //! 下载管理线程
        CSFManageThread*	            m_pSFManageThread;
        //! UI实例指针
        CEditUI*			m_pModeEdt;
        CLabelUI*			m_pCrcLabel;
        CLabelUI*			m_pDownPathLabel;
        CEditUI*			m_pProgrammerText;
        CEditUI*			m_pRawprogramText;
        CEditUI*			m_pPatchText;
        CButtonUI*			m_pClearSortBtn;
        CButtonUI*			m_pChooseFloderbtn;
        CCheckBoxUI*		m_pAutoSwitchComCheckBox;
        CCheckBoxUI*		m_pResetAfterDownloadCheckBox;
        CProgressUI*		m_pDownLoadProgress[DOWNLOAD_THREAD_MAX];
        CTextUI*			m_pstatusText[DOWNLOAD_THREAD_MAX];
        CLabelUI*			m_pcomLabel[DOWNLOAD_THREAD_MAX];
        CButtonUI*			m_plogBtn[DOWNLOAD_THREAD_MAX];
        //! 加载所有下载信息
        ps_download_all_file_info   mps_download_all_file_info;
        //std::wstring		m_strIniConfigPath;
        CCJGW_ConfigIni	mDownloadIniConfig;
        //! 下载配置信息
        CCJGW_DownloadConfig    mdownloadConfig;
        LPS_VIEW_RESOLVER_PARAM             mpsViewResolverParam;
        queue_buffer<SDeviceMsg_Info*> mqueue_buffer;
    };
}