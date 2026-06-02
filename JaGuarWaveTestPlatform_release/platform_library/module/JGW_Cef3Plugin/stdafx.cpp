// stdafx.cpp : 只包括标准包含文件的源文件
// JGW_Cef3Plugin.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
int gnMainIconResId = 107;
/*
// Program entry-point function.

// 程序入口函数

int main(int argc, char* argv[]) {
// Structure for passing command-line arguments.
// The definition of this structure is platform-specific.
// 传递命令行参数的结构体。
// 这个结构体的定义与平台相关。

CefMainArgs main_args(argc, argv);
// Optional implementation of the CefApp interface.

// 可选择性地实现CefApp接口
CefRefPtr<MyApp> app(new MyApp);
// Populate this structure to customize CEF behavior.

// 填充这个结构体，用于定制CEF的行为。
CefSettings settings;

// Specify the path for the sub-process executable.
// 指定子进程的执行路径
CefString(&settings.browser_subprocess_path).FromASCII(“/path/to/subprocess”);
// Initialize CEF in the main process.

// 在主进程中初始化CEF
CefInitialize(main_args, settings, app.get());

// Run the CEF message loop. This will block until CefQuitMessageLoop() is called.

// 执行消息循环，此时会堵塞，直到CefQuitMessageLoop()函数被调用。
CefRunMessageLoop();



// Shut down CEF.
// 关闭CEF
CefShutdown();

return 0;
}
*/
