#pragma once
#include "../../../platform_include/CJGWUiLibViewResolverImpl.h"
#include <UiLib/UIlib.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include "../../../platform_include/CJGWUiLibViewResolver_Define.h"
#include "CTSEMDConfig.h"
#include "CTSEMDManageThread.h"

namespace JGW
{
    class CCTSEMultiunitDownloadView:public CCJGWUiLibViewResolverImpl
    {
    public:
        CCTSEMultiunitDownloadView(void);
        ~CCTSEMultiunitDownloadView(void);
    protected://! 继承CCJGWUiLibViewResolverImpl 接口类
        bool InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam);

        void OnNotifySubWndPlugin(LPVOID pMsg);

        void CloseSubWndPlugin();
    private:
        void UpdateDownloadInfo(int nIndex,S_DOWNLOAD_PROGRESS_INFO* psDownloadProgressInfo);

        //! 响应设备插入消息
        void OnResponseDeviceMsg( const WPARAM& wParam,const LPARAM& lParam );
        //! 响应定时器消息
        void OnResponseTimer( const WPARAM& wParam,const LPARAM& lParam );
        //! 获取DUI控件指针
        bool GetDuiContronlClass();
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
        CCTSEMDManageThread* m_pTSEMDManageThread;
        LPS_TSE_MULTIUNIT_DOWNLOAD_CONFIG mpsTseMultiunitDownloadConfig;
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
        std::wstring        mstrIniConfigPath;
        CCTSEMDConfig       mMultiunitDownloadConfig;    
        LPS_VIEW_RESOLVER_PARAM mpsViewResolverParam;
        queue_buffer<SDeviceMsg_Info*> mqueue_buffer;
    };
}