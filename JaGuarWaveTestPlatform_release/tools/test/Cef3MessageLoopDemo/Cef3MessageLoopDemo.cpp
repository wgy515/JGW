// Cef3MessageLoopDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "resource.h"
#include <include/cef_app.h>
#include <JGW_Cef3Plugin/JGW_Cef3PluginExport.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>

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


int APIENTRY wWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine,
    int nCmdShow)
{
    std::wstring strSubProcessesPath;
    std::wstring strCefFolder;

    JGW::JGW_FormatWString(strCefFolder,L"%s%s",JGW::JGW_GetApplicationFolder(),L"cef3");
    AddCefDllToPath(strCefFolder);

    JGW::JGW_FormatWString(strSubProcessesPath,L"%scef3\\%s",JGW::JGW_GetApplicationFolder(),L"JGW_Cef3SubProcesses.exe");
    // Run the CEF message loop. This will block until CefQuitMessageLoop() is called.
    InitializeCef3Processes(GetModuleHandle(NULL),false,strSubProcessesPath.c_str());

    SetZipResResource(hInstance,IDR_ZIPRES1,L"ZIPRES");


    RECT rect ={0};
    //CreateBrowserHelp(NULL,rect,L"file:///G:/CEF/cef_binary_83.5.0+gbf03589+chromium-83.0.4103.106_windows32/tests/cefclient/resources/dialogs.html");

    //CreateBrowserHelp(NULL,rect,L"client://ziprs.com/dialogs.html");

    CreateBrowserHelp(NULL,rect,L"https://zipres.com/prs_connect_status.html");
    // 执行消息循环，此时会堵塞，直到CefQuitMessageLoop()函数被调用。
    CefRunMessageLoop();

    // Shut down CEF.
    // 关闭CEF
    CefShutdown();

    return 0;
}

