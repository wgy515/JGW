#pragma once
#include "JGW_PluginView_Define.h"
#include "../../../platform_include/CJGWUiLibViewResolverImpl.h"
#include <UiLib/UIlib.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include "../../../platform_include/CJGWUiLibViewResolver_Define.h"
#include <JGW_TestInterface/JGW_TestInterface.h>
#include "CJGW_TestStatusListenerImp.h"
#include "CJGW_TestStatusView.h"
namespace JGW
{
    /*
    常规测试插件试图
    1、单个测试项目
    2、有开始 PASS 直通率等等
    */
    class CCJGW_CommanTestPlnView:public CCJGWUiLibViewResolverImpl,CCJGW_TestStatusListenerImp
    {
    public:
        CCJGW_CommanTestPlnView(void);
        virtual ~CCJGW_CommanTestPlnView(void);
    protected:
        virtual bool OnRspTestStatusChange(E_TEST_STATUS_TYPE eTestStatusType);
    protected://! 继承CCJGWUiLibViewResolverImpl 接口类
        virtual bool InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam);

        virtual void OnNotifySubWndPlugin(LPVOID pMsg);

        virtual void CloseSubWndPlugin();
    protected: //! 响应主界面窗口消息
        virtual void OnTestStartClickMsg(const WPARAM& wParam,const LPARAM& lParam);

        virtual void OnTestPassMsg(const WPARAM& wParam,const LPARAM& lParam);

        virtual void OnTestFailMsg(const WPARAM& wParam,const LPARAM& lParam);

        virtual void OnConfigClickMsg(const WPARAM& wParam,const LPARAM& lParam);

        virtual void OnRspTimeMsg(const WPARAM& wParam,const LPARAM& lParam);   

        virtual void OnClearTestRecordClickMsg(const WPARAM& wParam,const LPARAM& lParam);
        //! WM_HWND_SHOW_LOG_LAYOUT_CLICK_MSG
        virtual void OnShowLogLayoutClickMsg(const WPARAM& wParam,const LPARAM& lParam);

        virtual void OnIsShowTestStatusWindowMsg(const WPARAM& wParam,const LPARAM& lParam);
		//! 是否显示测试窗口消息
		virtual void OnSaveTestLogSelectChangedMsg(const WPARAM& wParam,const LPARAM& lParam);
    protected:
        E_TEST_STATUS_TYPE meTestStatusType;
        bool                mbIsShowLogHwnd;
        bool                mbIsShowTestResultStatus; //! 是否显示测试结果状态
        bool                mbIsSaveTestLog;
        LPS_VIEW_RESOLVER_PARAM mpViewResolverParam;
        std::wstring        mstrSaveDownloadLogFolder;
        CPaintManagerUI* mpPluginPaintManagerUI;
        //CCJGW_TestStatusView mTestStatusView;
    };
}


