#pragma once
#include <include/cef_request_handler.h>
#include "CustomResourceRequestHandler.h"

namespace JGW
{
    class CustomRequestHandler : public CefRequestHandler
    {
    public:
        CustomRequestHandler(CefRefPtr<CefMessageRouterBrowserSide>& message_router);
        ~CustomRequestHandler(void);
    protected:
        //! OnBeforeBrowse是CEF3框架中用于控制浏览器导航行为的关键回调函数，属于CefRequestHandler类的核心方法。其功能和使用要点如下：
        /*
        ‌返回值‌：返回true可阻止导航，返回false允许继续导航
        ‌触发时机‌：在浏览器即将发起导航请求时调用（包括链接点击、JS跳转等）
        ‌URL拦截‌：过滤特定域名或协议（如阻止广告链接）
        ‌自定义路由‌：将部分URL重定向到本地处理逻辑（如实现hybrid应用）
        ‌安全控制‌：阻止跨站脚本攻击（XSS）或钓鱼网站访问
        browser	触发导航的浏览器实例
        frame	发起请求的Frame对象
        request	包含URL/Header等完整请求数据
        user_gesture	是否由用户主动触发（如点击
        is_redirect	是否为重定向请求
        需通过CefClient::GetRequestHandler()返回该处理器实例
        ‌5. 注意事项‌
        ‌线程安全‌：该方法在Browser进程的UI线程调用，避免耗时操作1
        ‌与资源拦截区别‌：不同于GetResourceRequestHandler，此方法仅处理主框架导航59
        ‌重定向处理‌：多次重定向可能导致多次调用，需结合is_redirect判断

        */
        bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool user_gesture, bool is_redirect) override;
#if 0
        bool OnOpenURLFromTab(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, WindowOpenDisposition target_disposition, bool user_gesture);
#endif
        //! CEF3的GetResourceRequestHandler是CefRequestHandler接口中的关键方法，用于为特定请求创建自定义资源处理器12。其核心功能和使用要点如下：
        //! 在资源请求初始化时被调用，允许开发者拦截请求并返回自定义处理器
        /*
        ‌2. 典型应用场景‌
        ‌本地资源加载‌：读取加密文件或ZIP压缩包中的网页资源
        ‌请求重定向‌：修改请求目标URL或响应内容
        ‌特殊协议处理‌：实现custom://等非标准协议的资源响应
        if (request->GetURL().find("local://") == 0) {
        // 返回自定义资源处理器
        return new MyCustomResourceHandler();
        }
        */
        CefRefPtr<CefResourceRequestHandler> GetResourceRequestHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_navigation, bool is_download, const CefString& request_initiator, bool& disable_default_handling) override { 
            return mptrResourceRequestHandler;
        }

        //! OnRenderProcessTerminated 是 CEF3 和 Qt WebEngine 中用于处理渲染进程崩溃的关键回调函数，其功能和使用要点如下：
        /*
        ‌1. 功能说明‌
        ‌触发条件‌：当渲染进程异常终止（崩溃或被强制结束）时调用
        ‌参数说明‌：
        terminationStatus：进程终止状态（如崩溃、被杀死等）
        exitCode：进程退出代码，非零值表示异常退出
        ‌典型场景‌：
        自动恢复页面（如重新加载崩溃的页面）
        记录崩溃日志用于分析

        ‌3. 注意事项‌
        ‌线程安全‌：在 CEF 中该方法在 Renderer 进程调用，Qt 中在主线程调用
        ‌异步恢复‌：直接调用 reload() 可能导致页面无响应，需通过 QTimer 延迟执行
        ‌状态区分‌：可通过 terminationStatus 区分崩溃类型（如内存不足 vs 主动杀死）
        ‌4. 相关扩展‌
        ‌CEF 多进程通信‌：崩溃后可结合 CefProcessMessage 通知 Browser 进程
        ‌Qt 单进程模式‌：添加 QTWEBENGINE_CHROMIUM_FLAGS="--single-process" 可避免进程分离导致的崩溃
        */
        void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, TerminationStatus status) override;
#if 0
        bool GetAuthCredentials(CefRefPtr<CefBrowser> browser, const CefString& origin_url, bool isProxy, const CefString& host, int port, const CefString& realm, const CefString& scheme, CefRefPtr<CefAuthCallback> callback) override;

        bool OnQuotaRequest(CefRefPtr<CefBrowser> browser, const CefString& origin_url, int64 new_size, CefRefPtr<CefRequestCallback> callback) override;

        bool OnCertificateError(CefRefPtr<CefBrowser> browser, cef_errorcode_t cert_error, const CefString& request_url, CefRefPtr<CefSSLInfo> ssl_info, CefRefPtr<CefRequestCallback> callback) override;

        bool OnSelectClientCertificate(CefRefPtr<CefBrowser> browser, bool isProxy, const CefString& host, int port, const X509CertificateList& certificates, CefRefPtr<CefSelectClientCertificateCallback> callback) override;

        void OnPluginCrashed(CefRefPtr<CefBrowser> browser, const CefString& plugin_path) override;

        void OnRenderViewReady(CefRefPtr<CefBrowser> browser) override;

        void OnDocumentAvailableInMainFrame(CefRefPtr<CefBrowser> browser) override;
#endif
    public:
        IMPLEMENT_REFCOUNTING(CustomRequestHandler);
    private:
        CefRefPtr<CustomResourceRequestHandler> mptrResourceRequestHandler;
        CefRefPtr<CefMessageRouterBrowserSide>& message_router_;
    };
}
