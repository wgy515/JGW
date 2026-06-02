#pragma once
#include <include/cef_load_handler.h>

namespace JGW
{
    class CustomLoadHandler : public CefLoadHandler
    {
    public:
        CustomLoadHandler(void);
        ~CustomLoadHandler(void);
    protected:
        //! 整体加载状态变化时	更新UI进度条/按钮状态
        void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward) override;
        //! 浏览器开始加载框架时	显示加载动画/初始化表单
        void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transition_type) override;
        //! 框架加载完成时（含HTTP状态码）	执行页面注入JS/提交表单
        void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) override;
        //! 资源加载失败或取消时	显示错误页面/重试逻辑
        void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) override;
    private:
        void LoadErrorPage(CefRefPtr<CefFrame> frame,
            const std::string& failed_url,
            cef_errorcode_t error_code,
            const std::string& other_info);
    public:
        IMPLEMENT_REFCOUNTING(CustomLoadHandler);
    };

}

