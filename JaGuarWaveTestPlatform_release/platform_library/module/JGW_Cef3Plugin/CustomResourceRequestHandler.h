#pragma once
#include <include/cef_resource_request_handler.h>
#include <include/wrapper/cef_resource_manager.h>
namespace JGW
{
    class CustomResourceRequestHandler : public CefResourceRequestHandler
    {
    public:
        CustomResourceRequestHandler(void);
        ~CustomResourceRequestHandler(void);
    protected:
        //! 资源加载前调用	修改请求头/重定向URL
        cef_return_value_t OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request,
            CefRefPtr<CefRequestCallback> callback) override;
        //! 需要自定义资源响应时调用	加载本地加密/ZIP文件 
        CefRefPtr<CefResourceHandler> GetResourceHandler(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request) override;
        //! 资源加载完成后调用	记录请求日志/分析响应
        void OnResourceLoadComplete(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefResponse> response, URLRequestStatus status, int64 received_content_length) override;

        CefRefPtr<CefResponseFilter> GetResourceResponseFilter(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request,
            CefRefPtr<CefResponse> response) override;

    public:
        IMPLEMENT_REFCOUNTING(CustomResourceRequestHandler);
    private:
        // Manages the registration and delivery of resources.
        CefRefPtr<CefResourceManager> resource_manager_;
        // Used to manage string resources in combination with StringResourceProvider.
        // Only accessed on the IO thread.
        StringResourceMap string_resource_map_;
    };
}
