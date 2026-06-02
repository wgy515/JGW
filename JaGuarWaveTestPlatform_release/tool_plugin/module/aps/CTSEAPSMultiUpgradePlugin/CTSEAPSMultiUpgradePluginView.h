#pragma once
#include "../../../../platform_include/CJGWUiLibViewResolverImpl.h"
#include <UiLib/UIlib.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include "../../../../platform_include/CJGWUiLibViewResolver_Define.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>

#include "CTSEAPS_UpgradeDefine.h"
#include "CTSEAPSMultiUpgreadManageThread.h"
#include "CJGW_LanSortConfig.h"

#include <map>
#include <iphlpapi.h>

namespace JGW
{
    class CCTSEAPSMultiUpgradePluginView : public CCJGWUiLibViewResolverImpl
    {
    public:
        CCTSEAPSMultiUpgradePluginView(void);
        ~CCTSEAPSMultiUpgradePluginView(void);
    protected://! 继承CCJGWUiLibViewResolverImpl 接口类
        //! 初始化
        bool InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam);

        void OnNotifySubWndPlugin(LPVOID pMsg);

        void CloseSubWndPlugin();
    private:
        //! 
        bool GetDuiContronlClass();
        //! 响应改变下载路径选择按钮
        void OnChangeDownloadPathChooseBtn();
        //! 响应配置按钮点击信息
        void OnClickConfigBtn();
        //! 加载下载配置文件
        void InitDownloadConfig();
        //! 保存下载配置文件
        void SaveDownloadConfig();
        //! 初始化UI
        void InitUIConfig();
        //!
        void UpdateDownloadInfo(int nIndex);
        //!
        bool ParseDownloadFolderConfig();
    private:   
        void OnResponseTimer(const WPARAM& wParam,const LPARAM& lParam);
        void UpdateDeviceInfo();
        LPS_NET_CARD_PARAM GetNetCardParamStruct(const std::wstring& strDescription);
        //! 获取网络适配器信息
        bool GetAdapterInfo();
        //!
        bool InitNetCardParamToAdapterInfo();
        //! 
        bool GetIfTableInfo();
    private:
        //! 窗口句柄
        HWND mMainHwnd;
        //! 参数试图信息
        LPS_VIEW_RESOLVER_PARAM mpsViewResolverParam;
        //! 
        CPaintManagerUI* mpPluginPaintManagerUI;
        //! net ptr
        size_t mnIpAdapterInfoMemSize;
        PIP_ADAPTER_INFO mpIpAdapterInfo;

        size_t mnIfTableMemSize;
        MIB_IFTABLE* mpIfTable;
        //! UI ptr
        CButtonUI* m_pClearSortBtn;
        CButtonUI* m_pChooseImageBtn;
        CButtonUI* m_pConfigLanBtn;
        CLabelUI* m_pDownImagePathLabel;
        CTextUI* m_pstatusText[DOWNLOAD_THREAD_MAX];
        CLabelUI* m_pcomLabel[DOWNLOAD_THREAD_MAX];
        CButtonUI* m_plogBtn[DOWNLOAD_THREAD_MAX];
        CProgressUI* m_pDownLoadProgress[DOWNLOAD_THREAD_MAX];
        //! 
        S_NET_CARD_PARAM msNetCardParam[DOWNLOAD_THREAD_MAX];
        //! 
        std::map<std::wstring,LPS_NET_CARD_PARAM> mmapNetCardInfos;
        //!
        CCJGW_ConfigIni	mDownloadConfig;
        //!
        CCJGW_LanSortConfig mLanSortConfig;
        //!
        S_APS_MULTI_UPGRADE_CONFIG msAPSMultiUpgradeConfig;
        //! 
        CCTSEAPSMultiUpgreadManageThread mMultiUpgreadManageThread;
    };
}