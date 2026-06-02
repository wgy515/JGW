// JaGuarWavePackageUpdateTool.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "CCJaGuarWavePackageUpdateToolDlg.h"
//#include <JGW_WindowsFuncPlugin/CJGW_CurlFtpDownload.h>
#include <JGW_FoundationFunc/jgw_single_application_instance.h>

#define JGW_Application_Instance_GUID "{A999546E-1808-49E9-A7A8-372EFE59721F}"
#define APPLICATION_WINDOW_NAME L"JaGuarWave Package Update Application"


using namespace JGW;
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
        JGW::CCJaGuarWavePackageUpdateToolDlg* dlg = new JGW::CCJaGuarWavePackageUpdateToolDlg;
#if 0
        dlg->CreateNoBorderStyleWindow(_T("skin\\JaGuarWavePackageUpdateTool"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else  
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWavePackageUpdateTool.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\JaGuarWavePackageUpdateTool"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#endif
#endif
        dlg->CenterWindow();
        dlg->SetIcon(IDI_JAGUARWAVEPACKAGEUPDATETOOL);
        dlg->ShowModal(SW_SHOW);
        delete dlg;
    }
    CPaintManagerUI::Term();
    return 0;



















////     CCJGW_PeekNamePide cPeekNamePide;
////     cPeekNamePide.SendDosCmds(L"C:\\JaGuarWaveTestPlatformInstall\\curl.exe",L" \"ftp://192.168.8.10:26/JaGuarWaveTestPlatform_Version/Update/DHW205221/update.xml\" -u \"jiebuser:Jie@2019!\" -o \"C:\\JaGuarWaveTestPlatformInstall\\update.xml\"",60);
////     std::wstring strRead;
////     cPeekNamePide.ReadDosCmd(strRead);
//
//    CCJGW_CurlFtpDownload curlFtpDownload("192.168.8.10","jiebuser","Jie@2019!",26);
//    //bool result = curlFtpDownload.DownloadCurlFTPFile("/JaGuarWaveTestPlatform_Version/Update/DHW205221/update.xml","C:\\JaGuarWaveTestPlatformInstall\\update.xml");
//    //bool result = curlFtpDownload.UpdateCurlFTPFile("/JaGuarWaveTestPlatform_Version/Update/DHW205221/update111.xml","C:\\JaGuarWaveTestPlatformInstall\\update.xml");
//    bool result = curlFtpDownload.CreateCurlFTPDirectory("/JaGuarWaveTestPlatform_Version/Update/DHW205227");
//    result = curlFtpDownload.TraverseCreateFTPDirectory("/JaGuarWaveTestPlatform_Version2/Update2/DHW205224");
//    //！curl ftp://192.168.8.10:26/JaGuarWaveTestPlatform_Version/Update/DHW205221/update.xml -u "jiebuser:Jie@2019!" -o C:\\JaGuarWaveTestPlatformInstall\\update.xml


	return 0;
}