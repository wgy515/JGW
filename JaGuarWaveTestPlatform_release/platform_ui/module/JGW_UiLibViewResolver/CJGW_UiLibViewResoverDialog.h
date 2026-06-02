#pragma once

#include <JaGuarWaveTestPlatform/JGWDuiiLlibSubWndImpl.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include <UiLib/CDuiDelegateDlg.h>
#include <JaGuarWaveTestPlatform/JGWLoadSubPlugin.h>
#include "../../../platform_include/CJGWUiLibViewResolverImpl.h"
#include <UiLib/JGW_AsyncWndMessage.h>

namespace JGW
{
    class CCJGWUiLibViewResoverDialog :
        public CJGWDuiiLlibSubWndImpl,CDuiDelegateDlg
    {
    public:
        CCJGWUiLibViewResoverDialog(void);
        ~CCJGWUiLibViewResoverDialog(void);
        /*		CCJGWDuiiLlibSubWndImpl	*/
    public:
        virtual bool CreateSubWnd( LPS_SUB_WND_IMPL_PARAM psSubWndImplParam );
        //! 绘画
        virtual void OnPaint(HDC hDC, const RECT* rcPaint);
        // 	//! 响应事件消息
        virtual void OnMsgNotify(LPVOID pNotifyMsg);

        virtual void CloseSubWndPlugin();

        virtual void OnParentCustomMessage(const UINT& uMsg,const WPARAM& wParam,const LPARAM& lParam);

        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);

        virtual void OnCloseWindow();
        /**************CDuiDelegateDlg***********************/
    private:
        LPCTSTR		GetWindowClassName() const;

        bool		InitWindow();

        void		Notify(TNotifyUI& msg);

    private:
        /*

        |-ZQ-Test-Application--------------------------------------------------------------------|
        |																						 |
        |-Menu-----------------------------------------------------------------------------------|
        |																						 |
        |-project---|-version--------------------------------------------------------------------|
        |           |-|--ParentContainer(subWndHorizontalLayout)---------------------------------|
        |           | |--|EmptySubContainer------------------------------------------------------|
        |           | |  |                                                                       |                                                                   
        |           | |  |                                                                       |
        |           | |  |                                                                       |
        |           | |  |                                                                       |
        |           | |  |                                                                       |
        |           | |  |                   plugin                                              |
        |           | |  |                                                                       |
        |           | |  |                                                                       |
        |           | |  |                                                                       |
        |           | |  |                                                                       |
        |           | |  |                                                                       |
        |           | |  |                                                                       |
        |           | |  |                                                                       |
        |           | |  |                                                                       |
        |           | |  |                                                                       |
        |___________|_|__|_______________________________________________________________________|
        |																						 |
        |							toolbar														 |
        |________________________________________________________________________________________|

        */
        //! 创建DuiLib子窗体
        bool		CreateDuiLibSubWnd();

        void		ReSizeHwnd();
    private:
        void        ExecuteFIFOEventMessage();
    private:
        //! 框架父容器
        CHorizontalLayoutUI*						mpParentContainer;

        CHorizontalLayoutUI*						mpEmptySubContainer;

        CControlUI*									mpSubContainer;
        CPaintManagerUI*							mpParentPaintUi;

    private:
        CCJGWUiLibViewResolverImpl*					mpZQUiLibViewResolverImpl;
        LPS_PLUGIN_INFO								mpPluginInfo;
        LPS_SUB_WND_IMPL_PARAM						mpsSubWndImplParam;
        CJGWLoadSubPlugin<CCJGWUiLibViewResolverImpl>	mZQLoadSubPlugin;
        _map_reg_msg								mMapRegMsg;
        CCJGW_AsyncWndMessage                   mAsyncWndMessage;
        //! CHorizontalLayoutUI*						mpMainParentContainer;
    };
}

