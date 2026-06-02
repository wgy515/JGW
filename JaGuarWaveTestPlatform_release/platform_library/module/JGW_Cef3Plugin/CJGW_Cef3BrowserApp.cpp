#include "StdAfx.h"
#include "CJGW_Cef3BrowserApp.h"
#include <include/wrapper/cef_closure_task.h>
#include <include/base/cef_bind.h>
#include "CJGW_Cef3ClientHandler.h"

const char kMultiThreadedMessageLoop[] = "multi-threaded-message-loop";
const char kExternalMessagePump[] = "external-message-pump";
const char kCachePath[] = "cache-path";
const char kUrl[] = "url";
const char kOffScreenRenderingEnabled[] = "off-screen-rendering-enabled";
const char kOffScreenFrameRate[] = "off-screen-frame-rate";
const char kTransparentPaintingEnabled[] = "transparent-painting-enabled";
const char kShowUpdateRect[] = "show-update-rect";
const char kSharedTextureEnabled[] = "shared-texture-enabled";
const char kExternalBeginFrameEnabled[] = "external-begin-frame-enabled";
const char kMouseCursorChangeDisabled[] = "mouse-cursor-change-disabled";
const char kOffline[] = "offline";
const char kRequestContextPerBrowser[] = "request-context-per-browser";
const char kRequestContextSharedCache[] = "request-context-shared-cache";
const char kBackgroundColor[] = "background-color";
const char kEnableGPU[] = "enable-gpu";
const char kFilterURL[] = "filter-url";
const char kUseViews[] = "use-views";
const char kHideFrame[] = "hide-frame";
const char kHideControls[] = "hide-controls";
const char kAlwaysOnTop[] = "always-on-top";
const char kHideTopMenu[] = "hide-top-menu";
const char kWidevineCdmPath[] = "widevine-cdm-path";
const char kSslClientCertificate[] = "ssl-client-certificate";
const char kCRLSetsPath[] = "crl-sets-path";
const char kLoadExtension[] = "load-extension";
const char kNoActivate[] = "no-activate";

namespace JGW
{
    CCJGW_Cef3BrowserApp::CCJGW_Cef3BrowserApp(void)
    {
    }


    CCJGW_Cef3BrowserApp::~CCJGW_Cef3BrowserApp(void)
    {
    }

    void CCJGW_Cef3BrowserApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
    {
#if 1
        if (process_type.empty())
        {
            //! 启用系统Flash插件：
            command_line->AppendSwitch("enable-system-flash");
            //! 设置PPAPI插件路径：
            command_line->AppendSwitchWithValue("ppapi-flash-path", "pepflashplayer.dll");
#ifdef _DEBUG__
            //! 强制单进程模式
            command_line->AppendSwitch("single-process");
#endif
            //同一个域下的使用同一个渲染进程
            command_line->AppendSwitch("process-per-site");
            command_line->AppendSwitch("disable-gpu");
            command_line->AppendSwitch("disable-gpu-compositing");

            //command_line->AppendSwitch("disable-gpu-vsync");
            //command_line->AppendSwitch("enable-gpu-rasterization");

            //! 通过CefSettings.command_line_args_disabled可禁用默认参数
            //! 通过CefSettings设置的等效命令行参数会先于该回调执行

            //! 在启用离屏呈现时传递额外的命令行标志。
            if (command_line->HasSwitch(kOffScreenRenderingEnabled) && !command_line->HasSwitch(kSharedTextureEnabled))
            {
                if (!command_line->HasSwitch(kEnableGPU))
                {
                    //! 使用软件渲染和合成（禁用GPU）增加FPS和减少CPU使用。这也会禁用WebGL，所以如果你需要这个功能，请删除这些开关。详见https://bitbucket.org/chromiumembedded/cef/issues/1257。
                    command_line->AppendSwitch("disable-gpu");
                    command_line->AppendSwitch("disable-gpu-compositing");
                }
            }

            if (command_line->HasSwitch(kUseViews) && !command_line->HasSwitch("top-chrome-md"))
            {
                //! 默认情况下在所有平台上使用non-material模式。除此之外，这会导致菜单按钮显示悬停状态。参见Chromium代码中MaterialDesignController::IsModeMaterial（）的用法。
                command_line->AppendSwitchWithValue("top-chrome-md", "non-material");
            }


            if (!command_line->HasSwitch(kCachePath) && !command_line->HasSwitch("disable-gpu-shader-disk-cache"))
            {
                //! 当cache-path未指定时，不要创建“GPUCache”目录。
                command_line->AppendSwitch("disable-gpu-shader-disk-cache");
            } 
        }
#endif
    }

    void CCJGW_Cef3BrowserApp::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
    {
#if 1
        //! 其中CEF_SCHEME_OPTION标志控制协议行为（如是否支持CORS）
        registrar->AddCustomScheme("myapp", CEF_SCHEME_OPTION_STANDARD | CEF_SCHEME_OPTION_SECURE);
        //! client://
        registrar->AddCustomScheme("client", CEF_SCHEME_OPTION_STANDARD | CEF_SCHEME_OPTION_CORS_ENABLED);
#endif
    }

    CefRefPtr<CefResourceBundleHandler> CCJGW_Cef3BrowserApp::GetResourceBundleHandler()
    {
        return nullptr;
    }

    CefRefPtr<CefBrowserProcessHandler> CCJGW_Cef3BrowserApp::GetBrowserProcessHandler()
    {
        return this;
    }



    void CCJGW_Cef3BrowserApp::OnContextInitialized()
    {
#if 0
        //! TID_UI 主线程运行
        if (!CefCurrentlyOn(TID_UI)) 
        {
            // Execute on the browser UI thread.
            CefPostTask(TID_UI, base::Bind(RunManifestCallback, callback, manifest));
            return;
        }
        callback.Run(manifest);
#endif
#if 0
        // 创建首个浏览器窗口
        CefWindowInfo window_info;
        CefBrowserSettings settings;
        CefBrowserHost::CreateBrowser(window_info, new CCJGW_Cef3ClientHandler(),  L"file:///G:/CEF/cef_binary_83.5.0+gbf03589+chromium-83.0.4103.106_windows32/tests/cefclient/resources/dialogs.html", settings, nullptr,NULL);
        //CefBrowserHost::CreateBrowser(window_info, new CCJGW_Cef3ClientHandler(),  L"www.baidu.com", settings, nullptr,NULL);
#endif
#if 0
        CEF_REQUIRE_UI_THREAD();

        CefRefPtr<CefCommandLine> command_line =
            CefCommandLine::GetGlobalCommandLine();

#if defined(OS_WIN) || defined(OS_LINUX)
        // Create the browser using the Views framework if "--use-views" is specified
        // via the command-line. Otherwise, create the browser using the native
        // platform framework. The Views framework is currently only supported on
        // Windows and Linux.
        const bool use_views = command_line->HasSwitch("use-views");
#else
        const bool use_views = false;
#endif

        // SimpleHandler implements browser-level callbacks.
        CefRefPtr<CCJGW_Cef3ClientHandler> handler(new CCJGW_Cef3ClientHandler());

        // Specify CEF browser settings here.
        CefBrowserSettings browser_settings;

        std::string url;

        // Check if a "--url=" value was provided via the command-line. If so, use
        // that instead of the default URL.
        url = command_line->GetSwitchValue("url");
        if (url.empty())
            url = "http://www.baidu.com";

        //if (use_views) {
        //    // Create the BrowserView.
        //    CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(
        //        handler, url, browser_settings, nullptr, nullptr,
        //        new SimpleBrowserViewDelegate());

        //    // Create the Window. It will show itself after creation.
        //    CefWindow::CreateTopLevelWindow(new SimpleWindowDelegate(browser_view));
        //} else {
            // Information used when creating the native window.
            CefWindowInfo window_info;

#if defined(OS_WIN)
            // On Windows we need to specify certain flags that will be passed to
            // CreateWindowEx().
            window_info.SetAsPopup(NULL, "cefsimple");
#endif

            // Create the first browser window.
            CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings,
                nullptr, nullptr);
        //}
#endif
    }

    void CCJGW_Cef3BrowserApp::OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line)
    {
#if 0
        // 添加调试参数
        command_line->AppendSwitch("--enable-logging");
        command_line->AppendSwitchWithValue("--v", "1");

        // 禁用沙箱（仅测试环境使用）
#ifdef DEBUG
        command_line->AppendSwitch("--no-sandbox");
#endif
#endif
#if 0
        command_line->AppendSwitch("--disable-gpu");
        command_line->AppendArgument("--my-custom-flag=value");
#endif
        command_line->AppendSwitch("--disable-gpu-vsync");
        command_line->AppendSwitch("--enable-gpu-rasterization");
       //--disable-gpu-vsync --enable-gpu-rasterization
    }


    void CCJGW_Cef3BrowserApp::OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info)
    {
#if 0
        // Browser进程传递参数
        CefRefPtr<CefListValue> extra_info = CefListValue::Create();
        extra_info->SetInt(0, 9222);  // 远程调试端口
#endif
    }

    void CCJGW_Cef3BrowserApp::OnScheduleMessagePumpWork(int64 delay_ms)
    {

    }
















































    void CCJGW_Cef3BrowserApp::SwitchThreadTask(void (*TaskCallback)(),CefThreadId threadId /* = TID_UI */)
    {
#if 0
        // Render进程中发送消息
        CefPostTask(TID_UI, 
            NewCefRunnableFunction(‌[]->{
                // 在Browser主线程执行操作
        }));


#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"

        class MyHandler : public CefBaseRefCounted {
        public:
            void SendToUIThread() {
                // 从IO线程投递任务到UI线程
                CefPostTask(TID_UI, 
                    base::Bind(&MyHandler::OnUIThread, this, "Data"));
            }

        private:
            void OnUIThread(const std::string& data) {
                DCHECK(CefCurrentlyOn(TID_UI));
                // UI线程处理逻辑
            }
        };
        //! 此例演示了如何安全地从IO线程通知UI线程处理数据
        // 定义处理类
        class DataProcessor : public CefBaseRefCounted {
        public:
            void OnDataReceived(const std::string& data) {
                DCHECK(CefCurrentlyOn(TID_UI));
                // 处理数据
            }
        };

        // 投递任务
        void SendToUIThread(CefRefPtr<DataProcessor> processor) {
            CefPostTask(TID_UI,
                CefClosureTask(base::BindOnce(
                &DataProcessor::OnDataReceived,
                processor,
                "Sample Data"
                ))
                );
        }

        // 绑定全局函数
        CefPostTask(TID_IO, base::Bind(&MyGlobalFunction, arg1, arg2));

        // 绑定成员方法
        CefPostTask(TID_UI, base::Bind(&MyClass::Method, object_ptr, arg1));
#endif
        //! TID_UI 主线程运行
        if (!CefCurrentlyOn(threadId)) 
        {
            //CefPostTask(TID_UI, base::Bind(&ClientHandler::ShowDevTools, this, browser,inspect_element_at));
            // Execute on the browser UI thread.
            CefPostTask(threadId, base::Bind(&CCJGW_Cef3BrowserApp::SwitchThreadTask,this, TaskCallback,threadId));
            return;
        }
        TaskCallback();
    }
}
