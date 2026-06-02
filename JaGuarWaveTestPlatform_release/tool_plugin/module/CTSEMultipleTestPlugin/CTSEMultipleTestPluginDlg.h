#pragma once
#include "../../../platform_include/CJGWUiLibViewResolverImpl.h"
#include <UiLib/UIlib.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include "../../../platform_include/CJGWUiLibViewResolver_Define.h"
#include "CTSEMultipleTestDefine.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include "CTSEMultipleTestManageThread.h"
#include "CJGW_SortRecordConfig.h"

namespace JGW
{
    typedef struct
    {
        int mnProcessPos;
        CLabelUI* mpIndexLabel;
        CProgressUI* mpDownloadProcess;
        //CButtonUI* mpStartButton;
        CLabelUI* mpStatusLabel;
        CTextUI* mpDeviceDesTextUI;
        CHorizontalLayoutUI* mpStatusHorizontal;
        CRichEditUI* mpLogRickEdit;
    }S_MULTIPLE_VIEW_INFO,*PS_MULTIPLE_VIEW_INFO;

    class CCTSEMultipleTestPluginDlg : public CCJGWUiLibViewResolverImpl
    {
    public:
        CCTSEMultipleTestPluginDlg(void);
        ~CCTSEMultipleTestPluginDlg(void);
    protected://! 继承CCJGWUiLibViewResolverImpl 接口类
        //! 初始化
        bool InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam);

        void OnNotifySubWndPlugin(LPVOID pMsg);

        void CloseSubWndPlugin();
    private:
        bool LoadTSEMultipleTestConfig();
        //! 清空开始测试点击事件
        void ClearStartMsgNotifyClick();
        //!
        void ClearTestLogUI();
        //! 响应点击开始测试事件
        bool OnStartMsgBtnClick(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam);
        //! 初始多路测试视图
        void InitMultipleView();
        //! 更新当前视图状态信息
        void UpdateMultipleSingleView(S_TSE_MULTIPLE_TEST_THREAD_PARAM& sThreadParam,S_MULTIPLE_VIEW_INFO& sMultipleViewInfo);
        //! 响应网卡状态改变消息
        void OnResponseNetCardChangeMSG(const WPARAM& wParam,const LPARAM& lParam);
        //! 
        void CheckInitMultipleTestIsOk();
        //! 解析下载配置文件
        bool ParseDownloadFolderConfig();
        //！更改下载路径
        void OnChangeDownloadPathChooseBtn();
        //!
        void OnClickConfigBtn();
        //! 
        void OnResponseTimer(const WPARAM& wParam,const LPARAM& lParam);
        //! 保存下载配置文件
        void SaveDownloadConfig(size_t row,size_t col,bool bBinaryImageIsFolder);
    private:
        bool mbInitMultipleTestOk;
        //! 
        CButtonUI* mpChooseBtn;
        //! 
        CLabelUI* mpBinaryImagePathLabel;
        //! 
        CButtonUI* mpConfigLanBtn;

        LPS_VIEW_RESOLVER_PARAM mpsViewResolverParam;
        //! 
        CPaintManagerUI* mpPluginPaintManagerUI;
        //! 
        CVerticalLayoutUI* mpMainMultipleVerticalLayout;
        //! 多路下载测试参数
        S_MULTIPLE_CONFIG msMultipleConfig;
        //! 多路下载视图
        std::vector<S_MULTIPLE_VIEW_INFO> mvMultipleTestView;
        std::vector<S_TSE_MULTIPLE_TEST_THREAD_PARAM> mvsMultipleTestThreadParam;
        //! 
        CCJGW_ConfigIni mcTSEMultipleTestConfig;
        //!
        CCJGW_SortRecordConfig mcSortRecordConfig;
        //! 
        CCTSEMultipleTestManageThread mcTSEMultipleTestManageThread;
    };
}