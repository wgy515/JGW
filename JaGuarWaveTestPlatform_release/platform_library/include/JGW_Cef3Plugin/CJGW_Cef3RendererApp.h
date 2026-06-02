#pragma once
#include <include/cef_app.h>
#include <include/cef_render_process_handler.h>
#include <include/wrapper/cef_message_router.h>


namespace JGW
{
    class IOnContextCreatedSolt;
    class CCJGW_ReflectCef3V8Handler;
    //! Renderer（渲染）进程
    //! browser从服务器器请求到了响应，将html文本发送给Renderer 进程，render进程加载html，进行渲染，展示网页的内容；除此之外，Renderer进程还负责Js Binding和对Dom节点的访问。
    class CCJGW_Cef3RendererApp : public CefApp,public CefRenderProcessHandler
    {
    public:
        CCJGW_Cef3RendererApp(void);
        ~CCJGW_Cef3RendererApp(void);
    protected:
#if 0
        //! 属于CefApp接口方法，主要用于处理本地化资源（如字符串、图片等）的加载逻辑。当CEF需要访问.pak资源包时会触发该回调
        //! 仅在‌浏览器进程‌中生效，通过继承CefBrowserProcessHandler实现
        //! 加载加密的本地化资源文件3
        //! 替换Chromium默认UI资源（如按钮图标）
        //! 动态生成内存资源
        //! 一般情况不使用
        CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler() override;
        //! 仅在‌浏览器进程‌中生效，渲染进程调用此方法将返回nullptr
        CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;
#endif
        //! 接收浏览器进程发送过来的消息，跨进程通讯
        //! 处理来自Browser进程的消息
        bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) override;
        //! GetRenderProcessHandler 是 CEF3 框架中 CefApp 类的核心方法，用于获取渲染进程的事件处理器
        /*
        ‌1. 核心作用‌
        ‌进程隔离‌：仅在渲染进程（Renderer Process）中生效，浏览器进程调用返回空指针
        ‌功能入口‌：提供访问 CefRenderProcessHandler 接口的能力，用于处理：
        V8 JS 上下文生命周期（如 OnContextCreated）
        进程通信消息（如 OnProcessMessageReceived）
        WebKit 初始化事件（如 OnWebKitInitialized）
        ‌3. 初始化流程‌
        ‌进程识别‌：通过 CefExecuteProcess 区分进程类型
        ‌处理器注册‌：在渲染进程初始化时自动调用该方法
        ‌功能绑定‌：通过返回的 CefRenderProcessHandler 实现具体功能
        ‌4. 注意事项‌
        ‌线程限制‌：所有 V8 相关操作必须在渲染进程主线程（TID_RENDERER）执行
        ‌多进程模型‌：需在 CefSettings 中配置 multi_threaded_message_loop 以支持多进程
        ‌内存管理‌：必须实现 IMPLEMENT_REFCOUNTING 宏处理引用计数
        */
        CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;

    protected:
        /*************************** CefRenderProcessHandler **********************************/
        //! CEF3中的CefRenderProcessHandler是处理渲染进程回调的核心接口，主要用于控制渲染进程的行为和实现进程间通信。以下是关键功能和使用要点：
        //! 处理渲染进程的生命周期事件和消息回调
        //! 管理V8 JavaScript上下文和执行环境
        //! 实现Browser进程与Render进程的IPC通信



        //! 在渲染进程主线程创建后调用。|extra_info|是一个只读值，来自CefBrowserProcessHandler::OnRenderProcessThreadCreated（）。不要在这个方法之外保留对|extra_info|的引用。
        void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) override;
        //! CEF3中的OnWebKitInitialized是CefRenderProcessHandler接口的关键方法，主要用于渲染进程初始化阶段执行V8扩展注册和JS-C++交互设置。以下是核心要点：
        //! 在WebKit引擎初始化完成后调用，早于任何JS上下文创建
        //! 每个渲染进程仅执行一次，适合做全局JS扩展注册
        //! JS扩展注册‌：通过CefRegisterExtension注入全局JS代码
        //! 进程通信准备‌：初始化与Browser进程的IPC通道
        //! ‌V8环境配置‌：设置JS异常处理等全局回调
        //! 所有V8操作需保证在TID_RENDERER线程执行
        //! 扩展注册后无法修改，且需避免在初始化期间访问DOM
        void OnWebKitInitialized() override;
        //! 获取V8上下文
        //! 通过V8引擎执行JS代码并获取返回值
        //! 所有V8操作必须在TID_RENDERER线程执行
        //! 不同于OnContextCreated（每个Frame调用）
        void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;
        //! 早于OnContextReleased（上下文销毁时调用）
        void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;
        /*
        //! CEF3中的OnUncaughtException是CefRenderProcessHandler接口中处理JavaScript未捕获异常的关键方法，主要功能如下：
        ‌//! 核心作用‌
        //! 捕获V8引擎中未被try-catch处理的JS运行时异常4
        //! 获取异常堆栈信息用于错误分析和上报4
        //! 防止因未处理异常导致渲染进程崩溃
        //! 仅在TID_RENDERER线程触发
        //! 无法捕获语法错误和异步异常（需结合window.onerror
        //! 通过CefV8StackTrace可获取完整的调用堆栈
        //! ‌注意事项‌
        //! ‌需通过SetUncaughtExceptionStackSize设置堆栈深度
        //! ‌与Browser进程通信需处理跨进程消息序列化
        //! ‌建议结合CEF的崩溃报告机制共同使用
        */
        void OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context, CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace) override;  
        /*
        ‌Render进程‌
        用于监测页面焦点元素变化。以下是核心要点
        ‌触发条件‌
        当页面DOM节点获得/失去焦点时触发
        包括input、textarea等可聚焦元素的状态变化
        ‌主要功能‌
        获取焦点元素的DOM属性（tagName/value/id等
        实现虚拟键盘自动弹出等交互功能
        通过进程消息传递焦点信息给Browser进程
        WinForm集成时可能不触发（需检查消息循环
        需确保已正确实现GetRenderProcessHandler
        多进程模式下需处理跨进程通信
        ‌相关方法‌
        常与OnProcessMessageReceived配合使用
        不同于OnContextCreated（上下文创建时触发）
        */
        void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node) override;

    public:
        // Include the default reference counting implementation.
        IMPLEMENT_REFCOUNTING(CCJGW_Cef3RendererApp);

        DISALLOW_COPY_AND_ASSIGN(CCJGW_Cef3RendererApp);

        
    private:
        //设置回调指针
        void SetIOnContextCreatedSolt(int browser_id ,CefRefPtr<IOnContextCreatedSolt> context_solt);
    private:
        // Handles the renderer side of query routing.
        CefRefPtr<CefMessageRouterRendererSide> message_router_;

        CefRefPtr<CCJGW_ReflectCef3V8Handler> mptrReflectCef3V8Handler;
    private:
        //V8扩展实例指针
        std::map<int, CefRefPtr<IOnContextCreatedSolt>> browser_v8extension_map_;
    };
}
