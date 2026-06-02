// TSE_ConfigEditTool.cpp : 定义应用程序的入口点。
//
#include "stdafx.h"
#include "resource.h"
#include "TSE_ConfigEditToolDlg.h"


#define APPLICATION_WINDOW_NAME L"JaGuarWave TSE Config Edit Tool"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);
    {
        JGW::CTSE_ConfigEditToolDlg* dlg = new JGW::CTSE_ConfigEditToolDlg;
#if 0
        dlg->CreateNoBorderStyleWindow(_T("skin\\TSEConfigEditTool"),_T("skin.xml"),APPLICATION_WINDOW_NAME);
#else  
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("skin.xml"),APPLICATION_WINDOW_NAME);
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\TSEConfigEditTool.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("skin.xml"),APPLICATION_WINDOW_NAME);
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\TSEConfigEditTool"),_T("skin.xml"),APPLICATION_WINDOW_NAME);
#endif
#endif
        dlg->CenterWindow();
        dlg->SetIcon(IDI_TSE_CONFIGEDITTOOL);
        dlg->ShowModal();
        delete dlg;
    }
    CPaintManagerUI::Term();
    return 0;
}