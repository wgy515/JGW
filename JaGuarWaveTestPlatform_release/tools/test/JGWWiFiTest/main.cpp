// JGWWiFiTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "resource.h"
#include "CJGWWiFiTestDlg.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{

    CPaintManagerUI::SetInstance(hInstance);
    {
        JGW::CCJGWWiFiTestDlg* dlg = new JGW::CCJGWWiFiTestDlg;
#if 1
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("wifi_test.xml"),_T("JaGuarWave WiFi Test Application"));
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\JGWWiFiTest.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("wifi_test.xml"),_T("JaGuarWave WiFi Test Application"));
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\JGWWiFiTest"),_T("wifi_test.xml"),_T("JaGuarWave WiFi Test Application"));
#endif
#else
       dlg->CreateNoBorderStyleWindow(_T("skin\\JGWWiFiTest"),_T("wifi_test.xml"),_T("JaGuarWave WiFi Test Application"));
#endif
       dlg->CenterWindow();
       dlg->SetIcon(IDI_JGWWIFITEST);
       dlg->ShowModal();
       delete dlg;
    }
    CPaintManagerUI::Term();
    return 0;
}