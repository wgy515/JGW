#pragma once
#include <JaGuarWaveTestPlatform/JGWDuiiLlibSubWndImpl.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include <UiLib/CDuiDelegateDlg.h>

namespace JGW
{
    class CCWelcomViewDlg:public CJGWDuiiLlibSubWndImpl
    {
    public:
        CCWelcomViewDlg(void);
        ~CCWelcomViewDlg(void);

        virtual bool CreateSubWnd( LPS_SUB_WND_IMPL_PARAM psSubWndImplParam );
        //! 响应事件消息
        virtual void OnMsgNotify(LPVOID pNotifyMsg);
        //! 关闭子窗体
        virtual void CloseSubWndPlugin();
    private:
        CContainerUI*								mpParentContainer;
        CPaintManagerUI*							mpPaintManeger;
        CControlUI*									mpSubContainer;
        LPS_PLUGIN_INFO								mpPluginInfo;
    };
}

