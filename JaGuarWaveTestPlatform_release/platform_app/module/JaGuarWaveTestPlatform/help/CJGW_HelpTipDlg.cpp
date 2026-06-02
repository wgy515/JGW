#include "StdAfx.h"
#include "CJGW_HelpTipDlg.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "../../../../platform_include/JGW_MSG_ID_Define.h"
namespace JGW
{
    CCJGW_HelpTipDlg::CCJGW_HelpTipDlg(S_JGW_HELP_CONFIG& sHelpConfig,HWND hParentHwnd /*= NULL*/):m_hParentHwnd(hParentHwnd),mhMainHwnd(NULL),msHelpConfig(sHelpConfig)
    {
    }


    CCJGW_HelpTipDlg::~CCJGW_HelpTipDlg(void)
    {
    }

    UINT CCJGW_HelpTipDlg::GetClassStyle() const
    {
        return UI_CLASSSTYLE_DIALOG;
    }

    int CCJGW_HelpTipDlg::DoModal(HWND hMainHwnd,E_JGW_HELP_AUTO_UPDATE_TYPE autoUpdateType,RECT rcDlg)
    {
        mAutoUpdateType = autoUpdateType;
        mhMainHwnd = hMainHwnd;
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateNoBorderStyleWindow(_T(""),_T("help_view_tip.xml"),_T("Automatic Updates Tip Windows"),m_hParentHwnd,UI_WNDSTYLE_FRAME);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
        CreateNoBorderStyleWindow(_T(""),_T("help_view_tip.xml"),_T("Automatic Updates Tip Windows"),m_hParentHwnd,UI_WNDSTYLE_FRAME);
#else
        CreateNoBorderStyleWindow(_T("skin\\JaGuarWave"),_T("help_view_tip.xml"),_T("Automatic Updates Tip Windows"),m_hParentHwnd,UI_WNDSTYLE_FRAME);
#endif
        //! 移动到主客户区右下角
        RECT rc = {0};
        int xLeft = 0, yTop = 0;
        GetClientRect(m_hWnd,&rc);
        
        xLeft = rcDlg.right - (rc.right - rc.left);
        yTop = rcDlg.bottom  -  (rc.bottom - rc.top);

        //! CenterWindow();
        ::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

        
        return ShowModal();
    }

    void CCJGW_HelpTipDlg::OnInitWindow()
    {
        SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
       // m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJGW_HelpTipDlg::OnEventCustomMessage,0);
    }

    void CCJGW_HelpTipDlg::OnCloseWindow()
    {
        ::PostMessage(mhMainHwnd,WM_HWND_HELP_TIP_CLISE_MSG,NULL,NULL);    
    }

    void CCJGW_HelpTipDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender->GetName() == L"updateNowBtn")
            {
                Close(IDOK);
                ::PostMessage(mhMainHwnd,WM_CLOSE, (WPARAM)IDOK, 0L);
                if (mAutoUpdateType == E_HELP_RSP_DOWNLOAD_TYPE)
                    OnRspHelpDownloadMsg();
                else if (mAutoUpdateType == E_HELP_RSP_UPDATE_TYPE)
                    OnRspUpdateDownloadMsg();
            }
            else if (msg.pSender->GetName() == L"updateLaterBtn")
            {
                Close(IDOK);
                //!::PostMessage(mhMainHwnd,WM_HWND_HELP_TIP_CLISE_MSG,NULL,NULL);   
            }
        }
    }

    void CCJGW_HelpTipDlg::OnRspHelpDownloadMsg()
    {
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

    void CCJGW_HelpTipDlg::OnRspUpdateDownloadMsg()
    {
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
//#ifdef _DEBUG
//        //ShellExecute(NULL,L"open",L"C:\\JW\\help\\JaGuarWaveUpdateHelp.exe",NULL,L"C:\\JW\\help\\",SW_NORMAL);
//        ShellExecute(NULL,L"open",L"F:\\project\\git_code_debug\\FTM-tools\\JaGuarWaveTestPlatform\\bin\\debug\\help_debug\\JaGuarWaveUpdateHelp_d.exe",NULL,L"F:\\project\\git_code_debug\\FTM-tools\\JaGuarWaveTestPlatform\\bin\\debug\\help_debug\\",SW_NORMAL);
//#else
//        std::wstring strTemp(JGW_GetApplicationFolder()),strFilePath;
//        JGW_EraseLastAndFristChars(strTemp,'\\');
//        JGW_EraseAfterToRemoveCharW(strTemp,L'\\');
//        strTemp += L"help\\";
//        strFilePath = strTemp + L"JaGuarWaveUpdateHelp.exe";
//        ShellExecute(NULL,L"open",strFilePath.c_str(),NULL,strTemp.c_str(),SW_NORMAL);
//#endif
    }
}

