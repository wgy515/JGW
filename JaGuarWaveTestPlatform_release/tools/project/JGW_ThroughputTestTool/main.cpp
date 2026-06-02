// JGW_IperfTool.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "JGW_IperfTool.h"
#include "CJGW_IperfToolDlg.h"
#include <JGW_FoundationFunc/jgw_process_manage.h>


#define APPLICATION_WINDOW_NAME L"JaGuarWave Throughput Test Tool"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    //! STL本地化设置切回默认值
    //! std::locale::global(std::locale(""));
    GET_PROPERTY;
    CPaintManagerUI::SetInstance(hInstance);
    {
        ::CoInitialize(NULL);
        JGW::CCJGW_IperfToolDlg* dlg = new JGW::CCJGW_IperfToolDlg;
        wkeInit();
#if 0   
        dlg->CreateNoBorderStyleWindow(_T("skin\\ThroughputTestTool"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else  
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\ThroughputTestTool.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\ThroughputTestTool"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#endif
#endif
        dlg->CenterWindow();
        dlg->SetIcon(IDI_JGW_IPERFTOOL);
        dlg->ShowModal();
        delete dlg;
        wkeShutdown();
        ::CoUninitialize();
    }
    CPaintManagerUI::Term();

    JGW::JGW_KillProcessToName(L"iperf3.exe");
    JGW::JGW_KillProcessToName(L"iperf.exe");

    return 0;
}