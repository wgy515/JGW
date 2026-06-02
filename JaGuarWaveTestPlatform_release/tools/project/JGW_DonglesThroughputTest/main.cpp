// JGW_DonglesThroughputTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "resource.h"
#include "CJGW_DonglesThroughputDlg.h"

#define APPLICATION_WINDOW_NAME L"JaGuarWave Dongles Throughput Test Application"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    //! STL本地化设置切回默认值
    //! std::locale::global(std::locale(""));
    CPaintManagerUI::SetInstance(hInstance);
    {
        ::CoInitialize(NULL);
        JGW::CCJGW_DonglesThroughputDlg* dlg = new JGW::CCJGW_DonglesThroughputDlg;
        wkeInit();
#if 0   
        dlg->CreateNoBorderStyleWindow(_T("skin\\DonglesThroughputTest"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else  
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\DonglesThroughputTest.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\DonglesThroughputTest"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#endif
#endif
        dlg->CenterWindow();
        dlg->SetIcon(IDI_JGW_DONGLESTHROUGHPUTTEST);
        dlg->ShowModal();
        delete dlg;
        wkeShutdown();
        ::CoUninitialize();
    }
    CPaintManagerUI::Term();

    return 0;
}