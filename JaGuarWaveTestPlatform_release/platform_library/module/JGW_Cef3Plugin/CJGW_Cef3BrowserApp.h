#pragma once
#include <include/cef_app.h>
#include <include/cef_browser_process_handler.h>

namespace JGW
{
    //! CefBrowser是浏览器窗口类，相当于浏览器的外壳框架窗口，包含向前、向后、加载、获取内部frame的等方法。调用CefBrowserHost的静态方法创建一个CefBrowser对象，表示一个网页窗口。
    //! Browser（浏览器）进程
    //! Browser被定义为主进程，负责窗口管理，网络请求，网页管理 、网络交互
    //! TID_UI 线程是浏览器的主线程。如果应用程序在调用调用CefInitialize()时，传递CefSettings.multi_threaded_message_loop=false，这个线程也是应用程序的主线程。
    //! TID_IO 线程主要负责处理IPC消息以及网络通信。
    //! TID_FILE 线程负责与文件系统交互。
    /*
    TID_RENDERER	渲染主线程	OnRenderProcessThreadCreated
    TID_IO	处理网络和IPC消息	不适用
    TID_FILE	文件系统操作	不适用
    */
#if 0
    CefRefPtr<CefBrowser> browser = ...;
    CefRefPtr<CefFrame> frame = browser->GetMainFrame();
    frame->ExecuteJavaScript("alert('ExecuteJavaScript works!');", frame->GetURL(), 0);
#endif
    class CCJGW_Cef3BrowserApp : public CefApp,public CefBrowserProcessHandler
    {
    public:
        CCJGW_Cef3BrowserApp(void);
        ~CCJGW_Cef3BrowserApp(void);
    protected:
        /************************** CefApp ********************************/
        //! CEF3中OnBeforeCommandLineProcessing是用于处理命令行参数的核心回调，其关键特性如下
        //! 在CEF初始化阶段，应用程序启动后立即执行，早于任何浏览器进程或渲染进程的创建。允许开发者在此阶段修改或追加Chromium/CEF专用的命令行参数。
        //! 浏览器进程：process_type参数为空字符串
        //! 渲染进程/其他子进程：process_type会标识进程类型（如"renderer"、"gpu-process"等）
        void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) override;
        //! 在CEF初始化阶段，早于任何进程创建前执行，主要用于向Chromium注册非标准协议（如myapp://）
        //! 必须在‌浏览器进程‌中通过CefApp::OnRegisterCustomSchemes实现
        //! 渲染进程无法直接修改协议注册表
        void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) override;
        //! 属于CefApp接口方法，主要用于处理本地化资源（如字符串、图片等）的加载逻辑。当CEF需要访问.pak资源包时会触发该回调
        //! 仅在‌浏览器进程‌中生效，通过继承CefBrowserProcessHandler实现
        //! 加载加密的本地化资源文件3
        //! 替换Chromium默认UI资源（如按钮图标）
        //! 动态生成内存资源
        //! 一般情况不使用
        CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler() override;
        //! 仅在‌浏览器进程‌中生效，渲染进程调用此方法将返回nullptr
        //! 需在CefInitialize()之前完成实现类注册
        CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;

        /*************************** CefBrowserProcessHandler **********************************/
        //! CEF3中的CefBrowserProcessHandler是Browser进程的核心处理接口，主要负责管理浏览器进程的生命周期和全局行为38。以下是关键功能与实现要点：
        /*
        ‌1. 核心功能‌
        ‌进程初始化‌：通过OnContextInitialized回调执行Browser进程初始化逻辑
        ‌资源管理‌：控制缓存路径、本地化资源等全局配置
        ‌子进程控制‌：定制子进程启动路径（如browser_subprocess_path)

        ‌3. 典型实现示例
        //! 创建首个浏览器窗口
        CefWindowInfo window_info;
        CefBrowserSettings settings;
        CefBrowserHost::CreateBrowser(window_info, new MyClient(), 
        "https://example.com", settings, nullptr);

        ‌4. 集成注意事项‌
        ‌多进程模式‌：需通过CefSettings.single_process=false启用（默认）68
        ‌线程安全‌：所有方法均在Browser进程的TID_UI线程调用5
        ‌与CefApp关系‌：需通过CefApp::GetBrowserProcessHandler()返回实例3

        ‌5. 相关配置参数‌
        在CefSettings中可配置以下关联参数：

        browser_subprocess_path：指定子进程可执行文件路径
        cache_path：设置磁盘缓存位置（影响Cookie持久化
        remote_debugging_port：启用远程调试端口
        完整配置需在CefInitialize前完成。


        CefSettings settings;
        settings.browser_subprocess_path = "subprocess.exe"; // 子进程路径:ml-citation{ref="5,8" data="citationList"}
        settings.cache_path = "cache";      // 缓存目录:ml-citation{ref="8" data="citationList"}
        settings.single_process = false;    // 多进程模式:ml-citation{ref="5" data="citationList"}

        应用根目录/
        ├── main_app.exe          # 主程序
        ├── subprocess.exe        # 子进程（Windows）
        ├── subprocess            # 子进程（Linux/macOS）
        ├── locales/              # 区域化资源
        └── resources/            # CEF资源文件（如cef.pak）:ml-citation{ref="2,7" data="citationList"}
        ‌5. 错误诊断方法‌
        ‌日志监控‌：开启log_severity = LOGSEVERITY_VERBOSE查看进程启动日志11
        ‌常见错误‌：
        路径错误：日志提示Failed to launch subprocess10
        资源缺失：渲染白屏（检查cef.pak是否部署）
        */

        //! Browser进程上下文就绪时触发，通常在此创建浏览器窗口
        //! OnContextInitialized是CEF3中CefBrowserProcessHandler的关键回调函数，在Browser进程上下文初始化完成后触发
        /*
        ‌1. 触发时机‌
        在CefInitialize()执行后，Browser进程完成基础初始化时调用
        仅Browser进程触发（Render进程不会调用）

        ‌窗口创建‌：主线程安全创建浏览器窗口的首选位置
        ‌全局配置‌：初始化Cookie管理器、注册自定义协议等
        ‌资源加载控制‌：通过browser_settings禁用图片/本地存储等

        ‌3. 实现要求‌
        必须继承CefBrowserProcessHandler并重写该方法
        需通过CefApp::GetBrowserProcessHandler()返回实例
        所有操作必须在‌TID_UI线程‌执行（CEF主线程）
        */
        void OnContextInitialized() override;
        //! 启动子进程前修改命令行参数
        /*
        OnBeforeChildProcessLaunch是CEF3中用于控制子进程启动的关键回调函数，其核心机制和用法如下：
        ‌1. 功能定位‌
        ‌进程控制‌：在浏览器进程启动子进程（渲染进程/GPU进程等）前触发，允许修改启动参数或环境变量
        ‌安全沙箱‌：可通过修改命令行参数禁用沙箱（--no-sandbox）或调整资源限制
        ‌调试支持‌：添加--enable-logging等参数以捕获子进程日志

        ‌禁用GPU加速‌	--disable-gpu	解决兼容性问题
        ‌启用远程调试‌	--remote-debugging-port=9222	配合DevTools调试
        ‌自定义沙箱策略‌	--no-sandbox	关闭安全沙箱（需权衡安全性）
        */
        void OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line) override;

        //! OnRenderProcessThreadCreated是CEF3渲染进程(Renderer Process)初始化时的关键回调函数，主要用于线程级别的资源初始化。其核心机制如下：
        /*
        ‌1. 触发时机‌
        ‌线程创建时调用‌：当CEF3创建渲染进程的主线程（通常是TID_RENDERER）后立即触发
        ‌单次执行‌：每个渲染进程生命周期内仅调用一次
        ‌执行顺序‌：早于OnWebKitInitialized和OnContextCreated等渲染相关回调

        extra_info参数允许Browser进程向Renderer进程传递初始化数据

        在为新的渲染进程创建主线程后在浏览器进程IO线程上调用。提供了一个指定额外信息的机会，这些信息将在渲染过程中传递给CefRenderProcessHandler::OnRenderThreadCreated（）。不要在这个方法之外保留对|extra_info|的引用。
        */
        void OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info) override;

        //! OnScheduleMessagePumpWork是CEF3中用于自定义消息泵调度的关键回调函数，其核心机制如下：
        //! 在MFC中集成CEF3的消息泵需要重写CefBrowserProcessHandler::OnScheduleMessagePumpWork并适配MFC消息循环，具体实现如下：
        /*
        ‌CEF初始化设置
        CefSettings settings;
        settings.external_message_pump = true;  // 启用外部消息泵控制:ml-citation{ref="2" data="citationList"}
        settings.multi_threaded_message_loop = false;  // 禁用CEF独立消息循环:ml-citation{ref="2" data="citationList"}
        必须关闭multi_threaded_message_loop以兼容MFC主线程

        class MfcCefHandler : public CefBrowserProcessHandler {
        public:
        void OnScheduleMessagePumpWork(int64 delay_ms) override {
        if (delay_ms <= 0) {
        PostMessage(hMessageWnd, WM_CEF_WORK, 0, 0);  // 立即触发消息处理:ml-citation{ref="6,10" data="citationList"}
        } else {
        SetTimer(hMessageWnd, TIMER_ID, delay_ms, nullptr);  // 延迟处理:ml-citation{ref="6" data="citationList"}
        }
        }
        };
        通过Windows消息机制调度CefDoMessageLoopWork

        BOOL CMyApp::PumpMessage() {
        BOOL result = CWinApp::PumpMessage();
        CefDoMessageLoopWork();  // 每次MFC消息循环后处理CEF任务:ml-citation{ref="1,10" data="citationList"}
        return result;
        }

        BEGIN_MESSAGE_MAP(CMyApp, CWinApp)
        ON_MESSAGE(WM_CEF_WORK, &CMyApp::OnCefWork)
        END_MESSAGE_MAP()

        LRESULT CMyApp::OnCefWork(WPARAM, LPARAM) {
        CefDoMessageLoopWork();
        return 0;
        }

        ‌线程安全‌

        所有CEF调用必须发生在主UI线程（MFC主线程）46
        跨线程操作需通过PostMessage转发
        高频调用CefDoMessageLoopWork可能增加CPU负载，建议通过delay_ms动态调整频率
        */
        void OnScheduleMessagePumpWork(int64 delay_ms) override;
    public:
        void SwitchThreadTask(void (*TaskCallback)(),CefThreadId threadId = TID_UI);
    public:
        // Include the default reference counting implementation.
        IMPLEMENT_REFCOUNTING(CCJGW_Cef3BrowserApp);
        DISALLOW_COPY_AND_ASSIGN(CCJGW_Cef3BrowserApp);
    };
}
