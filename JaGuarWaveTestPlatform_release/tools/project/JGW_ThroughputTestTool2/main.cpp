// JGW_ThroughputTestTool2.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "resource.h"
#include "CJGW_ThroughputTestToolDlg2.h"
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <JGW_PropertyConfigPlugin/CJGW_PropertyExport.h>

#define PER_ANGLE_TURN 10000
#define TURNTABLE_NODE_ID 32
#define PROPERTY_FILE_NAME L"throughput.test.property.config"
#define GET_PROPERTY GetFilePropertyToFileName(PROPERTY_FILE_NAME)

#define APPLICATION_WINDOW_NAME L"JaGuarWave Throughput Test Tool V2"

int APIENTRY _tWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)
{
    //! STL本地化设置切回默认值
    //! std::locale::global(std::locale("chs"));
    GET_PROPERTY;
    CPaintManagerUI::SetInstance(hInstance);
    {
        ::CoInitialize(NULL);
        JGW::CCJGW_ThroughputTestToolDlg2* dlg = new JGW::CCJGW_ThroughputTestToolDlg2;
        wkeInit();
#if 0   
        dlg->CreateNoBorderStyleWindow(_T("skin\\ThroughputTestTool2"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else  
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\ThroughputTestTool.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\ThroughputTestTool2"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#endif
#endif
        dlg->CenterWindow();
        dlg->SetIcon(IDI_JGW_THROUGHPUTTESTTOOL2);
        dlg->ShowModal();
        delete dlg;
        //! wkeShutdown();
        ::CoUninitialize();
    }
    CPaintManagerUI::Term();

    JGW::JGW_KillProcessToName(L"iperf3.exe");
    JGW::JGW_KillProcessToName(L"iperf.exe");

    return 0;
}