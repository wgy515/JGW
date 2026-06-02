// PTP4100_WiFi6Test.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PTP4100_WiFi6Test.h"
#include "PTP4100WiFi6TestMainDlg.h"
#include <JGW_FoundationFunc/jgw_single_application_instance.h>
#define JGW_Application_Instance_GUID "{BC3391ED-801D-45F4-8661-1199CA1D527D}"
#define APPLICATION_WINDOW_NAME L"JaGuarWave WiFi6 Test Tool"
#if 0
设置VGA1 0x01
echo -ne '\xFF\xFE\x20\x32\x01\x00\x00\x00\x01\x00\x00\x00\x00\x00\xFE\xFF\x0A'> /dev/ttyACM0
设置VGA2  0x01
echo -ne '\xFF\xFE\x20\x32\x01\x00\x00\x01\x00\x00\x00\x00\x00\x00\xFE\xFF\x0A'> /dev/ttyACM0
设置RX ATT 0x01
echo -ne '\xFF\xFE\x20\x32\x01\x00\x00\x00\x00\x01\x00\x00\x00\x00\xFE\xFF\x0A'> /dev/ttyACM0


#endif

using namespace JGW;
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
#if 0
    CCJGWQMSLWlanDeivce qmslWlanDeivce;
    if (!qmslWlanDeivce.ConnectStandaloneWlanCardQUTS("192.168.10.2"))
    {
        return false;
    }
#if 0
    if (!qmslWlanDeivce.LoadDut())
    {
        return false;
    }

    if (!qmslWlanDeivce.StartTx(18))
    {
        return false;
    }
    qmslWlanDeivce.StopTx();
#else

    if (!qmslWlanDeivce.LoadDut())
    {
        return false;
    }

    if (!qmslWlanDeivce.StartRx())
    {
        return false;
    }
    Sleep(1000);
    qmslWlanDeivce.StopRxWithReport(PHY_B);
#endif

   
    qmslWlanDeivce.CloseDut();
    qmslWlanDeivce.DisconnectStandaloneWlanCard();
#else
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
        JGW::CPTP4100WiFi6TestMainDlg* dlg = new JGW::CPTP4100WiFi6TestMainDlg;
#if 0
        dlg->CreateNoBorderStyleWindow(_T("skin\\PTP4100_WiFi6Test"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else  
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\PTP4100_WiFi6Test.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\PTP4100_WiFi6Test"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#endif
#endif
        dlg->CenterWindow();
        dlg->SetIcon(IDI_PTP4100_WIFI6TEST);
        dlg->ShowModal(SW_NORMAL);
        delete dlg;
    }
    CPaintManagerUI::Term();

#ifndef _DEBUG
    HANDLE hp   =   OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE,FALSE,GetCurrentProcessId()); 
    TerminateProcess(hp,0);
#endif
#endif
     return 0;
}