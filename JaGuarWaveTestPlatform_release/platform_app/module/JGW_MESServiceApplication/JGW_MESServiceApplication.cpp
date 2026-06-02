// JGW_MESServiceApplication.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "JGW_MESServiceApplication.h"
#include "CJGW_MESServiceApplicationDlg.h"
#include <JGW_FoundationFunc/jgw_single_application_instance.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
/*
LEQ22425001128
DHW247105
4520094747-10
6151PP24250550

*/
#define JGW_Application_Instance_GUID "{2C843AC5-B1CA-464C-A04F-D731CAEC41E1}"
#define APPLICATION_WINDOW_NAME L"JaGuarWave MES WEB Service Application"

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	//! STL本地化设置切回默认值
	if (!JGW::jgw_check_single_application_instance(_T(JGW_Application_Instance_GUID),NULL))
	{
		return -1;
	}

	CPaintManagerUI::SetInstance(hInstance);
	{
		JGW::CCJGW_MESServiceApplicationDlg* dlg = new JGW::CCJGW_MESServiceApplicationDlg();
#if 0
		dlg->CreateNoBorderStyleWindow(_T("skin\\JGW_MESServiceApplication"),_T("help_dlg.xml"),APPLICATION_WINDOW_NAME);
#else  
#ifdef USE_ZIP_RESOURCE_ENABLE
		dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
		dlg->CreateNoBorderStyleWindow(_T(""),_T("help_dlg.xml"),APPLICATION_WINDOW_NAME);
#elif defined USE_ZIP_FILE_ENABLE
		dlg->SetResourceType(UILIB_ZIP,0,L"skin\\JGW_MESServiceApplication.zip");
		dlg->CreateNoBorderStyleWindow(_T(""),_T("help_dlg.xml"),APPLICATION_WINDOW_NAME);
#else
		dlg->CreateNoBorderStyleWindow(_T("skin\\JGW_MESServiceApplication"),_T("help_dlg.xml"),APPLICATION_WINDOW_NAME);
#endif
#endif
		dlg->CenterWindow();
		dlg->SetIcon(IDI_JGW_MESSERVICEAPPLICATION);
		dlg->ShowModal(SW_HIDE);
		delete dlg;
	}
	CPaintManagerUI::Term();
	return 0;
}
