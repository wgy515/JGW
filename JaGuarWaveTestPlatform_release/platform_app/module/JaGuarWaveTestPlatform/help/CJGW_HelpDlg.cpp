#include "StdAfx.h"
#include "CJGW_HelpDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include "../../../../platform_include/JGW_MSG_ID_Define.h"
#include "../../../include/JaGuarWaveTestPlatformHelp/CCJGW_HelpThreadImpl.h"
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
namespace JGW
{
    CCJGW_HelpDlg::CCJGW_HelpDlg(S_JGW_HELP_CONFIG& sHelpConfig) : m_hParentHwnd(NULL),/*mUpdateClientThread(updateClientThread),*/mpHelpModuleThread(NULL),msHelpConfig(sHelpConfig)
    {
//         msHelpConfig.mnSocketTimeoutSec = 50;
//         msHelpConfig.mnUpdateServiceSocketPort = 3002;
//         msHelpConfig.mstrTaskNumber = "DHW181381";
    }


    CCJGW_HelpDlg::~CCJGW_HelpDlg(void)
    {
    }

    int CCJGW_HelpDlg::DoModal()
    {
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateNoBorderStyleWindow(_T(""),_T("help_view.xml"),_T("Automatic Updates Windows"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
        CreateNoBorderStyleWindow(_T(""),_T("help_view.xml"),_T("Automatic Updates Windows"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#else
        CreateNoBorderStyleWindow(_T("skin\\JaGuarWave"),_T("help_view.xml"),_T("Automatic Updates Windows"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#endif
        CenterWindow();

        return ShowModal();
    }

    void CCJGW_HelpDlg::OnNotify(TNotifyUI& msg)
    {

    }

    void CCJGW_HelpDlg::OnInitWindow()
    {
        mpUpdateProgressUI = static_cast<CProgressUI*>(m_PaintManager.FindControl(L"UpdateProgress"));
        mpUpdateProgressUI->SetMaxValue(60 * 10);
        //! 注册相关消息
        {
            m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJGW_HelpDlg::OnEventCustomMessage,0);
            SetTimer(m_hWnd,WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT,UPDATE_DOWNLOAD_PROGRESS_TIME,NULL);
        }
        //! 开启更新服务
        {
            mpHelpModuleThread = GetHelpModuleThread();
            mpHelpModuleThread->CreateMessageThread(0,0,true);
            mpHelpModuleThread->PostThreadMessage(WM_THREAD_TEST_START_MSG,WPARAM(&msHelpConfig),LPARAM(m_hWnd));
        }     
    }

    void CCJGW_HelpDlg::OnCloseWindow()
    {
        m_PaintManager.GetEventSource() -= MakeDelegate(this,&CCJGW_HelpDlg::OnEventCustomMessage,0);
        KillTimer(m_hWnd,WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT);
    }

    bool CCJGW_HelpDlg::OnEventCustomMessage( TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam )
    {
        UINT uMsg = pTEventUI->Type;
        switch (uMsg)
        {
        case WM_TIMER:
            if (WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT == pTEventUI->wParam && mpUpdateProgressUI) OnRspUpdateProgressTimer();
            break;
        case WM_HWND_RSP_HELP_DOWNLOAD_MSG:
            OnRspHelpDownloadMsg();
            break;
        case WM_HWND_RSP_HELP_UPDATE_MSG:
            OnRspUpdateDownloadMsg();
            break;
        case WM_HWND_RSP_HELP_CONNECT_ERROR_MSG:
            //JGW_KillProcessToName(L"JaGuarWaveHelp.exe");
            MessageBox(m_hWnd,GetString(E_HELP_SERVICE_CONNECTION_EXCEPTION),GetString(E_EXCEPTION_ERROR_TYPE),MB_ICONERROR);
            Close(IDCANCEL);
            break;
        case WM_HWND_RSP_HELP_FTP_CONNECT_ERROR_MSG:
            Close(IDOK == MessageBox(m_hWnd,GetString(E_HELP_CONNECT_FTP_ERROR),GetString(E_EXCEPTION_ERROR_TYPE),MB_OKCANCEL)?IDCANCEL:IDOK);
            break;
        case WM_HWND_RSP_HELP_ERROR_MSG:
            //JGW_KillProcessToName(L"JaGuarWaveHelp.exe");
            MessageBox(m_hWnd,(const wchar_t*)(pTEventUI->wParam),L"ERROR",MB_ICONERROR);
            Close(IDCANCEL);
            break;
        case WM_HWND_RSP_HELP_CLOSE_MSG:
            Close(IDOK);
            break;
        }

        return true;
    }

    void CCJGW_HelpDlg::OnRspHelpDownloadMsg()
    {
        MessageBox(m_hWnd,GetString(E_HELP_TOOL_UPDATE_INTSALL_VERSION_TIP),GetString(E_HELP_INSTALL_VERSION_MSGTIP),MB_ICONWARNING);
        Close(IDCANCEL);
#ifdef _DEBUG
        ShellExecute(NULL,L"open",L"C:\\JW\\Uninstall.exe",NULL,L"C:\\JW\\",SW_NORMAL);
#else
        std::wstring strTemp(JGW_GetApplicationFolder()),strFilePath;
        JGW_EraseLastAndFristChars(strTemp,'\\');
        JGW_EraseAfterToRemoveCharW(strTemp,L'\\');
        strFilePath = strTemp + L"Uninstall.exe";
        ShellExecute(NULL,L"open",strFilePath.c_str(),NULL,strTemp.c_str(),SW_NORMAL);
#endif      
    }

    void CCJGW_HelpDlg::OnRspUpdateDownloadMsg()
    {
        MessageBox(m_hWnd,GetString(E_HELP_TOOL_UPDATE_PACKAGE_VERSION_TIP),GetString(E_HELP_PACKAGE_VERSION_MSGTIP),MB_ICONWARNING);
        Close(IDCANCEL);
        std::wstring strParam,strExecPath;
        std::wstring strTemp(JGW_GetApplicationFolder()),strFilePath;
        JGW_EraseLastAndFristChars(strTemp,'\\');
        JGW_EraseAfterToRemoveCharW(strTemp,L'\\');
        strExecPath = strTemp;
        strTemp += L"help\\";
        strFilePath = strTemp + L"JaGuarWaveUpdateHelp.exe";

        JGW_FormatWString(strParam,L"%s&&%s&&%s",strExecPath.c_str(),_T(DEFAULT_UPDATE_FILE_PATH),msHelpConfig.msUpdateXMLConfig.mstrUpdateVersion.c_str());
#ifdef _DEBUG
        ShellExecute(NULL,L"open",L"F:\\project\\git_code_debug\\FTM-tools\\JaGuarWaveTestPlatform\\bin\\debug\\help_debug\\JaGuarWaveUpdateHelp_d.exe",strParam.c_str(),L"F:\\project\\git_code_debug\\FTM-tools\\JaGuarWaveTestPlatform\\bin\\debug\\help_debug\\",SW_NORMAL);
#else     
        ShellExecute(NULL,L"open",strFilePath.c_str(),strParam.c_str(),strTemp.c_str(),SW_NORMAL);
#endif
    }

    void CCJGW_HelpDlg::OnRspUpdateProgressTimer()
    {
        int val = mpUpdateProgressUI->GetValue();
        mpUpdateProgressUI->SetValue(val + 1);
        //! 如果5分钟都没有更新完成，就开始提示更新错误
        if (val >= 600)
        {
            KillTimer(m_hWnd,WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT);
            if (IDOK == MessageBox(m_hWnd,GetString(E_HELP_UPDATE_TIMEOUNT_TIP),GetString(E_HELP_UPDATE_TIMEOUNT_TIPMSG),MB_OKCANCEL))
            {
                JGW_KillProcessToName(HELP_SERVICE_APPLICATION_NAME);
                Close(IDCANCEL);  
                return ;
            }
            mpUpdateProgressUI->SetValue(1);
            SetTimer(m_hWnd,WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT,UPDATE_DOWNLOAD_PROGRESS_TIME,NULL);
        }
    }

    void CCJGW_HelpDlg::OnCloseBtn(TNotifyUI& msg)
    {
        //! MB_YESNOCANCEL 中止
        //! MB_RETRYCANCEL 忽略
        //! MB_YESNO 重试
        KillTimer(m_hWnd,WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT);
        int result = MessageBox(m_hWnd,GetString(E_HELP_CLOSE_UPDATE_APPLICATION),GetString(E_HELP_CLOSE_IGNORE_UPDATE_TIPMSG),MB_ABORTRETRYIGNORE);
        switch (result)
        {
        case MB_YESNOCANCEL:
            //! JGW_KillProcessToName(L"JaGuarWaveHelp.exe");
            Close(IDCANCEL);
            return;
        case MB_RETRYCANCEL:
            Close(IDOK);
            return;
        case MB_YESNO:
            break;
        }
        SetTimer(m_hWnd,WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT,UPDATE_DOWNLOAD_PROGRESS_TIME,NULL);
    }
}