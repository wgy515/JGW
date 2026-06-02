#pragma once
#include <JaGuarWaveTestPlatform/JGWDuiiLlibSubWndImpl.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include <UiLib/CDuiDelegateDlg.h>
#include <JaGuarWaveTestPlatform/JGWLoadSubPlugin.h>
#include "../../../platform_include/CJGWUiLibViewResolverImpl.h"
#include <UiLib/JGW_AsyncWndMessage.h>
namespace JGW
{
    class CCJGWUiLibPluginViewResolver:public CJGWDuiiLlibSubWndImpl
    {
    public:
        CCJGWUiLibPluginViewResolver(void);
        ~CCJGWUiLibPluginViewResolver(void);

        //! 创建试图加载器子窗体
        bool CreateSubWnd( LPS_SUB_WND_IMPL_PARAM psSubWndImplParam );
        //! 响应消息通知事件
        void OnMsgNotify(LPVOID pNotifyMsg);
        //! 响应custome msg消息
        void OnParentCustomMessage( const UINT& uMsg,const WPARAM& wParam,const LPARAM& lParam );
    private:
        //! 创建DuiLib子窗体
        bool CreateDuiLibSubWnd( LPS_SUB_WND_IMPL_PARAM psSubWndImplParam );
    private:
        void CloseSubWndPlugin();
    private:
        void        ExecuteFIFOEventMessage();
    private:
        HWND m_hWnd;
        CContainerUI*								mpParentContainer;
        CPaintManagerUI*							mpPaintManager;
        CControlUI*									mpSubContainer;
        LPS_PLUGIN_INFO								mpPluginInfo;
        CCJGWUiLibViewResolverImpl*					mpZQUiLibViewResolverImpl;
    private:
        CJGWLoadSubPlugin<CCJGWUiLibViewResolverImpl>	mZQLoadSubPlugin;
        _map_reg_msg									mMapRegMsg;
        CCJGW_AsyncWndMessage                       mAsyncWndMessage;
    };
}

