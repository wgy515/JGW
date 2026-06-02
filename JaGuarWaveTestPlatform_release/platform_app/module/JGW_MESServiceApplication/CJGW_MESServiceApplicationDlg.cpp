#include "StdAfx.h"
#include "CJGW_MESServiceApplicationDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"

namespace JGW
{
	CCJGW_MESServiceApplicationDlg::CCJGW_MESServiceApplicationDlg(void) : mbShow(false)
	{
		msMESSericeInfo.mstrMESServiceIP = "127.0.0.1";
		msMESSericeInfo.mstrMESServiceSocketPort = "3000";
	}


	CCJGW_MESServiceApplicationDlg::~CCJGW_MESServiceApplicationDlg(void)
	{
#if 0
		if (mpSoftwareDownloadThread)
		{
			mpSoftwareDownloadThread->ExitThread(false);
			Sleep(1000);
			mpSoftwareDownloadThread->TerminateMessageThread();
			delete mpSoftwareDownloadThread;
			mpSoftwareDownloadThread = NULL;
		}

		if (mpHelpServiceThread)
		{
			mpHelpServiceThread->ExitThread(false);
			Sleep(1000);
			mpHelpServiceThread->TerminateMessageThread();
			delete mpHelpServiceThread;
			mpHelpServiceThread = NULL;
		}

		if (mpFTPUploadingTestLogsThread)
		{
			mpFTPUploadingTestLogsThread->ExitThread(false);
			Sleep(1000);
			mpFTPUploadingTestLogsThread->TerminateMessageThread();
			delete mpFTPUploadingTestLogsThread;
			mpFTPUploadingTestLogsThread = NULL;
		}
#endif
	}

	void CCJGW_MESServiceApplicationDlg::OnInitWindow()
	{
		mpMesWebLogRichEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"MesWebLogRichEdt"));
		m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJGW_MESServiceApplicationDlg::OnEventCustomMessage,0);
		// 创建托盘程序
		mcTrayIco.CreateTrayIcon(m_PaintManager.GetPaintWindow(),IDI_SMALL,L"JaGuarWave Help Service Application V3");

		if (!mcMesTcpServerManager.StartMesTcpServerManager(&mAsyncWndMessage,&msMESSericeInfo))
		{
			MessageBox(m_hWnd,mcMesTcpServerManager.GetErrorMessage().c_str(),L"开启MES TCP服务失败",MB_ICONERROR);
			Close(IDCANCEL);
			return ;
		}
#if 0
		//! thread
		{
			try
			{
				mpHelpServiceThread = new CCJGW_UpdateServiceThread(msHelpV3Config);
			}
			catch (boost::system::system_error& se)
			{
				std::string strTemp = se.what();
				MessageBoxA(m_hWnd,strTemp.c_str(),"开启自动更新服务失败",MB_ICONERROR);
				Close(IDCANCEL);
				return;
			}
			mpHelpServiceThread->CreateMessageThread(0,0,true);
			mpHelpServiceThread->PostThreadMessage(WM_THREAD_TEST_START_MSG,WPARAM(&mAsyncWndMessage),LPARAM(m_hWnd)); 

			mpSoftwareDownloadThread = new CCJGW_SoftwareDownloadThreadV2(msHelpV3Config);
			mpSoftwareDownloadThread->CreateMessageThread(0,0,true);
			//! mpSoftwareDownloadThread->PostThreadMessage(WM_THREAD_HELP_REQ_DOWNLOAD_MSG);

			mpFTPUploadingTestLogsThread = new CCJGW_FTPUploadingTestLogsThread(msHelpV3Config);
			mpFTPUploadingTestLogsThread->CreateMessageThread(0,0,true);
			mpFTPUploadingTestLogsThread->PostThreadMessageW(WM_THREAD_START_UPDALODING_TEST_THREAD);

			//! 是否已经打开上传定时器，如果没有打开则直接打开
			if (!msHelpV3Config.mbIsOpenUploadTimer) 
			{
				SetTimer(m_hWnd,WM_TIMER_AUTO_UPLOAD_HELP,/* msHelpV3Config.mnUploadIntervalTimeSec * */1000,NULL);
				msHelpV3Config.mbIsOpenUploadTimer = true;
			}
		} 
#endif
	}

	void CCJGW_MESServiceApplicationDlg::OnCloseWindow()
	{
#if 0
		//! 检查当前FTP上传线程是否正在上传测试LOG
		if (!mpFTPUploadingTestLogsThread->IsUploadingTestLogsThread())
		{
			OnTimerMsg(WM_TIMER_AUTO_UPLOAD_HELP);
			Sleep(3 * 1000);
		}
		while (mpFTPUploadingTestLogsThread->IsUploadingTestLogsThread()) Sleep(10);


		KillTimer(m_hWnd,WM_TIMER_AUTO_CHECK_SOFTWARE_UPDATE_HELP);
		KillTimer(m_hWnd,WM_TIMER_AUTO_UPLOAD_HELP);
#endif
		if (mcTrayIco.Enabled()) mcTrayIco.DeleteTrayIcon();
		mcMesTcpServerManager.StopMesTcpServerManager();
		m_PaintManager.GetEventSource() -= MakeDelegate(this,&CCJGW_MESServiceApplicationDlg::OnEventCustomMessage,0);
#if 0
		if (mpHelpServiceThread) mpHelpServiceThread->StopUpdateServiceThread();   
		//Sleep(1000);
#endif
	}

	void CCJGW_MESServiceApplicationDlg::OnNotify(TNotifyUI& msg)
	{
		if (msg.sType == DUI_MSGTYPE_CLICK)
		{
			if (msg.pSender->GetName() == L"minTraybtn")
			{
				mbShow = false;
				ShowWindow(SW_HIDE);
				mcTrayIco.CreateTrayIcon(m_PaintManager.GetPaintWindow(),IDI_SMALL,L"JaGuarWave MES WEB Service Application");
			}
		}
	}

	bool CCJGW_MESServiceApplicationDlg::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
	{
		if (WM_NCLBUTTONDBLCLK == pTEventUI->Type) return false;
		switch (pTEventUI->Type)
		{
#if 0
		case WM_TIMER:
			OnTimerMsg(pTEventUI->wParam);
			break;
#endif
		case WM_MENUCLICK:
			OnMenuElementClick(pTEventUI);
			break;
		case UIEVENT_TRAYICON:
			OnShowTrayMenuWnd(pTEventUI);
			break;
		case WM_HWND_RICHEDIT_APPEND_TEST_MSG:
			OnAppendRichEditTest(pTEventUI->wParam,NULL);
			break;
#if 0
		case WM_HWND_HELP_REQ_DOWNLOAD_MSG:
			KillTimer(m_hWnd,WM_TIMER_AUTO_CHECK_SOFTWARE_UPDATE_HELP);
			if (!mpSoftwareDownloadThread->IsThreadRun()) mpSoftwareDownloadThread->CreateMessageThread(0,0,true);
			mpSoftwareDownloadThread->PostThreadMessage(WM_THREAD_HELP_REQ_DOWNLOAD_MSG);
			//! 是否已经打开上传定时器，如果没有打开则直接打开
			if (!msHelpV3Config.mbIsOpenUploadTimer) 
			{
				SetTimer(m_hWnd,WM_TIMER_AUTO_UPLOAD_HELP,/*msHelpV3Config.mnUploadIntervalTimeSec **/ 1000,NULL);
				msHelpV3Config.mbIsOpenUploadTimer = true;
			}
			break;
		case WM_HWND_OPEN_DOWNLOAD_HELP_TIMER_MSG:
			SetTimer(m_hWnd,WM_TIMER_AUTO_CHECK_SOFTWARE_UPDATE_HELP,msHelpV3Config.mnCheckSoftwareUpdateIntervalTimeSec * 1000,NULL);
			break;
		case WM_HWND_OPEN_UPLOADLOG_HELP_TIMER_MSG:
			SetTimer(m_hWnd,WM_TIMER_AUTO_UPLOAD_HELP,/*msHelpV3Config.mnUploadIntervalTimeSec **/ 1000,NULL);
			break;
#endif
		default:
			break;
		}
		return true;
	}

	void CCJGW_MESServiceApplicationDlg::OnAppendRichEditTest(WPARAM wParam,LPARAM lParam)
	{
		static int line = 0;
		mpMesWebLogRichEdt->AppendText((const wchar_t*)wParam);
		mpMesWebLogRichEdt->AppendText(L"\r\n");
		if (line > 3000) {mpMesWebLogRichEdt->Clear();line = 0;}
		line ++;
		mpMesWebLogRichEdt->EndDown();
	}

	void CCJGW_MESServiceApplicationDlg::OnMenuElementClick(TEventUI* pTEventUI)
	{
		const wchar_t* strName = (const wchar_t*)pTEventUI->wParam;
		if (NULL == JGW_WStrComparenoCaseWStr(strName,_T("exitApplicationMenu")))
		{
			Close(IDCANCEL);
		}
#if 0
		else if (NULL == JGW_WStrComparenoCaseWStr(strName,_T("CheckToolHelpMenu")))
		{
			if (!msHelpV3Config.msDownloadThreadParam.mstrExecFolder.empty())
			{
				OnTimerMsg(WM_TIMER_AUTO_CHECK_SOFTWARE_UPDATE_HELP);
			}
		}
		else if (NULL == JGW_WStrComparenoCaseWStr(strName,_T("UploadNowMenu")))
		{
			if (!msHelpV3Config.msDownloadThreadParam.mstrExecFolder.empty())
			{
				KillTimer(m_hWnd,WM_TIMER_AUTO_UPLOAD_HELP);
				mpFTPUploadingTestLogsThread->PostThreadMessage(WM_THREAD_UPLODING_TEST_LOG_LOOP);
			}
		}
#endif
	}

	void CCJGW_MESServiceApplicationDlg::OnShowTrayMenuWnd(TEventUI* pTEventUI)
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
#if 0
	void CCJGW_MESServiceApplicationDlg::OnTimerMsg(int id)
	{
		switch (id)
		{
		case WM_TIMER_AUTO_CHECK_SOFTWARE_UPDATE_HELP:
			KillTimer(m_hWnd,WM_TIMER_AUTO_CHECK_SOFTWARE_UPDATE_HELP);
			mpSoftwareDownloadThread->PostThreadMessage(WM_THREAD_HELP_REQ_DOWNLOAD_MSG);
			break;
		case WM_TIMER_AUTO_UPLOAD_HELP:
			KillTimer(m_hWnd,WM_TIMER_AUTO_UPLOAD_HELP);
			mpFTPUploadingTestLogsThread->PostThreadMessage(WM_THREAD_HELP_UPLOAD_LOG_MSG);
			break;
		}
	}
#endif

	void CCJGW_MESServiceApplicationDlg::LoadMesDataConfig()
	{
		std::wstring strMESConfigFilePath(JGW_GetApplicationFolder());
		strMESConfigFilePath += L"jgw_mes_config.data";

		CCJGW_ConfigIni cConfigIni;
		cConfigIni.InitIniFilePath(strMESConfigFilePath.c_str());

		msMESSericeInfo.mstrMESServiceIP = cConfigIni.GetIniKeyValueA(L"MESClient",L"MESServiceIP",L"127.0.0.1");
		msMESSericeInfo.mstrMESServiceSocketPort = cConfigIni.GetIniKeyValueA(L"MESClient",L"MESServiceSocketPort",L"3000");
	}
}
