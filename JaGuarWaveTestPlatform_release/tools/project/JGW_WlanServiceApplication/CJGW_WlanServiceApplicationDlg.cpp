#include "StdAfx.h"
#include "CJGW_WlanServiceApplicationDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"
namespace JGW
{
	CCJGW_WlanServiceApplicationDlg::CCJGW_WlanServiceApplicationDlg(void) : mbShow(false),mpWlanServiceThread(NULL)
	{
        Log4WI(L"Start CCJGW_WlanServiceApplicationDlg");
	}


	CCJGW_WlanServiceApplicationDlg::~CCJGW_WlanServiceApplicationDlg(void)
	{
	}

	void CCJGW_WlanServiceApplicationDlg::OnInitWindow()
	{
		mpLogRickEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"logEdt"));
		m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJGW_WlanServiceApplicationDlg::OnEventCustomMessage,0);
		mcTrayIco.CreateTrayIcon(m_PaintManager.GetPaintWindow(),IDI_SMALL,L"Wlan Service Application");

        try
        {
            mpWlanServiceThread = new CCJGW_WlanServiceThread;
        }
        catch (boost::system::system_error& se)
        {
            std::string strTemp = se.what();
            MessageBoxA(m_hWnd,strTemp.c_str(),"开启WLAN Service失败",MB_ICONERROR);
            Close(IDCANCEL);
            return;
        }
        
		mpWlanServiceThread->CreateMessageThread(0,0,true);
		mpWlanServiceThread->PostThreadMessage(WM_THREAD_TEST_START_MSG,WPARAM(&mAsyncWndMessage),LPARAM(m_hWnd));
	}

    void CCJGW_WlanServiceApplicationDlg::OnCloseWindow()
    {
        mcTrayIco.DeleteTrayIcon();
        m_PaintManager.GetEventSource() -= MakeDelegate(this,&CCJGW_WlanServiceApplicationDlg::OnEventCustomMessage,0);
        if (mpWlanServiceThread)
        {
            mpWlanServiceThread->StopClientThread();
            mpWlanServiceThread->TerminateMessageThread();
            delete mpWlanServiceThread;
            mpWlanServiceThread = NULL;
        }
    }

	void CCJGW_WlanServiceApplicationDlg::OnNotify(TNotifyUI& msg)
	{
		if (msg.sType == DUI_MSGTYPE_CLICK)
		{
			if (msg.pSender->GetName() == L"minTraybtn")
			{
				mbShow = false;
				ShowWindow(SW_HIDE);
				mcTrayIco.CreateTrayIcon(m_PaintManager.GetPaintWindow(),IDI_SMALL,L"Wlan Service Application");
			}
		}
	}

	bool CCJGW_WlanServiceApplicationDlg::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
	{
		if (WM_NCLBUTTONDBLCLK == pTEventUI->Type) return false;
		switch (pTEventUI->Type)
		{
		case WM_MENUCLICK:
			OnMenuElementClick(pTEventUI);
			break;
		case UIEVENT_TRAYICON:
			OnShowTrayMenuWnd(pTEventUI);
			break;
		case WM_HWND_RICHEDIT_APPEND_TEST_MSG:
			OnAppendRichEditTest(pTEventUI->wParam,NULL);
			break;
		default:
			break;
		}
		return true;
	}

	void CCJGW_WlanServiceApplicationDlg::OnAppendRichEditTest(WPARAM wParam,LPARAM lParam)
	{
		static int line = 0;
		mpLogRickEdt->AppendText((const wchar_t*)wParam);
		Log4WI((const wchar_t*)wParam);
		mpLogRickEdt->AppendText(L"\r\n");
		if (line > 500) {mpLogRickEdt->Clear();line = 0;}
		line ++;
		mpLogRickEdt->EndDown();
	}

	void CCJGW_WlanServiceApplicationDlg::OnMenuElementClick(TEventUI* pTEventUI)
	{
		const wchar_t* strName = (const wchar_t*)pTEventUI->wParam;
		if (NULL == JGW_WStrComparenoCaseWStr(strName,_T("exitApplicationMenu")))
		{
			Close(IDCANCEL);
		}
	}

	void CCJGW_WlanServiceApplicationDlg::OnShowTrayMenuWnd(TEventUI* pTEventUI)
	{
		if (WM_RBUTTONUP == pTEventUI->lParam)
		{
			::SetForegroundWindow(this->m_hWnd);

			CMenuWnd* pMenu = new CMenuWnd();
			CPoint point(0,0);
			GetCursorPos(&point);
			pMenu->Init(NULL, _T("tray_menu.xml"), point, &m_PaintManager, &m_MenuCheckInfo,eMenuAlignment_Left | eMenuAlignment_Bottom );

			::PostMessage(m_hWnd,WM_NULL,0,0);
		}  
		else if(WM_LBUTTONDOWN == pTEventUI->lParam)
		{             
			mcTrayIco.DeleteTrayIcon();
			mbShow = true;
			ShowWindow(SW_SHOWNORMAL);//显示主窗口	
		}
	}
}