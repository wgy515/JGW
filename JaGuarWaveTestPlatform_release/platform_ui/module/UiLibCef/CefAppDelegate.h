#pragma once
#include <include/cef_app.h>
#include <include\cef_command_line.h>

namespace JGW
{
    class JGW_Cef3HelpApp;

    class CefAppDelegate : public virtual CefBaseRefCounted {
    public:
        //! 提供在CEF和Chromium处理之前查看和/或修改命令行参数的机会。对于浏览器进程，|process_type|值将为空。不要保留对传递给该方法的CefCommandLine对象的引用。CefSettings.command_line_args_disabled值可用于以空命令行对象启动。在CefSettings中指定的等同于命令行参数的任何值都将在调用此方法之前设置。使用此方法修改非浏览器进程的命令行参数时要小心，因为这可能导致未定义的行为，包括崩溃。
        virtual void OnBeforeCommandLineProcessing(
            CefRefPtr<JGW_Cef3HelpApp> app,
            CefRefPtr<CefCommandLine> command_line);
        //! 在CEF上下文初始化后立即在浏览器进程UI线程上调用。
        virtual void OnContextInitialized(CefRefPtr<JGW_Cef3HelpApp> app);
        //! 在启动子进程之前调用。当启动渲染进程时，在浏览器进程UI线程上调用；当启动GPU或插件进程时，在浏览器进程IO线程上调用。提供修改子进程命令行的机会。不要在这个方法之外保留对|command_line|的引用。
        virtual void OnBeforeChildProcessLaunch(
            CefRefPtr<JGW_Cef3HelpApp> app,
            CefRefPtr<CefCommandLine> command_line);
        //! 在为新的渲染进程创建主线程后在浏览器进程IO线程上调用。提供了一个指定额外信息的机会，这些信息将在渲染过程中传递给CefRenderProcessHandler::OnRenderThreadCreated（）。不要在这个方法之外保留对|extra_info|的引用。
        virtual void OnRenderProcessThreadCreated(
            CefRefPtr<JGW_Cef3HelpApp> app,
            CefRefPtr<CefListValue> extra_info);
    };
}

