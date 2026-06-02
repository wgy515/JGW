#pragma once
#include <UiLib/CUiLibWnd.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include <JaGuarWaveTestPlatform/CJGW_PocoClassLoader.h>
#include <JaGuarWaveTestPlatform/JGWLoadSubPlugin.h>
#include <poco/NotificationCenter.h>
#include <JGW_DevicePool/DevicePortDefine.h>
#include "../../../platform_include/JGW_DevicePoolModule_Impl.h"

#include <JaGuarWaveTestPlatform/JGWViewLayoutConfig.h>
#include <JaGuarWaveTestPlatform/JGWUIEvent.h>
#include "IWPFWin32WindowInterface.h"

namespace JGW
{
    class CCJGWMainDlg_Net:public CCUiLibWnd,public IWPFWin32WindowInterface
    {
    public:
        CCJGWMainDlg_Net();
        ~CCJGWMainDlg_Net(void);
    public: //IWPFWin32WindowInterface
        //
        virtual HWND LoadWin32Window(HWND hMainWnd,DWORD dwUsbDevicePoolThreadID);
        //
        virtual void UpdateWin32WindowBackgroundColor(const wchar_t* color) ;
        //
        virtual void UpdateMESInfo(bool mesOnline,const wchar_t* taskNumber, const wchar_t* workOrderNumber, const wchar_t* userName, const wchar_t* password, int shiftID);
        //
        virtual void UpdatePluginInfo(const wchar_t* stationID,const wchar_t* clsid,int viewloader_id,const wchar_t* moduleFolder,const wchar_t* pluginName,const wchar_t* station_name,int image_id);
        //
        virtual void ShowLogControl();
        //
        virtual void ShowLogDialog();
        //
        virtual void StartRemoteLogSocket(bool bOpen);
        //
        virtual void CloseWin32Window();
        //
        virtual void UnLoadWin32Window();
    private: //! virtual
        void OnNotify(TNotifyUI& msg);

        UILIB_GetWindowClassName(CCJGWMainDlg_Net);

        void OnInitWindow();

        void OnCloseWindow();

        DUI_DECLARE_MESSAGE_MAP_B()
    private:
        bool LoadMESConfig(S_MES_INFO& msMesInfo);
        //! 初始化UI控件相关信息
        bool InitControlsUIClassPtr();
        //! 响应Log对话框显示按钮
        void OnSelectChanged(TNotifyUI& msg);
        //! 响应log点击按钮
        void OnRspLogBtnClick(TNotifyUI& msg);
        //! 最大化LOG窗口
        void OnMaximizeLogLayoutBtnClick(TNotifyUI& msg);
    private:
        //! 更新站点显示信息
        //void OnUpdateMESUiInfo(WPARAM wParam, LPARAM lParam);
        //! 响应加载子窗体消息
        bool OnResponseClickSubWindows();
        //! 响应定时器消息
        void OnResponceTimeMsg( WPARAM wParam, LPARAM lParam );
        //! 显示或者隐藏log试图
        void OnResponceViewLogInfoMsg(WPARAM wParam, LPARAM lParam);
        //! 更新时间显示
        //void OnUpdateTime();
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
        //void				StartDevicePool();
        //void				OnDeviceChange( WPARAM wParam,LPARAM lParam );
        //SDeviceMsg_Info*	GetDeviceMsgInfoPtr();
        //! 响应设备消息
        //void				OnResponceDeviceMsg(WPARAM& wParam,LPARAM& lParam);
        //! 响应插件版本号修改
        void				OnResponcePluginVersionMsg( WPARAM& wParam,LPARAM& lParam );
        //!
        LRESULT				OnResponceInputCommonWindowMsg( WPARAM& wParam,LPARAM& lParam );
        //! 已经使用过得DEVICE MSG消息
        //std::vector<SDeviceMsg_Info*> m_vSDeviceMsg;
    private:
        // WPF 窗口句柄
        HWND                            m_hParentHwnd;
        //! 插件版本文本框
        CLabelUI*						m_pPluginVersionLabel;
        //! 子窗体视图（插件视图布局）
        CHorizontalLayoutUI*			m_pSubWndLayoutUI;
        //! Log插件试图布局
        CHorizontalLayoutUI*            mpLogHorizontalLayout;
        // 测试布局
        CVerticalLayoutUI*              mpTestInfoVerticalLayout;
        //! 主Layout
        CVerticalLayoutUI*              mpMainVerticalLayoutUI;
        // log类型
        JGWPrintLogType                 meLogClickLevel;
        //
        CRichEditUI*                    mpLogTabedt;
        /* Log Tab */
        S_PLUGIN_INFO                   msPluginInfo;
        //! 视图布局配置文件
        CCJGWViewLayoutConfig			mcZQTViewLayoutConfig;
        //! 
        std::string                     mstrClsid;
        //! 背景颜色
        std::wstring                    mstrBackgroundColor;
        //! 子窗体信息（传递给插件）
        S_SUB_WND_IMPL_PARAM			m_sSubWndInfo;
        //! 通用窗口消息（传递给插件）
        S_COMMON_WND_MSG_INFO			m_sCommonWndMsgInfo;
        //!
        S_MES_INFO                      msMesInfo;
        //!
        //S_JGW_HELP_CONFIG& msHelpConfig;
        //! 视图加载器类指针
        CJGWDuiiLlibSubWndImpl*		m_pViewResolverClass;
        CJGWLoadSubPlugin<CJGWDuiiLlibSubWndImpl> m_cLoadSubPlugin;
        //! 线程池
        /*CCJGW_DevicePoolImpl*		    m_lpDevicePoolImpl;
        CCJGW_PocoClassLoader<CCJGW_DevicePoolImpl> m_cLoadDevicePoolSubPlugin;*/
    private:
        //! UI委托事件
        CCJGWUIEvent				mevent;
        Poco::NotificationCenter	mNotifiCenter;
    };
}