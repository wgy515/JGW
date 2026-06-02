// JGW_Cef3SubProcesses.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <JGW_Cef3Plugin/JGW_Cef3PluginExport.h>
#include <Windows.h>

//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

int APIENTRY wWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine,
    int nCmdShow)
{
#if 0
    // Structure for passing command-line arguments.
    // The definition of this structure is platform-specific.
    // 传递命令行参数的结构体。
    // 这个结构体的定义与平台相关。
    CefMainArgs main_args(::GetModuleHandle(nullptr));


    // Optional implementation of the CefApp interface.
    // 可选择性地实现CefApp接口
    CefRefPtr<JGW::CCJGW_Cef3RendererApp> app(new JGW::CCJGW_Cef3RendererApp);

    // Execute the sub-process logic. This will block until the sub-process should exit.
    // 执行子进程逻辑，此时会堵塞直到子进程退出。
    return CefExecuteProcess(main_args, app.get(),NULL);
#else
    //HWND console = GetConsoleWindow();
    //ShowWindow(console, SW_HIDE);

    return ExecuteSubProcesses(::GetModuleHandle(NULL));
#endif
}

