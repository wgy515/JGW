#pragma once
#include "resource.h"
#include <UiLib/CAsyncMsgDialog.h>
#include "CJGW_WlanServiceThread.h"
namespace JGW
{
	class CCJGW_WlanServiceApplicationDlg : public CCAsyncMsgDialog
	{
	public:
		CCJGW_WlanServiceApplicationDlg(void);
		~CCJGW_WlanServiceApplicationDlg(void);
	private:
		//! 初始化窗口
		void OnInitWindow();
		//! 
		UILIB_GetWindowClassName(CCJGW_WlanServiceApplicationDlg);
		//! 通知
		void OnNotify(TNotifyUI& msg);
		//! 关闭窗口
		void OnCloseWindow();
	private:
		//! 响应事件消息 默认返回TRUE
		bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
		//!
		void OnMenuElementClick(TEventUI* pTEventUI);
		//!
		void OnShowTrayMenuWnd(TEventUI* pTEventUI);
		//!
		void OnAppendRichEditTest(WPARAM wParam,LPARAM lParam);
	private:
		bool mbShow;
		CRichEditUI* mpLogRickEdt;
		CCJGW_WlanServiceThread* mpWlanServiceThread;
		std::map<CDuiString,bool> m_MenuCheckInfo;
		CDuiTrayIcon mcTrayIco;
	};
}