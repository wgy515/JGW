// JGW_TSEConfigEditTools.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Resource.h"
#include "CJGW_TSEConfigEditDlg.h"

#define APPLICATION_WINDOW_NAME L"JaGuarWave TSE Config Edit Application"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    //! STL本地化设置切回默认值
    //! std::locale::global(std::locale(""));
    CPaintManagerUI::SetInstance(hInstance);
    {
        JGW::CCJGW_TSEConfigEditDlg* dlg = new JGW::CCJGW_TSEConfigEditDlg;
#if 0
        dlg->CreateNoBorderStyleWindow(_T("skin\\TSEConfigEditTools"),_T("skin.xml"),APPLICATION_WINDOW_NAME);
#else  
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("skin.xml"),APPLICATION_WINDOW_NAME);
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\TSEConfigEditTools.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("skin.xml"),APPLICATION_WINDOW_NAME);
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\TSEConfigEditTools"),_T("skin.xml"),APPLICATION_WINDOW_NAME);
#endif
#endif
        dlg->CenterWindow();
        dlg->SetIcon(IDI_JGW_TSECONFIGEDITTOOLS);
        dlg->ShowModal();
        delete dlg;
    }
    CPaintManagerUI::Term();
    return 0;
}
