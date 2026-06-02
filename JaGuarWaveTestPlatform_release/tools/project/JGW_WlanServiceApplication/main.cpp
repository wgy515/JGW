// JGW_WlanServiceApplication.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "CJGW_WlanServiceApplicationDlg.h"
#include <JGW_FoundationFunc/jgw_single_application_instance.h>

#define JGW_Application_Instance_GUID "{6B6321FF-ED54-4164-9E67-7FAECA61B3BE}"
#define APPLICATION_WINDOW_NAME L"JaGuarWave Wlan Service Application"

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	//! STL本地化设置切回默认值
    JGW::ghInstance = hInstance;
    int i = 0;
	//! std::locale::global(std::locale(""));
    while(!JGW::jgw_check_single_application_instance(_T(JGW_Application_Instance_GUID),NULL))
    {
        Sleep(1000);
        i ++;
        if (i > 30) return -1;
    }

	CPaintManagerUI::SetInstance(hInstance);
	{
		JGW::CCJGW_WlanServiceApplicationDlg* dlg = new JGW::CCJGW_WlanServiceApplicationDlg;
#if 0
		dlg->CreateNoBorderStyleWindow(_T("skin\\WlanServiceApplication"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else  
#ifdef USE_ZIP_RESOURCE_ENABLE
		dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
		dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#elif defined USE_ZIP_FILE_ENABLE
		dlg->SetResourceType(UILIB_ZIP,0,L"skin\\LossConfigTools.zip");
		dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else
		dlg->CreateNoBorderStyleWindow(_T("skin\\WlanServiceApplication"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#endif
#endif
		dlg->CenterWindow();
		dlg->SetIcon(IDI_JGW_WLANSERVICEAPPLICATION);
		dlg->ShowModal(SW_HIDE);
		delete dlg;
	}
	CPaintManagerUI::Term();

#ifndef _DEBUG
    HANDLE hp   =   OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE,FALSE,GetCurrentProcessId()); 
    TerminateProcess(hp,0);
#endif

	return 0;
}