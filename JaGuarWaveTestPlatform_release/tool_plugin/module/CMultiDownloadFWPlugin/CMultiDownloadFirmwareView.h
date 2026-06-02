#pragma once
#include "../../../platform_include/CJGWUiLibViewResolverImpl.h"
#include <UiLib/UIlib.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include "../../../platform_include/CJGWUiLibViewResolver_Define.h"
#include "PumaDownload_Define.h"
#include "SFManageThread.h"
#include "CJGW_PumaImageFmtData.h"
// #include <JGW_FoundationFunc/queue_buffer.hpp>
// #include <JGW_DevicePool/DevicePortDefine.h>

namespace JGW
{
    class CSFManageThread;

    class CCMultiDownloadFirmwareView:public CCJGWUiLibViewResolverImpl
    {
    public:
        CCMultiDownloadFirmwareView(void);
        ~CCMultiDownloadFirmwareView(void);
    protected://! 继承CCJGWUiLibViewResolverImpl 接口类
        //! 初始化
        bool InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam);

        void OnNotifySubWndPlugin(LPVOID pMsg);

        void CloseSubWndPlugin();
    private:
        //! 响应改变下载路径选择按钮
        void OnChangeDownloadPathChooseBtn();
        //! 开启管理线程
        void StartMagagerThread();
        //! 加载下载配置文件
        void LoadDownloadConfigFile();
        //!
        void UpdateUIShow();
        //! 
        bool GetDuiContronlClass(void);
        //!
        void UpdateDownloadInfo(int nIndex,const S_DOWNLOAD_PROGRESS_INFO* psDownloadProgressInfo);
        //! 上报PUMA 设备消息，因为puma设备不会主动上报
        void OnReportedPumaDeviceMsg(const WPARAM& wParam,const LPARAM& lParam);
        //! 响应设备插入消息
        void OnResponseDeviceMsg(const WPARAM& wParam,const LPARAM& lParam);
        //! 响应定时器消息
        void OnResponseTimer(const WPARAM& wParam,const LPARAM& lParam);
    private:
        //! 下载初始化是否OK
        bool m_bIsInitDownLoadPathOk;
        //! 窗口句柄
        HWND mMainHwnd;

        CButtonUI* m_pClearSortBtn;
        CButtonUI* m_pChooseImageBtn;

        CLabelUI* m_pDownImagePathLabel;

        CCheckBoxUI* mpValidateImageComCheckBox;
        CCheckBoxUI* m_pAutoSwitchComCheckBox;
        CCheckBoxUI* mpFormatPDBComCheckBox;
        //! 参数试图信息
        LPS_VIEW_RESOLVER_PARAM mpsViewResolverParam;

        CPaintManagerUI* mpPluginPaintManagerUI;
        //! 下载管理线程
        CSFManageThread* m_pSFManageThread;
        //! 设备消息队列
        queue_buffer<SDeviceMsg_Info*> msDeviceMsgQueue;

        CTextUI* m_pstatusText[DOWNLOAD_THREAD_MAX];
        CLabelUI* m_pcomLabel[DOWNLOAD_THREAD_MAX];
        CButtonUI* m_plogBtn[DOWNLOAD_THREAD_MAX];
        CProgressUI* m_pDownLoadProgress[DOWNLOAD_THREAD_MAX];
        //!
        CCJGW_ConfigIni	mDownloadConfig;
        //! 多路下载配置
        SMultiunitDownloadFWParam msMultiDFWParam;
        //! puma下载信息
        S_PUMA_DOWNLOAD_INFO msPumaDownloadInfo;
        //! 解析PUMA IMAGE文件
        CCJGW_PumaImageFmtData mPumaImageFmtData;
        //! 已经存在的PID VID
        std::vector<std::wstring> mvstrExitsPidVid;
    };
}


