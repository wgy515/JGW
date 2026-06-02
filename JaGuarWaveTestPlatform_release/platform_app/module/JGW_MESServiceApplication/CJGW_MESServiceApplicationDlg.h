#pragma once
#include "resource.h"
#include <UiLib/CAsyncMsgDialog.h>
#include "JGW_MESServiceApplication_Define.h"
#include "CJGW_MesTcpServerManager.h"
namespace JGW
{
	class CCJGW_MESServiceApplicationDlg : public CCAsyncMsgDialog
	{
	public:
		CCJGW_MESServiceApplicationDlg(void);
		~CCJGW_MESServiceApplicationDlg(void);
	private:
		//! 初始化窗口
		void OnInitWindow();
		//! 
		UILIB_GetWindowClassName(CCJGW_MESServiceApplicationDlg);
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
		//!
		//void OnTimerMsg(int id);
	private:
		void LoadMesDataConfig();
	private:
		bool mbShow;
		CRichEditUI* mpMesWebLogRichEdt;
		S_MESSericeInfo msMESSericeInfo;
		std::map<CDuiString,bool> m_MenuCheckInfo;
		CDuiTrayIcon mcTrayIco;
		CCJGW_MesTcpServerManager mcMesTcpServerManager;
	};
}
