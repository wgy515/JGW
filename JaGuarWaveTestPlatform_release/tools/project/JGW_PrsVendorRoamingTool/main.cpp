// JGW_PrsVendorRoamingTool.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "resource.h"
#include "CJGW_PrsVendorRoamingToolDlg.h"
#include "JGW_FoundationFunc/jgw_process_manage.h"
#include <UiLibCef/UiLibCefExportManager.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#define APPLICATION_WINDOW_NAME L"JaGuarWave Roaming Test Tool"
JGW::UiLibCefHelpInterface* gpUiLibCefHelpInterface;

//! 获取指定字段的数值型数据
#include <regex>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
std::string GetNotifStringField(const std::string& strNotif,const std::string& strField)
{
    //! "\"%s\": \\[([\\d\\s,]+)\\]

    std::string strValue = JGW::JGW_GetFormatString("\"%s\": \\[([^\\[\\]]+)\\]",strField.c_str());
    //std::string strValue = JGW::JGW_GetFormatString("\"%s\": \\[\\{\"([^@]+)\\}\\],",strField.c_str());
    //std::string strValue = JGW::JGW_GetFormatString("\"%s\": \\{\"bytes\": \\[([\\d\\s,]+)\\]\\}",strField.c_str());
    std::regex regex_patterns(strValue);

    std::smatch cm;
    std::regex_search(strNotif,cm, regex_patterns);
    if (0 == cm.size()) throw JGW::JGW_GetFormatString("[ERROR] \"%s -------------------- %s\" field not found",strNotif.c_str(),strField.c_str());
    return (std::string)cm[1];
}

//! 获取指定字段的数值型数据
int GetNotifNumericField(const std::string& strNotif,const std::string& strField)
{
    std::regex regex_patterns(JGW::JGW_GetFormatString("\"%s\": (-?\\d+)",strField.c_str()));

    std::smatch cm;
    std::regex_search(strNotif,cm, regex_patterns);
    if (0 == cm.size()) throw JGW::JGW_GetFormatString("[ERROR] \"%s -------------------- %s\" field not found",strNotif.c_str(),strField.c_str());
    return atoi(((std::string)cm[1]).c_str());
}

void AddCefDllToPath(const std::wstring &cef_path)
{
    TCHAR path_envirom[4096] = { 0 };
    GetEnvironmentVariableW(L"path", path_envirom, 4096);

    std::wstring new_envirom(cef_path);
    new_envirom.append(L";").append(path_envirom);
    SetEnvironmentVariableW(L"path", new_envirom.c_str());

    std::wstring cmd_path = cef_path + L"\\dummy_cmd.exe";
    SetEnvironmentVariableW(L"ComSpec", cmd_path.c_str());
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    std::string strValue ("{\"type\": \"DeviceHostInterfaceV2\", \"timeReceived\": 1747121215649292, \"sequence\": 0, \"classId\": 12, \"serviceId\": 7, \"operationId\": 3, \"version\": 0, \"payload\": {\"localAddress\": {\"bytes\": [24, 214, 28, 161, 13, 242]}, \"bssid\": {\"bytes\": [32, 206, 196, 2, 162, 216]}, \"eAKMSuiteSelector\": 0, \"auth_type\": 1, \"hiddenNetwork\": 0, \"ssidCount\": 5, \"ssid\": [\"h\", \"v\", \"r\", \"_\", \"1\"]}}");
    //std::string strTemp = GetNotifStringField(strValue,"ssid");
    //int nSector = GetNotifNumericField(strTemp,"sector");
    std::wstring strCefFolder;
    JGW::JGW_FormatWString(strCefFolder,L"%s%s",JGW::JGW_GetApplicationFolder(),L"cef3");
    AddCefDllToPath(strCefFolder);

    gpUiLibCefHelpInterface = JGW::GetUiLibCefHelp();
    if (!gpUiLibCefHelpInterface->Initialize())
    {
        return 0;
    }

    CPaintManagerUI::SetInstance(hInstance);
    {
        ::CoInitialize(NULL);
        JGW::CCJGW_PrsVendorRoamingToolDlg* dlg = new JGW::CCJGW_PrsVendorRoamingToolDlg;
        wkeInit();
#if 0   
        dlg->CreateNoBorderStyleWindow(_T("skin\\PrsVendorRoamingTool"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else  
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\PrsVendorRoamingTool.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\PrsVendorRoamingTool"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#endif
#endif
        dlg->CenterWindow();
        dlg->SetIcon(IDI_JGW_PRSVENDORROAMINGTOOL);
        dlg->ShowModal();
        delete dlg;
        //! wkeShutdown();
        ::CoUninitialize();
    }
    CPaintManagerUI::Term();

    JGW::JGW_KillProcessToName(L"iperf3.exe");
    JGW::JGW_KillProcessToName(L"iperf.exe");
    gpUiLibCefHelpInterface->CloseAllBrowser(true);
    gpUiLibCefHelpInterface->UnInitialize();
#if 0
    HANDLE hp   =   OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE,FALSE,GetCurrentProcessId()); 
    TerminateProcess(hp,0);
#endif
    return 0;
}
