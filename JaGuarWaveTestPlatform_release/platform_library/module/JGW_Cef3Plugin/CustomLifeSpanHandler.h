#pragma once
#include <include/cef_life_span_handler.h>
#include <include/wrapper/cef_message_router.h>
#include <set>
#include <functional>
namespace JGW
{
    typedef std::function<CefRefPtr<CefValue> (CefRefPtr<CefListValue>)> CustomFunction;
    class CCJGW_Cef3ClientHandler;
    class CustomLifeSpanHandler : public CefLifeSpanHandler
    {
    public:
        CustomLifeSpanHandler(CefRefPtr<CefMessageRouterBrowserSide>& message_router,std::map<CefString, CustomFunction>& function_map,CCJGW_Cef3ClientHandler* pCef3ClientHandler);
        ~CustomLifeSpanHandler(void);
    protected:
        bool OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, const CefString& target_frame_name, WindowOpenDisposition target_disposition, bool user_gesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client, CefBrowserSettings& settings, CefRefPtr<CefDictionaryValue>& extra_info, bool* no_javascript_access) override;
        //! 浏览器窗口创建完成后调用	初始化窗口相关组件
        void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
        //! 收到关闭请求时调用	拦截或延迟窗口关闭
        /*
        true：表示由开发者自行处理窗口销毁（需手动调用 CefBrowserHost::CloseBrowser(true)）13
        false：允许CEF继续执行默认关闭流程
        */
        bool DoClose(CefRefPtr<CefBrowser> browser) override;
        //! 浏览器对象销毁前立即调用	释放关联资源/终止子进程
        void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;
    public:
        // Request that all existing browser windows close.
        void CloseAllBrowsers(bool force_close);
        bool IsClosing() const { return is_closing_; }
        void ExecuteJaveScript(int nBrowserId,const std::wstring& strJsCode);
        int GetBrowserIdToUrl(const std::wstring& strUrl);
    public:
        IMPLEMENT_REFCOUNTING(CustomLifeSpanHandler);
    private:
        int browser_count_;
        CefRefPtr<CefMessageRouterBrowserSide>& message_router_;
        // Set of Handlers registered with the message router.
        MessageHandlerSet message_handler_set_;
    private:
        bool is_closing_ ;
        //命名管道句柄
        HANDLE	handle_name_pipe_;
        // List of existing browser windows. Only accessed on the CEF UI thread.
        typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
        BrowserList browser_list_;
        //!
        CCJGW_Cef3ClientHandler* mpCef3ClientHandler;
        //锁
        base::Lock lock_;
        std::map<CefString, CustomFunction>& function_map_;

    };
}

