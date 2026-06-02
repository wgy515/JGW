// JaGuarWavePackageUpdateToolV2.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "resource.h"
#include "CJaGuarWavePackageUpdateToolDlgV2.h"
#include <JGW_FoundationFunc/jgw_single_application_instance.h>


#define JGW_Application_Instance_GUID "{9193D841-8867-4346-B2D4-5CDDF5D689CD}"
#define APPLICATION_WINDOW_NAME L"JaGuarWave Package Update Application V2"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    ghInstance = hInstance;
    //! STL本地化设置切回默认值
    if (!JGW::jgw_check_single_application_instance(_T(JGW_Application_Instance_GUID),NULL))
    {
        return -1;
    }

    CPaintManagerUI::SetInstance(hInstance);
    {
        JGW::CCJaGuarWavePackageUpdateToolDlgV2* dlg = new JGW::CCJaGuarWavePackageUpdateToolDlgV2;
#if 0
        dlg->CreateNoBorderStyleWindow(_T("skin\\JaGuarWavePackageUpdateToolV2"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else  
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWavePackageUpdateToolV2.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\JaGuarWavePackageUpdateToolV2"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#endif
#endif
        dlg->CenterWindow();
        dlg->SetIcon(IDI_JAGUARWAVEPACKAGEUPDATETOOLV2);
        dlg->ShowModal();
        delete dlg;
    }
    CPaintManagerUI::Term();
    return 0;
}