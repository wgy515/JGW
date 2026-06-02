#pragma once
#include <include\cef_base.h>
#include <include\base\cef_ref_counted.h>
#include <include\internal\cef_ptr.h>
#include <include\cef_values.h>
#include <include\cef_browser.h>
#include <include\cef_frame.h>
#include <include/cef_app.h>
#include <map>
namespace JGW
{
    class JGW_Cef3HelpApp;

    class CefRendererDelegate : public virtual CefBaseRefCounted 
    {
    public:
        CefRendererDelegate();
        //! 在渲染进程主线程创建后调用
        virtual void OnRenderThreadCreated(CefRefPtr<JGW_Cef3HelpApp> app,
            CefRefPtr<CefListValue> extra_info);
        //! 在WebKit初始化后调用。
        virtual void OnWebKitInitialized(CefRefPtr<JGW_Cef3HelpApp> app);
        //! 在创建浏览器后调用
        virtual void OnBrowserCreated(CefRefPtr<JGW_Cef3HelpApp> app,
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefDictionaryValue> extra_info);
        //! 在浏览器被销毁之前调用。
        virtual void OnBrowserDestroyed(CefRefPtr<JGW_Cef3HelpApp> app,
            CefRefPtr<CefBrowser> browser);
        
        virtual CefRefPtr<CefLoadHandler> GetLoadHandler(
            CefRefPtr<JGW_Cef3HelpApp> app);
        //!  在创建了一个帧的V8上下文之后立即调用
        //! CEF3的OnContextCreated回调执行于‌渲染进程(Render Process)‌中，具体机制如下：
        /*
        在CEF3中，OnContextCreated回调的触发机制与Frame创建密切相关，以下是关键要点：

        ‌触发条件‌
        每次创建新的Frame（包括主Frame和子Frame）时，只要该Frame包含独立的JS上下文环境，就会触发OnContextCreated回调23。这包括：

        主窗口首次加载
        iframe动态创建
        通过window.open()弹出的新窗口
        ‌执行线程限制‌
        该回调仅在渲染进程的TID_RENDERER线程执行，所有V8操作必须在此线程内完成34。若在其他线程操作V8会导致崩溃。

        ‌典型应用场景‌
        开发者通常在此回调中完成以下操作：
        */
        virtual void OnContextCreated(CefRefPtr<JGW_Cef3HelpApp> app,
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context);
        //! 在V8上下文释放帧之前立即调用。调用此方法后不应保留对上下文的引用。
        virtual void OnContextReleased(CefRefPtr<JGW_Cef3HelpApp> app,
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context);
        //! 在框架中调用全局未捕获异常。默认情况下，此回调的执行是禁用的。启用设置CefSettings。Uncaught_exception_stack_size >。
        virtual void OnUncaughtException(CefRefPtr<JGW_Cef3HelpApp> app,
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context,
            CefRefPtr<CefV8Exception> exception,
            CefRefPtr<CefV8StackTrace> stackTrace);
        //! 当浏览器中的新节点获得焦点时调用。如果没有特定节点获得焦点，则|节点的|值可能为空。传递给此方法的节点对象表示执行此方法时DOM的快照。DOM对象仅对该方法的作用域有效。不要保留对该方法作用域之外的任何DOM对象的引用或试图访问这些对象。
        virtual void OnFocusedNodeChanged(CefRefPtr<JGW_Cef3HelpApp> app,
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefDOMNode> node) ;
        // Called when a process message is received. Return true if the message was
        // handled and should not be passed on to other handlers. Delegates
        // should check for unique message names to avoid interfering with each
        // other.
        // 当从其他进程接收到新消息时调用。如果消息被处理，则返回true，否则返回false。在此回调之外保留对|消息|的引用是安全的。
        // ASSERT(source_process == PID_BROWSER);
        // 收到 browser 的消息回复
        // 重写CefRenderProcessHandler::OnProcessMessageReceived()方法来处理来自Browser进程的消息。
        virtual bool OnProcessMessageReceived(
            CefRefPtr<JGW_Cef3HelpApp> app,
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefProcessId source_process,
            CefRefPtr<CefProcessMessage> message);
    public:
        // Include the default reference counting implementation.
        IMPLEMENT_REFCOUNTING(CefRendererDelegate);
        DISALLOW_COPY_AND_ASSIGN(CefRendererDelegate);
    private:
        //std::map<std::wstring,std::wstring>

    };
}

