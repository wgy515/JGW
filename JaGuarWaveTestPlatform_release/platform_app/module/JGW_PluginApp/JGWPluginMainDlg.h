#pragma once
#include <UiLib/CUiLibWnd.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include <JaGuarWaveTestPlatform/CJGW_PocoClassLoader.h>
#include <JaGuarWaveTestPlatform/JGWLoadSubPlugin.h>
#include <poco/NotificationCenter.h>
#include <JGW_DevicePool/DevicePortDefine.h>
#include "../../../platform_include/JGW_DevicePoolModule_Impl.h"
#include <JGWUIEvent.h>

namespace JGW
{
    class CJGWPluginMainDlg:public CCUiLibWnd
    {
    public:
        CJGWPluginMainDlg(int nPluginAppID,HWND hMainTestPlatformWindow,S_MES_INFO& sMesInfo,S_PLUGIN_INFO& sPluginInfo);
        ~CJGWPluginMainDlg(void);
    private: //! virtual
        //! UI事件消息
        void OnNotify(TNotifyUI& msg);
        //!
        UILIB_GetWindowClassName(CCJGWMainDlg);
        //! 初始化窗口
        void OnInitWindow();
        //! 窗口正在关闭
        void OnCloseWindow();
        //！窗口准备退出
        virtual void OnFinalMessage(HWND hWnd);
        DUI_DECLARE_MESSAGE_MAP_B()
    private:
        //! 
        void OnSelectChanged(TNotifyUI& msg);
        //! 响应log点击按钮
        void OnRspLogBtnClick(TNotifyUI& msg);
        //!
        void OnResponceViewLogInfoMsg(WPARAM wParam, LPARAM lParam);
        //! return false 不继续下发， true 继续下发消息 默认返回true
        bool OnEventCustomMessage( TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam );
        //! 
        void OnResponceTimeMsg( WPARAM wParam, LPARAM lParam );
    private:
        //! 关闭子窗体
        void CloseSubPluginView();
        //! 卸载子插件
        void DestroySubPlugin();
        //! 最大化LOG窗口
        void OnMaximizeLogLayoutBtnClick(TNotifyUI& msg);
    private:
        CJGWDuiiLlibSubWndImpl* GetDuiLibSubWndImplPtr(int viewload_id);
        bool OnResponseClickSubWindows(LPVOID lpData);
        void LoadViewResolverPlugin();
        void OnResponcePluginVersionMsg( WPARAM& wParam,LPARAM& lParam );
        LRESULT OnResponceInputCommonWindowMsg( WPARAM& wParam,LPARAM& lParam );
        //void OnMaximizeLogLayoutBtnClick(TNotifyUI& msg);
    private:
        //!,S_MES_INFO& sMesInfo,S_PLUGIN_INFO& sPluginInfo
        HWND mhMainTestPlatformWindow;
        int mnPluginAppID;
        S_MES_INFO& msMesInfo;
        S_PLUGIN_INFO& msPluginInfo;
        std::string mstrClsid;
    private:
        //! 子窗体信息（传递给插件）
        S_SUB_WND_IMPL_PARAM			m_sSubWndInfo;
        //! 通用窗口消息（传递给插件）
        S_COMMON_WND_MSG_INFO			m_sCommonWndMsgInfo;
    private:
        JGWPrintLogType                 meLogClickLevel;
        CLabelUI* mpTitleLabel;
        COptionUI* mpLogOption;
        CHorizontalLayoutUI* mpTestInfoVerticalLayout;
        CHorizontalLayoutUI* m_pSubWndLayoutUI;
        CRichEditUI* mpLogTabedt;
        CHorizontalLayoutUI* mpLogHorizontalLayout;
    private:
        //! UI委托事件
        CCJGWUIEvent				mevent;
        Poco::NotificationCenter	mNotifiCenter;
        //! 视图加载器类指针
        CJGWDuiiLlibSubWndImpl*		m_pViewResolverClass;
        CJGWLoadSubPlugin<CJGWDuiiLlibSubWndImpl> m_cLoadSubPlugin;
    };
}
