// JGW_TestLogAnalyticalTools.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "resource.h"
#include "CJGW_TestLogAnalyticalTools.h"

#define APPLICATION_WINDOW_NAME L"JaGuarWave Test Status"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    //! STL本地化设置切回默认值
    //! std::locale::global(std::locale("chs"));
    CPaintManagerUI::SetInstance(hInstance);
    {
        JGW::CCJGW_TestLogAnalyticalToolsDlg* dlg = new JGW::CCJGW_TestLogAnalyticalToolsDlg;
#if 0
        dlg->CreateNoBorderStyleWindow(_T("skin\\TestLogAnalyticalTools"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else  
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\TestLogAnalyticalTools.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\TestLogAnalyticalTools"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#endif
#endif
        dlg->CenterWindow();
        dlg->SetIcon(IDI_JGW_TESTLOGANALYTICALTOOLS);
        dlg->ShowModal();
        delete dlg;
    }
    CPaintManagerUI::Term();

    return 0;
}