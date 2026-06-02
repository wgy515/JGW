#include "StdAfx.h"
#include "CustomRequestHandler.h"

namespace JGW
{
    CustomRequestHandler::CustomRequestHandler(CefRefPtr<CefMessageRouterBrowserSide>& message_router) : message_router_(message_router)
    {
        mptrResourceRequestHandler = new CustomResourceRequestHandler();
    }


    CustomRequestHandler::~CustomRequestHandler(void)
    {
    }

    bool CustomRequestHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool user_gesture, bool is_redirect)
    {
        CEF_REQUIRE_UI_THREAD();
#if 0
        const std::string& url = request->GetURL();
        // 示例：拦截广告域名
        if (url.find("ads.example.com") != std::string::npos) {
            return true; // 阻止导航
        }
        return false; // 允许其他请求
#endif


        message_router_->OnBeforeBrowse(browser, frame);
        return false;
    }

    void CustomRequestHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, TerminationStatus status)
    {
        CEF_REQUIRE_UI_THREAD();

        message_router_->OnRenderProcessTerminated(browser);

        // 示例：记录崩溃日志
        LOG(ERROR) << "Render process crashed with status: " << status;
#if 0
        //! 如果没有启动URL，或者指定了崩溃URL，则不要重新加载。
        // Don't reload if there's no start URL, or if the crash URL was specified.
        if (startup_url_.empty() || startup_url_ == "chrome://crash")
            return;

        CefRefPtr<CefFrame> frame = browser->GetMainFrame();
        std::string url = frame->GetURL();

        // Don't reload if the termination occurred before any URL had successfully
        // loaded.
        if (url.empty())
            return;

        std::string start_url = startup_url_;

        // Convert URLs to lowercase for easier comparison.
        std::transform(url.begin(), url.end(), url.begin(), tolower);
        std::transform(start_url.begin(), start_url.end(), start_url.begin(),
            tolower);

        // Don't reload the URL that just resulted in termination.
        if (url.find(start_url) == 0)
            return;

        frame->LoadURL(startup_url_);
#endif
    }
}
