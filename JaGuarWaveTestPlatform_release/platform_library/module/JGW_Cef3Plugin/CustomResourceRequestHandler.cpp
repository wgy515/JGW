#include "StdAfx.h"
#include "CustomResourceRequestHandler.h"

namespace JGW
{
    CustomResourceRequestHandler::CustomResourceRequestHandler(void)
    {
        resource_manager_ = new CefResourceManager();
       SetupResourceManager(resource_manager_,&string_resource_map_);
    }


    CustomResourceRequestHandler::~CustomResourceRequestHandler(void)
    {
    }

    cef_return_value_t CustomResourceRequestHandler::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback)
    {
        CEF_REQUIRE_IO_THREAD();
#if 0
        std::string strUrl = request->GetURL();
        // 示例：拦截特定URL请求
        if (strUrl.find("blocked.com") != std::string::npos) {
            callback->Cancel();
            return RV_CANCEL;
        }
        return RV_CONTINUE;
#endif
        return resource_manager_->OnBeforeResourceLoad(browser, frame, request,callback);
    }

    CefRefPtr<CefResourceHandler> CustomResourceRequestHandler::GetResourceHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request)
    {
#if 0
        if (request->GetURL().find("local://") == 0) 
        {
            // 返回自定义资源处理器
            return new MyCustomResourceHandler();
        }
#endif
        CEF_REQUIRE_IO_THREAD();

        return resource_manager_->GetResourceHandler(browser, frame, request);
    }

    void CustomResourceRequestHandler::OnResourceLoadComplete(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefResponse> response, URLRequestStatus status, int64 received_content_length)
    {
        CEF_REQUIRE_IO_THREAD();
    }

     CefRefPtr<CefResponseFilter> CustomResourceRequestHandler::GetResourceResponseFilter(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefResponse> response)
     {
         CEF_REQUIRE_IO_THREAD();
#if 0
         return test_runner::GetResourceResponseFilter(browser, frame, request,
             response);
#else
         return nullptr;
#endif
     }
}
