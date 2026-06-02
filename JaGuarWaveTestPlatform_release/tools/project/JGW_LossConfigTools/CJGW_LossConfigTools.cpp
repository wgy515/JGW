// JGW_LossConfigTools.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "resource.h"
#include "CJGW_LossConfigToolDlg.h"

#define APPLICATION_WINDOW_NAME L"JaGuarWave Loss Config Application"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    //! STL本地化设置切回默认值
    //! std::locale::global(std::locale(""));
    CPaintManagerUI::SetInstance(hInstance);
    {
        JGW::CCJGW_LossConfigToolDlg* dlg = new JGW::CCJGW_LossConfigToolDlg;
#if 0
        dlg->CreateNoBorderStyleWindow(_T("skin\\LossConfigTools"),_T("skin.xml"),APPLICATION_WINDOW_NAME);
#else  
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("skin.xml"),APPLICATION_WINDOW_NAME);
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\LossConfigTools.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("skin.xml"),APPLICATION_WINDOW_NAME);
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\LossConfigTools"),_T("skin.xml"),APPLICATION_WINDOW_NAME);
#endif
#endif
        dlg->CenterWindow();
        dlg->SetIcon(IDI_JGW_LOSSCONFIGTOOLS);
        dlg->ShowModal();
        delete dlg;
    }
    CPaintManagerUI::Term();
    return 0;
}