#pragma once
#include "../../../platform_include/CJGWUiLibViewResolverImpl.h"
#include <UiLib/UIlib.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include "../../../platform_include/CJGWUiLibViewResolver_Define.h"
#include "PluginAppWindowUI.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include "CTSEMultiPluginAppPluginDefine.h"

//1 2 4 8
namespace JGW
{
    
    class CCTSEMultiPluginAppPluginDlg : public CCJGWUiLibViewResolverImpl,public IDialogBuilderCallback
    {
    public:
        CCTSEMultiPluginAppPluginDlg(void);
        ~CCTSEMultiPluginAppPluginDlg(void);
    protected://! 继承CCJGWUiLibViewResolverImpl 接口类
        //! 初始化
        bool InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam);
        // 接收消息
        void OnNotifySubWndPlugin(LPVOID pMsg);

        void CloseSubWndPlugin();
    private:
        void OnContextMenuClick(TNotifyUI* pNotifyMsg);
        void OnMenuClick(const WPARAM& wParam,const LPARAM& lParam);
        void InitTSEMultiPluginAppPluginView();
        void UnInitTSEMultiPluginAppPluginView();
        virtual CControlUI* CreateControl(LPCTSTR pstrClass);
        void OnPluginAppHwndMsg(const WPARAM& wParam,const LPARAM& lParam);
        void OnMainHwndToPluginAppMsg(const WPARAM& wParam,const LPARAM& lParam);
    private:
        bool LoadTSEMultiPluginAppConfig();
    private:
        //! 多路测试几行
        int mnMultipleTestRow;
        //！每行多少列
        int mnMultipleTestCol;
        //! Number of multiple tests
        E_NUMBER_OF_MULTIPLE_TEST meNumberOfMultipleTests;
        //
        S_MultiPluginAppConfig msMultiPluginAppConfig;
        //! 
        CCJGW_ConfigIni mConfigIni;
    private:
        std::map<int,CPluginAppWindowUI*> mmappPluginAppWindowUI;
        LPS_VIEW_RESOLVER_PARAM mpsViewResolverParam;
        CPaintManagerUI* mpPluginPaintManagerUI;
        CVerticalLayoutUI* mpMainMultipleVerticalLayout;
        //! 菜单栏点击状态
        map<CDuiString,bool> mpMenuCheckInfo;
    };
}
