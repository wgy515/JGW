// JaGuarWaveHelp.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "CJaGuarWaveHelpDlg.h"
#include <JGW_FoundationFunc/jgw_single_application_instance.h>

#define JGW_Application_Instance_GUID "{9CA35354-5FAD-4510-B25C-BBBBF186B5ED}"
#define APPLICATION_WINDOW_NAME L"JaGuarWave Help Application"


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    ghInstance = hInstance;
    //! STL本地化设置切回默认值
    if (!JGW::jgw_check_single_application_instance(_T(JGW_Application_Instance_GUID),NULL))
    {
        return -1;
    }

    CPaintManagerUI::SetInstance(hInstance);
    {
        JGW::CCJaGuarWaveHelpDlg* dlg = new JGW::CCJaGuarWaveHelpDlg;
#if 0
        dlg->CreateNoBorderStyleWindow(_T("skin\\JaGuarWaveHelp"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else  
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWaveHelp.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\JaGuarWaveHelp"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#endif
#endif
        dlg->CenterWindow();
        dlg->SetIcon(IDI_JAGUARWAVEHELP);
        dlg->ShowModal(SW_HIDE);
        delete dlg;
    }
    CPaintManagerUI::Term();
    return 0;
}