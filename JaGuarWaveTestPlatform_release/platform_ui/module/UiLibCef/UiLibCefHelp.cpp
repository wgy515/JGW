#include "StdAfx.h"
#include "UiLibCefHelp.h"

#include <include\internal\cef_string_wrappers.h>
#include <include\cef_app.h>
#include "JGW_Cef3HelpApp.h"
#include "CefRendererDelegate.h"
#include "JGW_Cef3MessageRouterHandler.h"

namespace JGW
{
    JGW_Cef3HelpApp::DelegateSet gDelegateSet;

    UiLibCefHelpInterface* GetUiLibCefHelp()
    {
        return new UiLibCefHelp();
    }

    void DestroyWiFiSetting(UiLibCefHelpInterface* pUiLibCefHelpInterface)
    {
        UiLibCefHelp* pUiLibCefHelp = (UiLibCefHelp*)pUiLibCefHelpInterface;
        delete pUiLibCefHelp;
    }

    UiLibCefHelp::UiLibCefHelp() : m_is_enable_offset_render(false),mpCef3HelpHandler(NULL)
    {
        gDelegateSet.insert(new CefRendererDelegate());
    }

    UiLibCefHelp::~UiLibCefHelp()
    {
        if (NULL != mpCef3HelpHandler)
        {
            CefHandlerDelegate* pCefHandlerDelegate = mpCef3HelpHandler->GetCefHandlerDelegate();
            mpCef3HelpHandler->DetachDelegate();
            delete pCefHandlerDelegate;
            delete mpCef3HelpHandler;
        }

    }

    //void Prompt(CefRefPtr<CefBrowser> browser,
    //    const std::string& type,
    //    const std::string& label,
    //    const std::string& default_value) {
    //        // Prompt the user for a new value. Works as follows:
    //        // 1. Show a prompt() dialog via JavaScript.
    //        // 2. Pass the result to window.cefQuery().
    //        // 3. Handle the result in PromptHandler::OnQuery.
    //        const std::string& code = "window.cefQuery({'request': '" +
    //            std::string(kPrompt) + type + ":' + prompt('" +
    //            label + "', '" + default_value + "')});";
    //        browser->GetMainFrame()->ExecuteJavaScript(
    //            code, browser->GetMainFrame()->GetURL(), 0);
    //}

    void JGW_Cef3HelpApp::CreateCefRendererDelegates(DelegateSet& delegates)
    {
        delegates.insert(gDelegateSet.begin(),gDelegateSet.end());
    }

    void JGW_Cef3HelpApp::CreateCefAppDelegates(CefAppDelegateSet& delegates)
    {

    }

    void JGW_Cef3HelpApp::RegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar,
        std::vector<CefString>& cookiable_schemes) {
            registrar->AddCustomScheme(
                "client", CEF_SCHEME_OPTION_STANDARD | CEF_SCHEME_OPTION_CORS_ENABLED);
    }

    void UiLibCefHelp::AddCefRendererDelegate(void* pCefRendererDelegate)
    {
        //! 
        gDelegateSet.insert((CefRendererDelegate*)pCefRendererDelegate);
    }

    void UiLibCefHelp::GetCefSetting(const std::wstring &cef_temp_path, CefSettings &settings)
    {
        // --no-sandbox非沙箱模式
        settings.no_sandbox = true;

        // cache_path缓存数据将存储在磁盘上的位置。如果为空，内存缓存将用于某些功能，而临时磁盘缓存将用于其他功能。如果指定了缓存路径，HTML5 数据库（如 localStorage）将仅在会话中持久存在。
        // 设置localstorage，不要在路径末尾加"\\"，否则运行时会报错
        CefString(&settings.cache_path) = cef_temp_path + L"\\CefLocalStorage";
        CefString(&settings.root_cache_path) = cef_temp_path + L"\\CefLocalRootStorage";
        // log_file用于调试日志的目录和文件名。如果为空，将使用默认名称“debug.log”并将文件写入应用程序目录。也可使用“日志文件”命令行开关进行配置。
        // 设置debug log文件位置
        CefString(&settings.log_file) = cef_temp_path + L"\\cef.log";

        // multi_threaded_message_loop设置为 true 让浏览器处理消息循环在单独的线程中运行。有关详细信息，请参阅“消息循环集成”部分。
        // cef2623、2526版本debug模式:在使用multi_threaded_message_loop时退出程序会触发中断
        // 加入disable-extensions参数可以修复这个问题，但是会导致一些页面打开时报错
        // 开启Cef多线程消息循环，兼容nbase库消息循环
        // 定期调用 CefDoMessageLoopWork() 而不是调用 CefRunMessageLoop()。每次调用 CefDoMessageLoopWork() 都会执行一次 CEF 消息循环迭代。应谨慎使用此方法。过于频繁地调用该方法会使 CEF 消息循环耗尽并对浏览器性能产生负面影响。过于频繁地调用该方法会对 CPU 使用率产生负面影响。有关高级用法的详细信息，请参阅CefBrowserProcessHandler::OnScheduleMessagePumpWork。您可以在 cefclient 中通过使用“--external-message-pump”命令行标志运行来测试此模式。
        // 设置 CefSettings.multi_threaded_message_loop = true（仅限 Windows 和 Linux）。这将导致 CEF 在与主应用程序线程不同的线程上运行浏览器 UI 线程。使用这种方法，既不需要调用 CefDoMessageLoopWork() 也不需要调用 CefRunMessageLoop()。CefInitialize() 和 CefShutdown() 仍应在主应用程序线程上调用。您将需要提供自己的机制来与主应用程序线程通信（例如，请参阅 cefclient_win.cpp 中的消息窗口用法）。您可以在 Windows 或 Linux 上的 cefclient 中通过使用“--multi-threaded-message-loop”命令行标志运行来测试此模式。
        settings.multi_threaded_message_loop = true;

        // browser_subprocess_path将为子进程启动的单独可执行文件的路径。有关详细信息，请参阅“单独的子流程可执行文件”部分。
        //settings.browser_subprocess_path;
        // Specify the path for the sub-process executable.
        //CefString(&settings.browser_subprocess_path).FromASCII(“ / path / to / subprocess”);

        // command_line_args_disabled设置为 true 以禁用使用标准 CEF 和 Chromium 命令行参数配置浏览器进程功能。有关详细信息，请参阅“命令行参数”部分。

        // 开启离屏渲染
        settings.windowless_rendering_enabled = m_is_enable_offset_render;
    }

    bool UiLibCefHelp::Initialize(const std::wstring &cef_temp_path /* = L"cef_temp" */, const bool &is_enable_offset_render /* = false */)
    {
#if 1
        bool ret = false;

        //启用高分辨率，启动进程
        CefEnableHighDPISupport();

        m_is_enable_offset_render = is_enable_offset_render;
        CefSettings settings;
        CefMainArgs main_args(GetModuleHandle(NULL));
        CefRefPtr<JGW_Cef3HelpApp> app(new JGW_Cef3HelpApp());

        //! 子进程（渲染进程）主进程浏览器进程
        // 如果是在子进程中调用，会堵塞直到子进程退出，并且exit_code返回大于等于0
        // 如果在Browser进程中调用，则立即返回-1
        int exit_code = CefExecuteProcess(main_args, app.get(), NULL);
        if (exit_code >= 0)  return false;

        GetCefSetting(cef_temp_path, settings);
        ret = CefInitialize(main_args, settings, app.get(), NULL);

        if (m_is_enable_offset_render)
        {
            //HWND hwnd = CreateWindow(L"Static", L"", WS_POPUP, 0, 0, 0, 0, NULL, NULL, NULL, NULL);
            //(TID_UI, base::Bind(&FixContextMenuBug, hwnd));
        }
        return ret;
#else

        // Enable High-DPI support on Windows 7 or newer.
        CefEnableHighDPISupport();

        void* sandbox_info = nullptr;

#if defined(CEF_USE_SANDBOX)
        // Manage the life span of the sandbox information object. This is necessary
        // for sandbox support on Windows. See cef_sandbox_win.h for complete details.
        CefScopedSandboxInfo scoped_sandbox;
        sandbox_info = scoped_sandbox.sandbox_info();
#endif

        // Provide CEF with command-line arguments.
        CefMainArgs main_args(GetModuleHandle(NULL));

        // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
        // that share the same executable. This function checks the command-line and,
        // if this is a sub-process, executes the appropriate logic.
        int exit_code = CefExecuteProcess(main_args, nullptr, sandbox_info);
        if (exit_code >= 0) {
            // The sub-process has completed so return here.
            return false;
        }

        // Specify CEF global settings here.
        CefSettings settings;

#if !defined(CEF_USE_SANDBOX)
        settings.no_sandbox = true;
#endif

        // SimpleApp implements application-level callbacks for the browser process.
        // It will create the first browser instance in OnContextInitialized() after
        // CEF has initialized.
        //CefRefPtr<SimpleApp> app(new SimpleApp);
         CefRefPtr<JGW_Cef3HelpApp> app(new JGW_Cef3HelpApp());

        // Initialize CEF.
        CefInitialize(main_args, settings, app.get(), sandbox_info);
#endif
        return true;
    }

    bool UiLibCefHelp::LoadCef(const HWND &parent_handle,UiLibCefMsgHandler* pUiLibCefMsgHandler, const std::wstring &url, const RECT &rect, const bool &bUseBrowserMenu /* = false */)
    {
#if 0
        bool ret = false;
        do
        {
            if (NULL == hwnd)
            {
                break;
            }
            if (m_browser_handler.get() == nullptr)
            {
                m_browser_handler = new BrowserHandler;
                //m_browser_handler->SetHostWindow(hwnd);
                m_browser_handler->SetUseBrowserMenu(bUseBrowserMenu);
            }

            m_browser_handler->SetHostWindow(hwnd);

            CefWindowInfo window_info;
            //window_info.SetAsWindowless(hwnd, false);
            window_info.SetAsChild(hwnd, rect);
            CefBrowserSettings browser_settings;
            browser_settings.application_cache = STATE_DISABLED;

            BOOL bSucced = CefBrowserHost::CreateBrowser(window_info
                , m_browser_handler
                , url //_T("http://www.baidu.com/")
                , browser_settings
                , NULL
                , NULL);

            if (!m_js_bridge.get())
            {
                m_js_bridge.reset(new CefJSBridge);
            }
            ret = true;
        } while (false);
        return ret;
#else
        CefBrowserSettings browser_settings;
        browser_settings.application_cache = STATE_DISABLED;

        if (NULL == mpCef3HelpHandler)
        {
            mpCef3HelpHandler = new JGW_Cef3HelpHandler(new CefHandlerDelegate(pUiLibCefMsgHandler),m_is_enable_offset_render);
        }

        //REQUIRE_MAIN_THREAD();
        CefWindowInfo window_info;
        //RECT wnd_rect = {rect.x, rect.y, rect.x + rect.width, rect.y + rect.height};
        window_info.SetAsChild(parent_handle, rect);

        if (GetWindowLongPtr(parent_handle, GWL_EXSTYLE) & WS_EX_NOACTIVATE) {
            // Don't activate the browser window on creation.
            window_info.ex_style |= WS_EX_NOACTIVATE;
        }

        CefBrowserHost::CreateBrowser(window_info, mpCef3HelpHandler,
            url/*client_handler_->startup_url()*/, browser_settings,
            NULL,NULL /*request_context*/);

        return false;
#endif
    }

    void UiLibCefHelp::UnInitialize()
    {
        mpCef3HelpHandler->CloseAllBrowser(true);
        CefShutdown();
    }

	void UiLibCefHelp::CloseBrowserToId(int id,const bool &bForceClose /* = false */ )
	{
		if (NULL != mpCef3HelpHandler)
		{
			mpCef3HelpHandler->CloseBrowserToBrowserId(id,bForceClose);
		}
	}

	void UiLibCefHelp::CloseAllBrowser(const bool &bForceClose /* = false */)
	{
		if (NULL != mpCef3HelpHandler)
		{
			mpCef3HelpHandler->CloseAllBrowser(bForceClose);
		}
	}

	void UiLibCefHelp::QuitCefQuitMessageLoop()
	{
		if (NULL != mpCef3HelpHandler)
		{
			mpCef3HelpHandler->QuitCefQuitMessageLoop();
		}
	}

    void UiLibCefHelp::SizeMsgHandler(RECT rect)
    {
        if (NULL != mpCef3HelpHandler)
        {
            mpCef3HelpHandler->SizeMsgHandler(rect);
        }
    }

    HWND UiLibCefHelp::GetBrowserWindowHandleToId(int id)
    {
        if (NULL != mpCef3HelpHandler)
        {
            return mpCef3HelpHandler->GetBrowserWindowHandleToId(id);  
        }
        return NULL;
    }

    void UiLibCefHelp::ExecuteJavaScriptToId(int id,const std::wstring& strJsCode)
    {
        if (NULL != mpCef3HelpHandler)
        {
            mpCef3HelpHandler->ExecuteJavaScriptToId(id,strJsCode);
        }
    }

    void UiLibCefHelp::RegisterCppFunc(JsCallCppFuncBK pJsCallCppFuncBK)
    {
        JGW_Cef3MessageRouterHandler::RegisterCppFunc(pJsCallCppFuncBK);
    }

    void UiLibCefHelp::UnRegisterCppFunc(JsCallCppFuncBK pJsCallCppFuncBK)
    {
        JGW_Cef3MessageRouterHandler::UnRegisterCppFunc(pJsCallCppFuncBK);
    }
}
