// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <JGW_Cef3Plugin/JGW_Cef3PluginExport.h>
#include <include/cef_app.h>
#include <JGW_Cef3Plugin/CJGW_Cef3RendererApp.h>
#include <set>
#include "JGW_ZipResResourceProvider.h"
#include <include/wrapper/cef_stream_resource_handler.h>
#include "JGW_Cef3MessageRouterHandler.h"
#include "CJGW_Cef3BrowserApp.h"
#include "CJGW_Cef3ClientHandler.h"

const char kTestHost[] = "tests";
const char kLocalHost[] = "localhost";
const char kTestOrigin[] = "http://tests/";

// Pages handled via StringResourceProvider.
const char kTestGetSourcePage[] = "get_source.html";
const char kTestGetTextPage[] = "get_text.html";
const char kTestPluginInfoPage[] = "plugin_info.html";

// Provider that returns string data for specific pages. Used in combination
// with LoadStringResourcePage().
class StringResourceProvider : public CefResourceManager::Provider {
public:
    StringResourceProvider(const std::set<std::string>& pages,
        StringResourceMap* string_resource_map)
        : pages_(pages), string_resource_map_(string_resource_map) {
            DCHECK(!pages.empty());
    }

    bool OnRequest(scoped_refptr<CefResourceManager::Request> request) OVERRIDE {
        CEF_REQUIRE_IO_THREAD();
        //! file:///G:/CEF/cef_binary_83.5.0+gbf03589+chromium-83.0.4103.106_windows32/tests/cefclient/resources/dialogs.html
        const std::string& url = request->url();
#if 0
        if (url.find(kTestOrigin) != 0U) {
            // Not handled by this provider.
            return false;
        }

        const std::string& page = url.substr(strlen(kTestOrigin));
        if (pages_.find(page) == pages_.end()) {
            // Not handled by this provider.
            return false;
        }

        std::string value;
        StringResourceMap::const_iterator it = string_resource_map_->find(page);
        if (it != string_resource_map_->end()) {
            value = it->second;
        } else {
            value = "<html><body>No data available</body></html>";
        }
#else
        //! client://dialogs/
        if (url.find("client://") != 0U) 
        {
            // Not handled by this provider.
            return false;
        }
        const std::string& page = url.substr(strlen("client://"));
        std::string value = page;

#endif
        CefRefPtr<CefStreamReader> response = CefStreamReader::CreateForData(
            static_cast<void*>(const_cast<char*>(value.c_str())), value.size());

        request->Continue(new CefStreamResourceHandler(
            200, "OK", "text/html", CefResponse::HeaderMap(), response));
        return true;
    }

private:
    const std::set<std::string> pages_;

    // Only accessed on the IO thread.
    StringResourceMap* string_resource_map_;

    DISALLOW_COPY_AND_ASSIGN(StringResourceProvider);
};

bool SetZipResResource(HINSTANCE hModuleHandle, DWORD dwResourceNameID, const wchar_t* strResourceType,const wchar_t* szPassword /*= NULL*/)
{
    return JGW::JGW_ZipResResourceProvider::SetZipResResource(hModuleHandle,dwResourceNameID,strResourceType,szPassword);
}

bool SetZipFilePath(const wchar_t* strZipFilePath,const wchar_t* szPassword /*= NULL*/)
{
    return JGW::JGW_ZipResResourceProvider::SetZipFilePath(strZipFilePath,szPassword);
}

void SetupResourceManager(CefRefPtr<CefResourceManager> resource_manager,StringResourceMap* string_resource_map)
{
    // Set of supported string pages.
    std::set<std::string> string_pages;
    string_pages.insert(kTestGetSourcePage);
    string_pages.insert(kTestGetTextPage);
    string_pages.insert(kTestPluginInfoPage);

    //! Add provider for string resources.
    resource_manager->AddProvider(new StringResourceProvider(string_pages, string_resource_map), 0,std::string());
    //! Add provider for string resources.
    resource_manager->AddProvider(new JGW::JGW_ZipResResourceProvider(),0,std::string());
}

void CreateMessageHandlers(MessageHandlerSet& handlers)
{
    handlers.insert(new JGW::JGW_Cef3MessageRouterHandler());
}

int ExecuteSubProcesses(HINSTANCE hInstance)
{
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
}


void GetCefSetting(bool bMultiThreadedMessageLoop,const std::wstring &cef_temp_path,const wchar_t* strSubProcessesPath, CefSettings &settings)
{
    // --no-sandbox非沙箱模式
    settings.no_sandbox = true;

    // cache_path缓存数据将存储在磁盘上的位置。如果为空，内存缓存将用于某些功能，而临时磁盘缓存将用于其他功能。如果指定了缓存路径，HTML5 数据库（如 localStorage）将仅在会话中持久存在。
    // 设置localstorage，不要在路径末尾加"\\"，否则运行时会报错
    CefString(&settings.cache_path) = cef_temp_path + L"cef_temp\\CefLocalStorage";
    CefString(&settings.root_cache_path) = cef_temp_path + L"cef_temp";
    // log_file用于调试日志的目录和文件名。如果为空，将使用默认名称“debug.log”并将文件写入应用程序目录。也可使用“日志文件”命令行开关进行配置。
    // 设置debug log文件位置
    CefString(&settings.log_file) = cef_temp_path + L"cef_temp\\cef.log";

    settings.log_severity = LOGSEVERITY_ERROR;  // 日志级别:ml-citation{ref="1" data="citationList"}
    //settings.log_file = "debug.log";   

    // multi_threaded_message_loop设置为 true 让浏览器处理消息循环在单独的线程中运行。有关详细信息，请参阅“消息循环集成”部分。
    // cef2623、2526版本debug模式:在使用multi_threaded_message_loop时退出程序会触发中断
    // 加入disable-extensions参数可以修复这个问题，但是会导致一些页面打开时报错
    // 开启Cef多线程消息循环，兼容nbase库消息循环
    // 定期调用 CefDoMessageLoopWork() 而不是调用 CefRunMessageLoop()。每次调用 CefDoMessageLoopWork() 都会执行一次 CEF 消息循环迭代。应谨慎使用此方法。过于频繁地调用该方法会使 CEF 消息循环耗尽并对浏览器性能产生负面影响。过于频繁地调用该方法会对 CPU 使用率产生负面影响。有关高级用法的详细信息，请参阅CefBrowserProcessHandler::OnScheduleMessagePumpWork。您可以在 cefclient 中通过使用“--external-message-pump”命令行标志运行来测试此模式。
    // 设置 CefSettings.multi_threaded_message_loop = true（仅限 Windows 和 Linux）。这将导致 CEF 在与主应用程序线程不同的线程上运行浏览器 UI 线程。使用这种方法，既不需要调用 CefDoMessageLoopWork() 也不需要调用 CefRunMessageLoop()。CefInitialize() 和 CefShutdown() 仍应在主应用程序线程上调用。您将需要提供自己的机制来与主应用程序线程通信（例如，请参阅 cefclient_win.cpp 中的消息窗口用法）。您可以在 Windows 或 Linux 上的 cefclient 中通过使用“--multi-threaded-message-loop”命令行标志运行来测试此模式。
    settings.multi_threaded_message_loop = bMultiThreadedMessageLoop;

    // browser_subprocess_path将为子进程启动的单独可执行文件的路径。有关详细信息，请参阅“单独的子流程可执行文件”部分。
    //settings.browser_subprocess_path;
    // Specify the path for the sub-process executable.
    //CefString(&settings.browser_subprocess_path).FromASCII(“ / path / to / subprocess”);
    // Specify the path for the sub-process executable.
    // 指定子进程的执行路径
    if (NULL != strSubProcessesPath)
    {
        CefString(&settings.browser_subprocess_path).FromWString(strSubProcessesPath);
    }

    // command_line_args_disabled设置为 true 以禁用使用标准 CEF 和 Chromium 命令行参数配置浏览器进程功能。有关详细信息，请参阅“命令行参数”部分。

    // 开启离屏渲染
    settings.windowless_rendering_enabled = false;
}

bool InitializeCef3Processes(HINSTANCE hInstance,bool bMultiThreadedMessageLoop,const wchar_t* strSubProcessesPath,const std::wstring& cef_temp_path)
{
#if 1
    //启用高分辨率，启动进程
    CefEnableHighDPISupport();
    // Structure for passing command-line arguments.
    // The definition of this structure is platform-specific.
    // 传递命令行参数的结构体。
    // 这个结构体的定义与平台相关。
    CefMainArgs main_args(hInstance);
    // Optional implementation of the CefApp interface.

    // 可选择性地实现CefApp接口
    CefRefPtr<JGW::CCJGW_Cef3BrowserApp> app(new JGW::CCJGW_Cef3BrowserApp);
    // Populate this structure to customize CEF behavior.

    // 填充这个结构体，用于定制CEF的行为。
    CefSettings settings;
    // Initialize CEF in the main process.
    GetCefSetting(bMultiThreadedMessageLoop,cef_temp_path,strSubProcessesPath, settings);
    //CefString(&settings.browser_subprocess_path).FromWString(L"G:\\git_code_debug\\FTM-tools\\JaGuarWaveTestPlatform\\bin\\debug\\exec_debug\\cef3\\JGW_Cef3SubProcesses.exe");

    // 在主进程中初始化CEF
    return CefInitialize(main_args, settings, app.get(),NULL);
#else

#if 1
    std::wstring cef_temp_path  = L"cef_temp";
    bool ret = false;

    //启用高分辨率，启动进程
    CefEnableHighDPISupport();

    //m_is_enable_offset_render = false;
    CefSettings settings;
    CefMainArgs main_args(GetModuleHandle(NULL));
    CefRefPtr<JGW::CCJGW_Cef3BrowserApp> app(new JGW::CCJGW_Cef3BrowserApp());

    // 指定子进程的执行路径
    if (NULL != strSubProcessesPath)
    {
        CefString(&settings.browser_subprocess_path).FromWString(strSubProcessesPath);
    }

    //! 子进程（渲染进程）主进程浏览器进程
    // 如果是在子进程中调用，会堵塞直到子进程退出，并且exit_code返回大于等于0
    // 如果在Browser进程中调用，则立即返回-1
    int exit_code = CefExecuteProcess(main_args, app.get(), NULL);
    if (exit_code >= 0)  return false;

    GetCefSetting(cef_temp_path, settings);
    ret = CefInitialize(main_args, settings, app.get(), NULL);


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
#endif
}

CefRefPtr<JGW::CCJGW_Cef3ClientHandler> ptrCef3ClientHandler;

void CloseAllBrowsers(bool force_close)
{
    if (ptrCef3ClientHandler.get())
    {
        ptrCef3ClientHandler->CloseAllBrowsers(force_close);
    }
}

bool IsCloseCef3Browser()
{
    if (ptrCef3ClientHandler.get())
    {
        return ptrCef3ClientHandler->IsClosing();
    }
    return true;
}

void UnInitializeCef3Processes()
{
    // Shut down CEF.
    // 关闭CEF
    CefShutdown();
}

void RegisterCppFuncToJs(const wchar_t* strFuntionName,pCppFuncCallback pCppFunc)
{
    if (ptrCef3ClientHandler.get())
    {
        ptrCef3ClientHandler->RegisterCppFuncToJs(strFuntionName,pCppFunc);
    }
}

int GetBrowserIdToUrl(const wchar_t* strUrl)
{
    if (ptrCef3ClientHandler.get())
    {
        return ptrCef3ClientHandler->GetBrowserIdToUrl(strUrl);
    }
    return -1;
}

bool ExecuteJaveScript(int nBrowserId,const std::wstring& strJsCode)
{
    if (ptrCef3ClientHandler.get())
    {
        return ptrCef3ClientHandler->ExecuteJaveScript(nBrowserId,strJsCode);
    }
    return false;
}

void AddBrowserEventDelegate(JGW::CCJGW_Cef3BrowserEventDelegate* pCef3BrowserEventDelegate)
{
     if (ptrCef3ClientHandler.get())
     {
         ptrCef3ClientHandler->AddBrowserEventDelegate(pCef3BrowserEventDelegate);
     }
}

void RemoveBrowserEventDelegate(JGW::CCJGW_Cef3BrowserEventDelegate* pCef3BrowserEventDelegate)
{
    if (ptrCef3ClientHandler.get())
    {
        ptrCef3ClientHandler->RemoveBrowserEventDelegate(pCef3BrowserEventDelegate);
    }
}

void CreateBrowserHelp(HWND parent_handle,RECT rect,const wchar_t* strUrl,int nMainIconResId /*= 107*/)
{
    gnMainIconResId = nMainIconResId;
    if (!ptrCef3ClientHandler.get())
    {
        ptrCef3ClientHandler = new JGW::CCJGW_Cef3ClientHandler();
    }
    

    if (NULL != parent_handle)
    {
        CefBrowserSettings browser_settings;
        browser_settings.application_cache = STATE_DISABLED;
        CefWindowInfo window_info;
        window_info.SetAsChild(parent_handle, rect);

        if (GetWindowLongPtr(parent_handle, GWL_EXSTYLE) & WS_EX_NOACTIVATE) 
        {
            // Don't activate the browser window on creation.
            window_info.ex_style |= WS_EX_NOACTIVATE;
        }

        CefBrowserHost::CreateBrowser(window_info, ptrCef3ClientHandler,
            strUrl/*client_handler_->startup_url()*/, browser_settings,
            NULL,NULL /*request_context*/);
    }
    else
    {
        // SimpleHandler implements browser-level callbacks.
        

        // Specify CEF browser settings here.
        CefBrowserSettings browser_settings;
        CefWindowInfo window_info;

#if defined(OS_WIN)
        // On Windows we need to specify certain flags that will be passed to
        // CreateWindowEx().
        window_info.SetAsPopup(NULL, "cefsimple");
#endif

        // Create the first browser window.
        CefBrowserHost::CreateBrowser(window_info,ptrCef3ClientHandler, strUrl, browser_settings, nullptr, nullptr);
#if 0
        CefWindowInfo window_info;
        CefBrowserSettings browser_settings;
        browser_settings.application_cache = STATE_DISABLED;
        CefBrowserHost::CreateBrowser(window_info, new JGW::CCJGW_Cef3ClientHandler(),
            strUrl/*client_handler_->startup_url()*/, browser_settings,
            NULL,NULL /*request_context*/);
#endif
    }
    
}

BOOL APIENTRY DllMain( HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
    )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

