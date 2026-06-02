// JGW_StationMonitor.cpp : 定义应用程序的入口点。
//
#include "stdafx.h"
#include "resource.h"
#include "CJGW_StationMonitorMainDlg.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
//     std::string strJson("{\"requestCode\":1001,\"mac\":\"b4:c0:f5:d7:49:ba\",\"ip\":\"192.168.133.67\",\"class\":0,\"rssi\":-18,\"tx_mcs\":4,\"rx_mcs\":4,\"tx_sector\":56,\"rx_sector\":56,\"tx_drop_rate\":0,\"rx_drop_rate\":0}");
//     boost::shared_ptr<std::vector<char>> ptrBuf(new std::vector<char>(strJson.length(),0));
//     strcpy_s(&(*ptrBuf)[0],strJson.length() + 1,strJson.c_str());


    CPaintManagerUI::SetInstance(hInstance);
    {
        JGW::CCJGW_StationMonitorMainDlg* dlg = new JGW::CCJGW_StationMonitorMainDlg();
// #ifdef USE_ZIP_RESOURCE_ENABLE
//         dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
//         dlg->CreateNoBorderStyleWindow(_T(""),_T("StationMonitor.xml"),_T("JaGuarWave Station Status Monitor"),NULL,UI_WNDSTYLE_FRAME,WS_EX_TOOLWINDOW);
// #elif defined USE_ZIP_FILE_ENABLE
//         dlg->SetResourceType(UILIB_ZIP,0,L"skin\\StationMonitor.zip");
//         dlg->CreateNoBorderStyleWindow(_T(""),_T("DonglesMonitor.xml"),_T("JaGuarWave Station Status Monitor"),NULL,UI_WNDSTYLE_FRAME,WS_EX_TOOLWINDOW);
// #else
//         dlg->CreateNoBorderStyleWindow(_T("skin\\StationMonitor"),_T("StationMonitor.xml"),_T("JaGuarWave Station Status Monitor"),NULL,UI_WNDSTYLE_FRAME,WS_EX_TOOLWINDOW);
// #endif
// #else
//         dlg->CreateNoBorderStyleWindow(_T("skin\\StationMonitor"),_T("StationMonitor.xml"),_T("JaGuarWave Station Status Monitor"),NULL,UI_WNDSTYLE_FRAME,WS_EX_TOOLWINDOW);
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("StationMonitor.xml"),_T("JaGuarWave Station Status Monitor"));
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\StationMonitor.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("StationMonitor.xml"),_T("JaGuarWave Station Status Monitor"));
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\StationMonitor"),_T("StationMonitor.xml"),_T("JaGuarWave Station Status Monitor"));
#endif


        dlg->CenterWindow();
        dlg->SetIcon(IDI_JGW_STATIONMONITOR);
        dlg->ShowModal();
        delete dlg;
    }
    CPaintManagerUI::Term();
    return 0;
}