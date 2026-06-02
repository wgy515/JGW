#pragma once
#include <include/cef_display_handler.h>

namespace JGW
{
    class CustomDisplayHandler : public CefDisplayHandler
    {
    public:
        CustomDisplayHandler(void);
        ~CustomDisplayHandler(void);
    protected:
        //! 地址栏URL变化时
        //! 更新导航栏地址显示
        void OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url) override;
        //! 网页标题发生变化时 
        //! 同步更新应用窗口标题
        void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) override;
        //! 页面输出控制台日志时
        //! 记录前端调试日志
        //! 调用以显示控制台消息。返回true以阻止消息输出到控制台
        bool OnConsoleMessage(CefRefPtr<CefBrowser> browser, cef_log_severity_t level, const CefString& message, const CefString& source, int line);
        //! 页面切换全屏模式时
        //! 调整应用窗口布局
        void OnFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen);
    public:
        IMPLEMENT_REFCOUNTING(CustomDisplayHandler);
    };

}

