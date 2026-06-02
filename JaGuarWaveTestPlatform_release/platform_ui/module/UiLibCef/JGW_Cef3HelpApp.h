#pragma once
#include <include/cef_app.h>
#include <include/wrapper/cef_message_router.h>
//#include "JGW_Cef3JsBindCallbackHandler.h"
#include "CefRendererDelegate.h"
#include "CefAppDelegate.h"
#include <set>

namespace JGW
{
    typedef std::map<std::pair<std::string, int>,
        std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value> > >
        CallbackMap;
	/*
	OnWebKitInitialized: 当WebKit（包括其内部的JavaScript引擎V8）初始化完成后被调用。
	OnContextCreated: 当一个新的V8 JavaScript环境（context）为一个特定的frame创建之后被调用。
	OnContextReleased: 在一个frame的V8 JavaScript环境被释放之前被调用。
	OnUncaughtException: 当在任何一个frame中出现未捕获的JavaScript异常时被调用。
	BeginExecuteJavaScript 和 AfterExecuteJavaScript: 这两个你或者针对你的项目添加的自定义函数可以分别在JavaScript开始执行和结束执行时调用，这样可以让你监控和了解到JavaScript的执行情况。
	*/
	// 参考文档https://zhuanlan.zhihu.com/p/630130886
	// CEF3 使用多个进程运行。处理窗口创建、UI 和网络访问的主要进程称为“浏览器”进程。这通常是与主应用程序相同的进程，并且大部分应用程序逻辑将在浏览器进程中运行
	// Blink 渲染和 JavaScript 执行发生在单独的“渲染”进程中。一些应用程序逻辑，例如 JavaScript 绑定和 DOM 访问，也会在渲染进程中运行
	// 默认进程模型将为每个唯一的来源（方案+域，即URL）生成一个新的渲染进程。其他进程将根据需要产生，例如处理加速合成的“gpu”进程。
	// 默认情况下，主应用程序可执行文件将被多次启动为系统中的独立进程。这是通过传递到 CefExecuteProcess 函数的命令行标志来处理的。如果主应用程序可执行文件很大，加载时间很长，或者不适用于非浏览器进程，可以使用单独的可执行文件。这可以通过 CefSettings.browser_subprocess_path 变量进行配置。有关详细信息，请参阅“应用程序结构”部分。

    //! renderer程序继承CefApp和CefRenderProcessHandler类
    //! browserapp要继承CefApp和CefBrowserProcessHandler类
	class JGW_Cef3HelpApp : public CefApp,
		public CefBrowserProcessHandler,
		public CefRenderProcessHandler
	{
	public:
		JGW_Cef3HelpApp();
		~JGW_Cef3HelpApp();
        typedef std::set<CefRefPtr<CefRendererDelegate>> DelegateSet;
        typedef std::set<CefRefPtr<CefAppDelegate>> CefAppDelegateSet;
#if 1
    private:
        // Creates all of the Delegate objects. Implemented by cefclient in
        // client_app_delegates_renderer.cc
        static void CreateCefRendererDelegates(DelegateSet& delegates);

        static void CreateCefAppDelegates(CefAppDelegateSet& delegates);
        // Registers custom schemes. Implemented by cefclient in
        // client_app_delegates_common.cc
        static void RegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar, std::vector<CefString>& cookiable_schemes);
#endif
	public:
#if 1
		/************************** CefApp **************************************************/
		// CefApp是应用程序类，网页嵌入程序都要实现这样一个应用程序类。提供了一些简单的接口：命令行参数修改，主题修改，获取进程句柄等。还有很多的子类，例如SimpleApp、ClientApp等，clientapp又有很多子类ClientAppBrowser、ClientAppRenderer、ClientAppOther，表示不同的应用类型：应用程序类定义如下：
		///
		// Provides an opportunity to view and/or modify command-line arguments before
		// processing by CEF and Chromium. The |process_type| value will be empty for
		// the browser process. Do not keep a reference to the CefCommandLine object
		// passed to this method. The CefSettings.command_line_args_disabled value
		// can be used to start with an empty command-line object. Any values
		// specified in CefSettings that equate to command-line arguments will be set
		// before this method is called. Be cautious when using this method to modify
		// command-line arguments for non-browser processes as this may result in
		// undefined behavior including crashes.
		// 提供在CEF和Chromium处理之前查看和/或修改命令行参数的机会。对于浏览器进程，|process_type|值将为空。不要保留对传递给该方法的CefCommandLine对象的引用。CefSettings.command_line_args_disabled值可用于以空命令行对象启动。在CefSettings中指定的等同于命令行参数的任何值都将在调用此方法之前设置。使用此方法修改非浏览器进程的命令行参数时要小心，因为这可能导致未定义的行为，包括崩溃。
		///
		/*--cef(optional_param=process_type)--*/
		void OnBeforeCommandLineProcessing(
			const CefString& process_type,
			CefRefPtr<CefCommandLine> command_line) OVERRIDE;

		///
		// Provides an opportunity to register custom schemes. Do not keep a reference
		// to the |registrar| object. This method is called on the main thread for
		// each process and the registered schemes should be the same across all
		// processes.
		// 提供注册自定义方案的机会。不要保留对|注册器|对象的引用。该方法在每个进程的主线程上被调用，注册的方案在所有进程中应该是相同的。
		///
		/*--cef()--*/
		void OnRegisterCustomSchemes(
			CefRawPtr<CefSchemeRegistrar> registrar) OVERRIDE;

		///
		// Return the handler for resource bundle events. If
		// CefSettings.pack_loading_disabled is true a handler must be returned. If no
		// handler is returned resources will be loaded from pack files. This method
		// is called by the browser and render processes on multiple threads.
		// 返回资源束事件的处理程序。如果CefSettings。Pack_loading_disabled为true，必须返回处理程序。如果没有返回处理程序，将从包文件加载资源。此方法由浏览器调用，并在多个线程上呈现进程。
		///
		/*--cef()--*/
		CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler() OVERRIDE {
			return nullptr;
		}

		///
		// Return the handler for functionality specific to the browser process. This
		// method is called on multiple threads in the browser process.
		///
		/*--cef()--*/
		CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE {
			return this;
		}

		///
		// Return the handler for functionality specific to the render process. This
		// method is called on the render process main thread.
		///
		/*--cef()--*/
		CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE {
			return this;
		}
#endif  //! CefApp


#if 1
		/*********************** CefBrowserProcessHandler *****************************************************/
		// 
		///
		// Called on the browser process UI thread immediately after the CEF context
		// has been initialized.
		// 在CEF上下文初始化后立即在浏览器进程UI线程上调用。
		///
		/*--cef()--*/
		void OnContextInitialized() OVERRIDE;

		///
		// Called before a child process is launched. Will be called on the browser
		// process UI thread when launching a render process and on the browser
		// process IO thread when launching a GPU or plugin process. Provides an
		// opportunity to modify the child process command line. Do not keep a
		// reference to |command_line| outside of this method.
		// 在启动子进程之前调用。当启动渲染进程时，在浏览器进程UI线程上调用；当启动GPU或插件进程时，在浏览器进程IO线程上调用。提供修改子进程命令行的机会。不要在这个方法之外保留对|command_line|的引用。
		///
		/*--cef()--*/
		void OnBeforeChildProcessLaunch(
			CefRefPtr<CefCommandLine> command_line) OVERRIDE;
        ///
        // Called on the browser process IO thread after the main thread has been
        // created for a new render process. Provides an opportunity to specify extra
        // information that will be passed to
        // CefRenderProcessHandler::OnRenderThreadCreated() in the render process. Do
        // not keep a reference to |extra_info| outside of this method.
        ///
        /*--cef()--*/
        void OnRenderProcessThreadCreated(
            CefRefPtr<CefListValue> extra_info) OVERRIDE;

        ///
        // Return the handler for printing on Linux. If a print handler is not
        // provided then printing will not be supported on the Linux platform.
        // 返回用于在Linux上打印的处理程序。如果没有提供打印处理程序，那么Linux平台将不支持打印。
        ///
        /*--cef()--*/
        CefRefPtr<CefPrintHandler> GetPrintHandler() OVERRIDE { return nullptr; }
		///
		// Called from any thread when work has been scheduled for the browser process
		// main (UI) thread. This callback is used in combination with CefSettings.
		// external_message_pump and CefDoMessageLoopWork() in cases where the CEF
		// message loop must be integrated into an existing application message loop
		// (see additional comments and warnings on CefDoMessageLoopWork). This
		// callback should schedule a CefDoMessageLoopWork() call to happen on the
		// main (UI) thread. |delay_ms| is the requested delay in milliseconds. If
		// |delay_ms| is <= 0 then the call should happen reasonably soon. If
		// |delay_ms| is > 0 then the call should be scheduled to happen after the
		// specified delay and any currently pending scheduled call should be
		// cancelled.
		// 当为浏览器进程主线程（UI）调度工作时，从任何线程调用。这个回调与CefSettings一起使用。external_message_pump和CefDoMessageLoopWork（）在CEF消息循环必须集成到现有应用程序消息循环的情况下（参见CefDoMessageLoopWork的其他注释和警告）。这个回调应该安排CefDoMessageLoopWork（）调用发生在主（UI）线程上。|delay_ms|是请求的延迟时间，单位为毫秒。如果|delay_ms| <= 0，那么调用应该很快发生。如果|delay_ms|是> 0，那么调用应该被安排在指定的延迟之后发生，任何当前待定的计划调用应该被取消。
		///
		/*--cef()--*/
		void OnScheduleMessagePumpWork(int64 delay_ms) OVERRIDE;

		///
		// Return the default client for use with a newly created browser window. If
		// null is returned the browser will be unmanaged (no callbacks will be
		// executed for that browser) and application shutdown will be blocked until
		// the browser window is closed manually. This method is currently only used
		// with the chrome runtime.
		// 返回默认客户端，以便与新创建的浏览器窗口一起使用。如果返回null，则浏览器将被非托管（不会为该浏览器执行回调），并且应用程序关闭将被阻止，直到手动关闭浏览器窗口。此方法目前仅用于chrome运行时。
		///
		/*--cef()--*/
		//CefRefPtr<CefClient> GetDefaultClient() OVERRIDE { return nullptr; }
#endif //! CefBrowserProcessHandler


#if 1
		/*********************************************************************************************************/
		// CefRenderProcessHandler Render进程这边， 重写CefRenderProcessHandler:: OnProcessMessageReceived()方法来处理来自Browser进程的消息。
        ///
        // Called after the render process main thread has been created. |extra_info|
        // is a read-only value originating from
        //CefBrowserProcessHandler::OnRenderProcessThreadCreated(). Do not keep a
        // reference to |extra_info| outside of this method.
        // 在渲染进程主线程创建后调用
        ///
        void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) OVERRIDE;
		///
		// Called after WebKit has been initialized.
		///
		// 在WebKit初始化后调用。
		/*--cef()--*/
		void OnWebKitInitialized() OVERRIDE;

		///
		// Called after a browser has been created. When browsing cross-origin a new
		// browser will be created before the old browser with the same identifier is
		// destroyed. |extra_info| is an optional read-only value originating from
		// CefBrowserHost::CreateBrowser(), CefBrowserHost::CreateBrowserSync(),
		// CefLifeSpanHandler::OnBeforePopup() or CefBrowserView::CreateBrowserView().
		// 在创建浏览器后调用。当浏览跨域浏览器时，将在旧浏览器被销毁之前创建一个具有相同标识符的新浏览器。|extra_info|是一个可选的只读值，源自CefBrowserHost::CreateBrowser（）、CefBrowserHost::CreateBrowserSync（）、cefanhandler::OnBeforePopup（）或CefBrowserView::CreateBrowserView（）。
		///
		/*--cef(optional_param=extra_info)--*/
		void OnBrowserCreated(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefDictionaryValue> extra_info) OVERRIDE;

		///
		// Called before a browser is destroyed.
		// 在浏览器被销毁之前调用。
		///
		/*--cef()--*/
		void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) OVERRIDE;

		///
		// Return the handler for browser load status events.
		// 返回浏览器加载状态事件的处理程序。
		///
		/*--cef()--*/
		CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;
		///
		// Called immediately after the V8 context for a frame has been created. To
		// retrieve the JavaScript 'window' object use the CefV8Context::GetGlobal()
		// method. V8 handles can only be accessed from the thread on which they are
		// created. A task runner for posting tasks on the associated thread can be
		// retrieved via the CefV8Context::GetTaskRunner() method.
		// 在创建了一个帧的V8上下文之后立即调用。使用CefV8Context::GetGlobal（）方法来检索JavaScript ‘window’对象。V8句柄只能从创建它们的线程访问。在关联线程上发布任务的任务运行器可以通过CefV8Context::GetTaskRunner（）方法获得。
		///
		/*--cef()--*/
		void OnContextCreated(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context) OVERRIDE;

		///
		// Called immediately before the V8 context for a frame is released. No
		// references to the context should be kept after this method is called.
		// 在V8上下文释放帧之前立即调用。调用此方法后不应保留对上下文的引用。
		///
		/*--cef()--*/
		void OnContextReleased(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context) OVERRIDE;

		///
		// Called for global uncaught exceptions in a frame. Execution of this
		// callback is disabled by default. To enable set
		// CefSettings.uncaught_exception_stack_size > 0.
		// 在框架中调用全局未捕获异常。默认情况下，此回调的执行是禁用的。启用设置CefSettings。Uncaught_exception_stack_size >。
		///
		/*--cef()--*/
		void OnUncaughtException(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context,
			CefRefPtr<CefV8Exception> exception,
			CefRefPtr<CefV8StackTrace> stackTrace) OVERRIDE;

		///
		// Called when a new node in the the browser gets focus. The |node| value may
		// be empty if no specific node has gained focus. The node object passed to
		// this method represents a snapshot of the DOM at the time this method is
		// executed. DOM objects are only valid for the scope of this method. Do not
		// keep references to or attempt to access any DOM objects outside the scope
		// of this method.
		// 当浏览器中的新节点获得焦点时调用。如果没有特定节点获得焦点，则|节点的|值可能为空。传递给此方法的节点对象表示执行此方法时DOM的快照。DOM对象仅对该方法的作用域有效。不要保留对该方法作用域之外的任何DOM对象的引用或试图访问这些对象。
		///
		/*--cef(optional_param=frame,optional_param=node)--*/
		void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefDOMNode> node) OVERRIDE;

		///
		// Called when a new message is received from a different process. Return true
		// if the message was handled or false otherwise. It is safe to keep a
		// reference to |message| outside of this callback.
		// 当从其他进程接收到新消息时调用。如果消息被处理，则返回true，否则返回false。在此回调之外保留对|消息|的引用是安全的。
		// ASSERT(source_process == PID_BROWSER);
		// 收到 browser 的消息回复
		// 重写CefRenderProcessHandler::OnProcessMessageReceived()方法来处理来自Browser进程的消息。
		///
		/*--cef()--*/
		bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message) OVERRIDE;
#endif //! CefRenderProcessHandler

	public:
		// Include the default reference counting implementation.
		IMPLEMENT_REFCOUNTING(JGW_Cef3HelpApp);

		DISALLOW_COPY_AND_ASSIGN(JGW_Cef3HelpApp);
	public:
		//bool m_last_node_is_editable;
		// Handles the renderer side of query routing.
        DelegateSet delegates_;
        CefAppDelegateSet mCefAppDelegateSet;
        // Schemes that will be registered with the global cookie manager.
        std::vector<CefString> cookieable_schemes_;
        // Handles the renderer side of query routing.
        CefRefPtr<CefMessageRouterRendererSide> message_router_;
	};
}

