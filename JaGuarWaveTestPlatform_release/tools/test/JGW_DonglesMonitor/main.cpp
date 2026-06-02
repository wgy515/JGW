// JGW_DonglesMonitor.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "resource.h"
#include "CJGW_DonglesMonitorDlg.h"
#include "CJGW_DonglesSelectNetCardDlg.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);
    std::string strNetCardInfo;
    {
        JGW::CCJGW_DonglesSelectNetCardDlg* dlg = new JGW::CCJGW_DonglesSelectNetCardDlg(strNetCardInfo);
#if 1
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("SelectNetCard.xml"),_T("JaGuarWave Dongles Status Monitor"));
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\DonglesMonitor.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("SelectNetCard.xml"),_T("JaGuarWave Dongles Status Monitor"));
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\DonglesMonitor"),_T("SelectNetCard.xml"),_T("JaGuarWave Dongles Status Monitor"));
#endif
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\DonglesMonitor"),_T("SelectNetCard.xml"),_T("JaGuarWave Dongles Status Monitor"));
#endif
        dlg->CenterWindow();
        dlg->SetIcon(IDI_JGW_DONGLESMONITOR);
        if (IDOK !=dlg->ShowModal())
        {
            delete dlg;
            return 1;
        }
        delete dlg;
    }
    {
        JGW::CCJGW_DonglesMonitorDlg* dlg = new JGW::CCJGW_DonglesMonitorDlg(strNetCardInfo);
#if 1
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("DonglesMonitor.xml"),_T("JaGuarWave Dongles Status Monitor"),NULL,UI_WNDSTYLE_FRAME,WS_EX_TOOLWINDOW);
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\DonglesMonitor.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("DonglesMonitor.xml"),_T("JaGuarWave Dongles Status Monitor"),NULL,UI_WNDSTYLE_FRAME,WS_EX_TOOLWINDOW);
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\DonglesMonitor"),_T("DonglesMonitor.xml"),_T("JaGuarWave Dongles Status Monitor"),NULL,UI_WNDSTYLE_FRAME,WS_EX_TOOLWINDOW);
#endif
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\DonglesMonitor"),_T("DonglesMonitor.xml"),_T("JaGuarWave Dongles Status Monitor"),NULL,UI_WNDSTYLE_FRAME,WS_EX_TOOLWINDOW);
#endif
        dlg->CenterWindow();
        dlg->SetIcon(IDI_JGW_DONGLESMONITOR);
        dlg->ShowModal();
        delete dlg;
    }
    CPaintManagerUI::Term();
    return 0;
}