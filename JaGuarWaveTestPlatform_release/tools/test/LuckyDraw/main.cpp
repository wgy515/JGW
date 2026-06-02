// LuckyDraw.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "CLuckyDrawDlg.h"

#define APPLICATION_WINDOW_NAME L"Lucky Draw Application"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);
    {
        JGW::CCLuckyDrawDlg* dlg = new JGW::CCLuckyDrawDlg;
#if 1
        dlg->CreateNoBorderStyleWindow(_T("skin\\LuckyDraw"),_T("skin.xml"),APPLICATION_WINDOW_NAME);
#else  
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("skin.xml"),APPLICATION_WINDOW_NAME);
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\LuckyDraw.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("skin.xml"),APPLICATION_WINDOW_NAME);
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\LuckyDraw"),_T("skin.xml"),APPLICATION_WINDOW_NAME);
#endif
#endif
        dlg->CenterWindow();
        dlg->SetIcon(IDI_LUCKYDRAW);
        dlg->ShowModal();
        delete dlg;
    }
    CPaintManagerUI::Term();
    return 0;
}