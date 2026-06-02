#pragma once
#include <UiLib/CUiLibWnd.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include <JaGuarWaveTestPlatform/CJGW_PocoClassLoader.h>
#include <JaGuarWaveTestPlatform/JGWLoadSubPlugin.h>
#include <poco/NotificationCenter.h>
#include <JGW_DevicePool/DevicePortDefine.h>
#include "../../../platform_include/JGW_DevicePoolModule_Impl.h"

#include "JGWViewLayoutConfig.h"
#include "JGWUIEvent.h"

namespace JGW
{
    class CCJGWMainDlg:public CCUiLibWnd
    {
    public:
        CCJGWMainDlg(S_MES_INFO& sMesInfo,S_JGW_HELP_CONFIG& sHelpConfig);
        ~CCJGWMainDlg(void);
    private: //! virtual
        void OnNotify(TNotifyUI& msg);

        UILIB_GetWindowClassName(CCJGWMainDlg);

        void OnInitWindow();

        void OnCloseWindow();

        virtual void OnFinalMessage(HWND hWnd);

        DUI_DECLARE_MESSAGE_MAP_B()
    private:
        //! 初始化UI控件相关信息
        bool InitControlsUIClassPtr();
#if 0
        //! 初始化MES配置文件
        bool InitMESConfig();
        //! 开启MES服务
        void StartMESService();
        //! 加载MES订单信息
        void LoadMESTaskInfo();
#endif
        //! 检查当前账号登录是否已经过期
        void CheckLoginIsExpired();
        //! 响应Log对话框显示按钮
        void OnSelectChanged(TNotifyUI& msg);
        //! 响应log点击按钮
        void OnRspLogBtnClick(TNotifyUI& msg);
        //! 响应System 菜单栏
        void OnSytemMenuClick(TNotifyUI& msg);
        //! 最大化LOG窗口
        void OnMaximizeLogLayoutBtnClick(TNotifyUI& msg);
        //! 响应VIEW 菜单栏
        void OnViewMenuClick(TNotifyUI& msg);
        //! 响应Setting 菜单栏
        void OnSettingMenuClick(TNotifyUI& msg);
        //! 响应Tool菜单栏
        void OnToolMenuClick(TNotifyUI& msg);
        //!
        void OnShowProjectMenu(CControlUI* pSend);
        //!
        void OnMenuElementClick(TEventUI* pTEventUI);  
        //! 
        void OnRspHelpMsg(int msg);
    private:
        //! 更新站点显示信息
        void OnUpdateMESUiInfo(WPARAM wParam, LPARAM lParam);
        //! 响应加载子窗体消息
        bool OnResponseClickSubWindows(LPVOID lpData);
        //! 响应定时器消息
        void OnResponceTimeMsg( WPARAM wParam, LPARAM lParam );
        //! 显示或者隐藏log试图
        void OnResponceViewLogInfoMsg(WPARAM wParam, LPARAM lParam);
        //! 更新时间显示
        void OnUpdateTime();
        //! 加载视图加载框架
        void LoadViewResolverPlugin();
        //! 
        CJGWDuiiLlibSubWndImpl* GetDuiLibSubWndImplPtr(int viewload_id);
        //! return false 不继续下发， true 继续下发消息 默认返回true
        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
    private:
        //! 关闭子窗体
        void CloseSubPluginView();
        //! 卸载子插件
        void DestroySubPlugin();
    private:
        void AddMenuElementToViewLayoutConfig(CMenuWnd* pMenu,const std::wstring& strMenuName);

        void AddMenuElementToProjectNames(CMenuWnd* pMenu);
    private://! device pool
        void				StartDevicePool();
        void				OnDeviceChange( WPARAM wParam,LPARAM lParam );
        SDeviceMsg_Info*	GetDeviceMsgInfoPtr();
        //! 响应设备消息
        void				OnResponceDeviceMsg(WPARAM& wParam,LPARAM& lParam);
        //! 响应插件版本号修改
        void				OnResponcePluginVersionMsg( WPARAM& wParam,LPARAM& lParam );
        //!
        LRESULT				OnResponceInputCommonWindowMsg( WPARAM& wParam,LPARAM& lParam );
        //! 已经使用过得DEVICE MSG消息
        std::vector<SDeviceMsg_Info*> m_vSDeviceMsg;
    private:
        //!
        bool mbOpenHelpTipDlg;
        //! 
        int                             mnOldProjectIndex;  
        //! 工具标题
        CLabelUI*                       mpToolTitleLabel;
        //! 测试工站
        CEditUI*                        mpOperationIDEdt;
        //! 工单号 
        CEditUI*                        mpWrokOrederNumberEdt;
        //! 班次
        CEditUI*						mpshiftEdt;
        //! 用户名称
        CEditUI*						mpUserNameEdt;
        //! 插件版本文本框
        CLabelUI*						m_pPluginVersionLabel;
        //! 提示文本框，显示设备连接与断开消息
        CLabelUI*						m_pTipLabel;
        //! 项目名称
        CLabelUI*                       m_pProjectNameLabel;
        //! MES 信息框
        CLabelUI*                       m_pMesInfoLabel;
        //! 时间显示文本框
        CLabelUI*						m_pTimerLabel;
        //!
        CLabelUI*                       m_pAutoUpdateServiceStatusLabel;
        //! 测试项目，以及测试插件视图
        CUIVerticalLayoutUIProjectBtn*	m_pPrejectVerticalLayoutUI;
        //! 子窗体视图（插件视图布局）
        CHorizontalLayoutUI*			m_pSubWndLayoutUI;
        //! Log插件试图布局
        CHorizontalLayoutUI*            mpLogHorizontalLayout;
        //!
        CHorizontalLayoutUI*            mpStatusBarLayout;
        //!
        CVerticalLayoutUI*              mpLeftFrameVerticalLayout; 

        CVerticalLayoutUI*              mpTestInfoVerticalLayout;
#if 0
        //! log显示
        CButtonUI*                      mpLogButton;
        /* Log Tab */

        COptionUI*                      mpLogDebugTab;
        COptionUI*                      mpLogErrorTab;
        COptionUI*                      mpLogInfoTab;
        COptionUI*                      mpLogWaraningTab;

        //! 开始测试按钮
        CButtonUI*                      mpStartTestBtn;
#endif
        JGWPrintLogType                 meLogClickLevel;
        CRichEditUI*                    mpLogTabedt;
        //!
        /* Log Tab */
        //! 点击测试插件发送控件
        CControlUI*						m_pClickPSender;
        //! 视图布局配置文件
        CCJGWViewLayoutConfig			mcZQTViewLayoutConfig;
        //! 
        std::string                     mstrClsid;
        //!
        std::wstring                    mstrClickMenuName;
        //! 子窗体信息（传递给插件）
        S_SUB_WND_IMPL_PARAM			m_sSubWndInfo;
        //! 通用窗口消息（传递给插件）
        S_COMMON_WND_MSG_INFO			m_sCommonWndMsgInfo;
        //!
        S_MES_INFO&                      msMesInfo;
        //!
        S_JGW_HELP_CONFIG& msHelpConfig;
        //! 视图加载器类指针
        CJGWDuiiLlibSubWndImpl*		m_pViewResolverClass;
        CJGWLoadSubPlugin<CJGWDuiiLlibSubWndImpl> m_cLoadSubPlugin;
        //! 线程池
        CCJGW_DevicePoolImpl*		    m_lpDevicePoolImpl;
        CCJGW_PocoClassLoader<CCJGW_DevicePoolImpl> m_cLoadDevicePoolSubPlugin;
        //! 菜单栏点击状态
        map<CDuiString,bool> mpMenuCheckInfo;
    private:
        //! UI委托事件
        CCJGWUIEvent				mevent;
        Poco::NotificationCenter	mNotifiCenter;
    };
}