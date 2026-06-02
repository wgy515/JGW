#pragma once
#include <JGW_PluginViewLib/CJGW_CommanTestPlnView.h>
#include <JGW_FoundationFunc/CJGW_HiPerfTimer.h>
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <vector>

namespace JGW
{
	class CCSharpTSEAdapterPluginDlg : public CCJGW_CommanTestPlnView
	{
	public:
		CCSharpTSEAdapterPluginDlg(void);
		~CCSharpTSEAdapterPluginDlg(void);
	private://! CCJGW_TestStatusListenerImp
		bool OnIdelTestStatus();
		bool OnBusyTestStatus();
		bool OnPassTestStatus();
		bool OnFailTestStatus();
	private:
		//! 响应点击config按钮消息
		void OnConfigClickMsg(const WPARAM& wParam,const LPARAM& lParam);
		//! 响应TSE初始化失败消息
		void OnTSEInitFail(const WPARAM& wParam,const LPARAM& lParam);
		//! 重新更新窗口大小
		void OnShowLogLayoutClickMsg(const WPARAM& wParam,const LPARAM& lParam);
		//! 窗体关闭消息
		void CloseSubWndPlugin();
	private:
		void OnTSEAdapterLog(const WPARAM& wParam,const LPARAM& lParam);
		//! 
		void OnCsharpAdapterViewReportHwnd(const WPARAM& wParam,const LPARAM& lParam);
		//! 
		void ReSizeCSharpAdapterHwnd();
		//! 
		void OnPaint(const WPARAM& wParam,const LPARAM& lParam);
        //! 
        void AppendRicheditLog(std::wstring& strLog);
	private:
		void UpdateMESInfoToCSharpAdapterHwnd();
	private:
		bool StartCSharpTSEAdapterExec();
	private:
		CRichEditUI* mpThroughtputTestEdt;
		CHorizontalLayoutUI* mpEmptySubLayout;
		RECT mCSharpAdapterHwndrect;
		HWND mhCSharpAdapterExecWindowHandle;
        HINSTANCE mhTSEAdapterHInstance;
	};

}

