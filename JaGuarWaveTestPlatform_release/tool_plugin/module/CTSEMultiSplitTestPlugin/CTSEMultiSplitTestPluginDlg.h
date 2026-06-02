#pragma once
#include "../../../platform_include/CJGWUiLibViewResolverImpl.h"
#include <UiLib/UIlib.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include "../../../platform_include/CJGWUiLibViewResolver_Define.h"
#include "TSEMultiSplitTestPluginDefine.h"

#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>

namespace JGW
{
    class CCTSETestPluginUI;
	class CCTSEMultiSplitTestPluginDlg : public CCJGWUiLibViewResolverImpl,public IDialogBuilderCallback
	{
	public:
		CCTSEMultiSplitTestPluginDlg(void);
		~CCTSEMultiSplitTestPluginDlg(void);
	protected://! 继承CCJGWUiLibViewResolverImpl 接口类
		//! 初始化
		bool InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam);

		void OnNotifySubWndPlugin(LPVOID pMsg);

		void CloseSubWndPlugin();
	private:
		bool LoadTSEMultiPluginAppConfig();
	private:
		void OnMenuClick(const WPARAM& wParam,const LPARAM& lParam);
		void OnContextMenuClick(TNotifyUI* pNotifyMsg);
		void UnInitTSEMultiPluginAppPluginView();
		void InitTSEMultiPluginAppPluginView();
        virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	private:
		//! 多路测试几行
		int mnMultipleTestRow;
		//！每行多少列
		int mnMultipleTestCol;
		//! Number of multiple tests
		E_NUMBER_OF_MULTIPLE_TEST meNumberOfMultipleTests;
		//
		S_MultiSplitTestConfig msMultiPluginAppConfig;
		//! 
		CCJGW_ConfigIni mConfigIni;
	private:
		std::map<int,CCTSETestPluginUI*> mmappPluginAppWindowUI;
		LPS_VIEW_RESOLVER_PARAM mpsViewResolverParam;
		CPaintManagerUI* mpPluginPaintManagerUI;
		CVerticalLayoutUI* mpMainMultipleVerticalLayout;
		//! 菜单栏点击状态
		map<CDuiString,bool> mpMenuCheckInfo;
	};
}
