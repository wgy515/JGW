#include "StdAfx.h"
#include "CustomLifeSpanHandler.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "CJGW_Cef3ClientHandler.h"
namespace JGW
{
    CustomLifeSpanHandler::CustomLifeSpanHandler(CefRefPtr<CefMessageRouterBrowserSide>& message_router,std::map<CefString, CustomFunction>& function_map,CCJGW_Cef3ClientHandler* pCef3ClientHandler) : message_router_(message_router),function_map_(function_map),browser_count_(0),is_closing_(false),mpCef3ClientHandler(pCef3ClientHandler)
    {
    }


    CustomLifeSpanHandler::~CustomLifeSpanHandler(void)
    {
    }

    bool CustomLifeSpanHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, const CefString& target_frame_name, WindowOpenDisposition target_disposition, bool user_gesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client, CefBrowserSettings& settings, CefRefPtr<CefDictionaryValue>& extra_info, bool* no_javascript_access)
    {
        return false;
    }

    void NotifyBrowserClosed() {
        // 在UI线程执行
        if (!CefCurrentlyOn(TID_UI)) {
            CefPostTask(TID_UI, base::Bind(&NotifyBrowserClosed));
            return;
        }

        // 退出CEF消息循环
        CefPostTask(TID_UI, base::Bind(&CefQuitMessageLoop));
    }


    void CustomLifeSpanHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
    {
        CEF_REQUIRE_UI_THREAD();
        browser_count_++;
        if (!message_router_) 
        {
            // Create the browser-side router for query handling.
            CefMessageRouterConfig config;
            message_router_ = CefMessageRouterBrowserSide::Create(config);

            // Register handlers with the router.
            CreateMessageHandlers(message_handler_set_);
            MessageHandlerSet::const_iterator it = message_handler_set_.begin();
            for (; it != message_handler_set_.end(); ++it)
                message_router_->AddHandler(*(it), false);
        }

        base::AutoLock lock_scope(lock_);
        if (browser.get()) 
        {
            //browser_ = browser;
            //is_created_ = true;
            //给Render进程发送消息
            //创建命名管道
            wchar_t name_pipe[50] = {0};
            wsprintf(name_pipe, L"\\\\.\\pipe\\cef_pipe_%d", browser->GetIdentifier());
            handle_name_pipe_ = CreateNamedPipe(name_pipe, PIPE_ACCESS_DUPLEX|FILE_FLAG_OVERLAPPED,
                0, 1, 1024, 1024, 0, NULL);

            //发送消息 创建JS回调和连接管道
            CefRefPtr<CefProcessMessage> msg= CefProcessMessage::Create(L"CreateBrowser");
            if (msg->IsValid()) 
            {
                CefRefPtr<CefListValue> msg_param = msg->GetArgumentList();
                msg_param->SetString(0, name_pipe);
                browser->GetMainFrame()->SendProcessMessage(PID_RENDERER, msg);
            }

            //客户端连接
            if (handle_name_pipe_ != INVALID_HANDLE_VALUE) 
            {
                HANDLE hEvent;
                hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
                if (hEvent != INVALID_HANDLE_VALUE) {
                    OVERLAPPED over = {0};
                    ConnectNamedPipe(handle_name_pipe_, &over);
                }
            }

            //给Render进程发送消息 设置函数名称
            if (function_map_.size() != 0) 
            {
                CefRefPtr<CefProcessMessage> msg_fun= CefProcessMessage::Create(L"SetFunctionName");
                if (msg_fun->IsValid()) {
                    CefRefPtr<CefListValue> args = msg_fun->GetArgumentList();
                    int index = 0;
                    for (auto iter = function_map_.begin(); iter != function_map_.end(); ++iter) {
                        args->SetString(index, iter->first);
                        ++index;
                    }
                    browser->GetMainFrame()->SendProcessMessage(PID_RENDERER, msg_fun);
                }
            }
        }

        HWND hWnd = browser->GetHost()->GetWindowHandle();
        if (hWnd) 
        {
            HICON hIcon = (HICON)::LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(gnMainIconResId), IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
            ::SendMessage(hWnd, WM_SETICON, (WPARAM) TRUE, (LPARAM) hIcon);
            hIcon = (HICON)::LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(gnMainIconResId), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
            ::SendMessage(hWnd, WM_SETICON, (WPARAM) FALSE, (LPARAM) hIcon);
            //::SendMessage(hWnd, WM_SETICON, TRUE, (LPARAM)LoadIcon(NULL, MAKEINTRESOURCE(107)));  // 设置大图标
            //::SendMessage(hWnd, WM_SETICON, FALSE, (LPARAM)LoadIcon(NULL, MAKEINTRESOURCE(107))); // 设置小图标
        }

        // Add to the list of existing browsers.
        browser_list_.push_back(browser);

        {
            base::AutoLock autoLock(mpCef3ClientHandler->mBrowserEventDelegateLock);
            for (std::vector<CCJGW_Cef3BrowserEventDelegate*>::iterator it = mpCef3ClientHandler->mvCef3BrowserEventDelegate.begin();it != mpCef3ClientHandler->mvCef3BrowserEventDelegate.end();++it)
            {
                if (NULL != it[0])
                    it[0]->OnBrowserCreate(browser->GetIdentifier(),browser->GetMainFrame()->GetURL().ToWString());
            }
        }

    }

    bool CustomLifeSpanHandler::DoClose(CefRefPtr<CefBrowser> browser)
    {
         CEF_REQUIRE_UI_THREAD();

#if 0
        // 拦截关闭请求，等待异步任务完成
        if (hasPendingTasks) {
            ScheduleAsyncCleanup();
            return true; // 暂不关闭
        }
        return false;
#endif
        // Closing the main window requires special handling. See the DoClose()
        // documentation in the CEF header for a detailed destription of this
        // process.
        //! 关闭主窗口需要特殊处理
        if (browser_list_.size() == 1) 
        {
            // Set a flag to indicate that the window close should be allowed.
            is_closing_ = true;
        }

        //通知render进程关闭浏览器
        BrowserList::iterator bit = browser_list_.begin();
        for (; bit != browser_list_.end(); ++bit) {
            if ((*bit)->IsSame(browser)) {
                CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(L"CloseBrowser");
                (*bit)->GetMainFrame()->SendProcessMessage(PID_RENDERER, msg);
            }
        }
        
        

        // Allow the close. For windowed browsers this will result in the OS close
        // event being sent.
        //通知render进程关闭浏览器
        //CefRefPtr<CefProcessMessage> msg= CefProcessMessage::Create(L"CloseBrowser");
        //browser->GetMainFrame()->SendProcessMessage(PID_RENDERER, msg);
        OutputDebugStringW(L"--------------------DoClose-----------------------------\r\n");
       
        return false;
    }

    void CustomLifeSpanHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
    {
        // 释放与浏览器关联的资源
        CEF_REQUIRE_UI_THREAD();
        OutputDebugStringW(L"--------------------OnBeforeClose-----------------------------\r\n");
        if (--browser_count_ == 0) 
        {
            // Remove and delete message router handlers.
            MessageHandlerSet::const_iterator it = message_handler_set_.begin();
            for (; it != message_handler_set_.end(); ++it) {
                message_router_->RemoveHandler(*(it));
                delete *(it);
            }
            message_handler_set_.clear();
            message_router_ = nullptr;
        }

        // Remove from the list of existing browsers.
        BrowserList::iterator bit = browser_list_.begin();
        for (; bit != browser_list_.end(); ++bit) {
            if ((*bit)->IsSame(browser)) {
                browser_list_.erase(bit);
                break;
            }
        }
     
        if (browser_list_.empty()) {
            // All browser windows have closed. Quit the application message loop.
            CefQuitMessageLoop();
        }
        
    }

    void CustomLifeSpanHandler::CloseAllBrowsers(bool force_close) 
    {
        if (!CefCurrentlyOn(TID_UI)) {
            // Execute on the UI thread.
            CefPostTask(TID_UI, base::Bind(&CustomLifeSpanHandler::CloseAllBrowsers, this,
                force_close));
            return;
        }

        DCHECK(CefCurrentlyOn(TID_UI));

        if (browser_list_.empty())
            return;

        BrowserList::const_iterator it = browser_list_.begin();
        for (; it != browser_list_.end(); ++it)
            (*it)->GetHost()->CloseBrowser(force_close);
    }

    void CustomLifeSpanHandler::ExecuteJaveScript(int nBrowserId,const std::wstring& strJsCode)
    {
        if (!CefCurrentlyOn(TID_UI)) {
            // Execute on the UI thread.
            CefPostTask(TID_UI, base::Bind(&CustomLifeSpanHandler::ExecuteJaveScript, this,
                nBrowserId,strJsCode));
            return;
        }

        DCHECK(CefCurrentlyOn(TID_UI));

        //! 传入复合字符包换换行符，无法传递参数
        std::wstring strRawJsCode(strJsCode);
#if 0
        JGW_ReplaceStringW(strRawJsCode,L"\r\n",L"");
        JGW_ReplaceStringW(strRawJsCode,L"\\n\t",L"");
        JGW_ReplaceStringW(strRawJsCode,L" ",L"");
        //CString strReplace = s2.c_str();
        //strReplace.Replace(L"\r\n", L"");
        //strReplace.Replace(L"\n\t", L"");
        //strReplace.Replace(L" ", L"");
        //std::wstring s3(strReplace);
#else
        /*
        ?字符?	?处理方式?	?示例?
        换行符	转义为 \n	"Line1\\nLine2"36
        双引号	转义为 \" 或改用单引号	\"value\" 或 'value'6
        反斜杠	转义为 \\	"C:\\\\path"6
        */
        JGW_ReplaceStringW(strRawJsCode,L"\n",L"\\n");
        JGW_ReplaceStringW(strRawJsCode,L"\r",L"\\r");
        JGW_ReplaceStringW(strRawJsCode,L"\\",L"\\\\");
#endif



        BrowserList::const_iterator it = browser_list_.begin();
        for (; it != browser_list_.end(); ++it)
        {
            if (nBrowserId == (*it)->GetIdentifier())
            {
                //! 在这个框架中执行一串JavaScript代码。|script_url|参数是可以找到所讨论的脚本的URL（如果有的话）。呈现程序可以请求这个URL向开发人员显示错误的来源。|start_line|参数是用于错误报告的基线行号。
                (*it)->GetMainFrame()->ExecuteJavaScript(strRawJsCode,(*it)->GetMainFrame()->GetURL(),215);
            }
        }
    }

    int CustomLifeSpanHandler::GetBrowserIdToUrl(const std::wstring& strUrl)
    {
        BrowserList::const_iterator it = browser_list_.begin();
        for (; it != browser_list_.end(); ++it)
        {
            if ((*it)->GetMainFrame()->GetURL() == strUrl)
            {
                return (*it)->GetIdentifier();
            }
        }
        return -1;
    }
}
