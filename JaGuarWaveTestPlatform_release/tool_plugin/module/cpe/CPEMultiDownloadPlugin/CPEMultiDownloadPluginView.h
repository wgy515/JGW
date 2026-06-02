#pragma once
#include "../../../../platform_include/CJGWUiLibViewResolverImpl.h"
#include <UiLib/UIlib.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include "../../../../platform_include/CJGWUiLibViewResolver_Define.h"
#include "MultiDownloadDefine.h"
#include "CPEMultiDownloadSortConfig.h"
#include "CPEMultiDownloadConfig.h"
#include "CPEMultiDownloadManageThread.h"

namespace JGW
{
    class CCPEMultiDownloadPluginView : public CCJGWUiLibViewResolverImpl
    {
    public:
        CCPEMultiDownloadPluginView(void);
        ~CCPEMultiDownloadPluginView(void);
    protected://! 继承CCJGWUiLibViewResolverImpl 接口类
        //! 初始化
        bool InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam);

        void OnNotifySubWndPlugin(LPVOID pMsg);

        void CloseSubWndPlugin();
    private:
        //! 
        bool GetDuiContronlClass();
        //! 定时器回调函数
        void OnResponseTimer(const WPARAM& wParam,const LPARAM& lParam);
        //! 更新下载状态信息
        void UpdateDownloadInfo(int nIndex);
        //! 响应配置按钮点击信息
        void OnClickConfigBtn();
        //! 加载下载配置文件
        void InitDownloadConfig();
        //! 保存下载配置文件
        void SaveDownloadConfig();
        //! 初始化UI
        void InitUIConfig();
        //! 
        void OnResponseDeviceMsg(const WPARAM& wParam,const LPARAM& lParam);
		//! 根据下载路径检查下载配置文件是否都存在
		void CheckDownloadPathConfig();
		//! 下载路径更改
		void OnChangeDownloadPathChooseBtn();
    private:
		//! 当前是否已经有设备插入
		bool mbInsertedDevice;
		//! 判断当前下载路径是否已经初始化
        bool m_bIsInitDownLoadPathOk;
        //! 窗口句柄
        HWND mMainHwnd;
        //! 参数试图信息
        LPS_VIEW_RESOLVER_PARAM mpsViewResolverParam;
        //! 
        CPaintManagerUI* mpPluginPaintManagerUI;
        //! UI ptr
        CButtonUI* m_pClearSortBtn;
        CButtonUI* m_pChooseImageBtn;
        CButtonUI* m_pConfigLanBtn;
        CLabelUI* m_pDownImagePathLabel;
        CTextUI* m_pstatusText[DOWNLOAD_THREAD_MAX];
        CLabelUI* m_pcomLabel[DOWNLOAD_THREAD_MAX];
        CButtonUI* m_plogBtn[DOWNLOAD_THREAD_MAX];
        CProgressUI* m_pDownLoadProgress[DOWNLOAD_THREAD_MAX];
        //! 多路下载参数
        S_MULTI_DOWNLOAD_VIEW_PARAM msMultiDownloadViewParam[DOWNLOAD_THREAD_MAX];
        //! 下载相关配置
        S_MULTI_DOWNLOAD_CONFIG msMultiDownloadConfig;
		//! 设备插入信息队列
		queue_buffer<SDeviceMsg_Info*> mqueue_buffer;
		//! 多路下载排序配置
		CCPEMultiDownloadSortConfig mMultiDownloadSortConfig;
		//! 多路下载管理线程
		CCPEMultiDownloadManageThread mMultiDownloadManageThread;
		//! 下多路载配置
		CCPEMultiDownloadConfig mMultiDownloadConfig;
    };
}