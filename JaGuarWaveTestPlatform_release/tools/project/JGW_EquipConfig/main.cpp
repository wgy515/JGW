// JGWEquipConfig.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "resource.h"
#include "CJGW_EquipConfigDlg.h"

#define APPLICATION_WINDOW_NAME L"JaGuarWave Equip Config Application"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    //! STL本地化设置切回默认值
    //! std::locale::global(std::locale(""));
    CPaintManagerUI::SetInstance(hInstance);
    {
        JGW::CCJGW_EquipConfigDlg* dlg = new JGW::CCJGW_EquipConfigDlg;
#if 0
        dlg->CreateNoBorderStyleWindow(_T("skin\\EquipConfig"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else  
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\EquipConfig.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\EquipConfig"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#endif
#endif
        dlg->CenterWindow();
        dlg->SetIcon(IDI_JGWEQUIPCONFIG);
        dlg->ShowModal();
        delete dlg;
    }
    CPaintManagerUI::Term();

    return 0;
}