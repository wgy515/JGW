#include "StdAfx.h"
#include "CJaGuarWaveHelpDlgV3.h"
#include "Resource.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_CryptPlugin/CJGW_CryptPassword.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"


namespace JGW
{
    CCJaGuarWaveHelpDlgV3::CCJaGuarWaveHelpDlgV3(S_HELP_V3_CONFIG& sHelpV3Config) : msHelpV3Config(sHelpV3Config),mbShow(false),mpHelpServiceThread(NULL),mpSoftwareDownloadThread(NULL),mpFTPUploadingTestLogsThread(NULL)
    {
    }


    CCJaGuarWaveHelpDlgV3::~CCJaGuarWaveHelpDlgV3(void)
    {
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
    }

    void CCJaGuarWaveHelpDlgV3::OnInitWindow()
    {
        mpLogRickEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"logEdt"));
        m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJaGuarWaveHelpDlgV3::OnEventCustomMessage,0);
        mcTrayIco.CreateTrayIcon(m_PaintManager.GetPaintWindow(),IDI_SMALL,L"JaGuarWave Help Service Application V3");

        {
            gMainHwnd = m_hWnd;
        }
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
    }

    void CCJaGuarWaveHelpDlgV3::OnCloseWindow()
    {
        //! 检查当前FTP上传线程是否正在上传测试LOG
        if (!mpFTPUploadingTestLogsThread->IsUploadingTestLogsThread())
        {
            OnTimerMsg(WM_TIMER_AUTO_UPLOAD_HELP);
            Sleep(3 * 1000);
        }
        while (mpFTPUploadingTestLogsThread->IsUploadingTestLogsThread()) Sleep(10);


        KillTimer(m_hWnd,WM_TIMER_AUTO_CHECK_SOFTWARE_UPDATE_HELP);
        KillTimer(m_hWnd,WM_TIMER_AUTO_UPLOAD_HELP);
        if (mcTrayIco.Enabled()) mcTrayIco.DeleteTrayIcon();
        m_PaintManager.GetEventSource() -= MakeDelegate(this,&CCJaGuarWaveHelpDlgV3::OnEventCustomMessage,0);
        if (mpHelpServiceThread) mpHelpServiceThread->StopUpdateServiceThread();   
        //Sleep(1000);
    }

    void CCJaGuarWaveHelpDlgV3::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender->GetName() == L"minTraybtn")
            {
                mbShow = false;
                ShowWindow(SW_HIDE);
                mcTrayIco.CreateTrayIcon(m_PaintManager.GetPaintWindow(),IDI_SMALL,L"JaGuarWave Help Service Application");
            }
        }
    }

    bool CCJaGuarWaveHelpDlgV3::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    {
        if (WM_NCLBUTTONDBLCLK == pTEventUI->Type) return false;
        switch (pTEventUI->Type)
        {
        case WM_TIMER:
            OnTimerMsg(pTEventUI->wParam);
            break;
        case WM_MENUCLICK:
            OnMenuElementClick(pTEventUI);
            break;
        case UIEVENT_TRAYICON:
            OnShowTrayMenuWnd(pTEventUI);
            break;
        case WM_HWND_RICHEDIT_APPEND_TEST_MSG:
            OnAppendRichEditTest(pTEventUI->wParam,NULL);
            break;
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
        default:
            break;
        }
        return true;
    }

    void CCJaGuarWaveHelpDlgV3::OnAppendRichEditTest(WPARAM wParam,LPARAM lParam)
    {
        static int line = 0;
        mpLogRickEdt->AppendText((const wchar_t*)wParam);
        mpLogRickEdt->AppendText(L"\r\n");
        if (line > 3000) {mpLogRickEdt->Clear();line = 0;}
        line ++;
        mpLogRickEdt->EndDown();
    }

    void CCJaGuarWaveHelpDlgV3::OnMenuElementClick(TEventUI* pTEventUI)
    {
        const wchar_t* strName = (const wchar_t*)pTEventUI->wParam;
        if (NULL == JGW_WStrComparenoCaseWStr(strName,_T("exitApplicationMenu")))
        {
            Close(IDCANCEL);
        }
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
    }

    void CCJaGuarWaveHelpDlgV3::OnShowTrayMenuWnd(TEventUI* pTEventUI)
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

    void CCJaGuarWaveHelpDlgV3::OnTimerMsg(int id)
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

}
