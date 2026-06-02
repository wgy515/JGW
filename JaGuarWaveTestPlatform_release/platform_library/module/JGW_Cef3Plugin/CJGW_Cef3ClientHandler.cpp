#include "StdAfx.h"
#include "CJGW_Cef3ClientHandler.h"
#include <include/cef_parser.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    // Musr match the value in client_renderer.cc.
    const char kFocusedNodeChangedMessage[] = "ClientRenderer.FocusedNodeChanged";


    CCJGW_Cef3ClientHandler::CCJGW_Cef3ClientHandler(void)
    {
        display_handler_ = new CustomDisplayHandler();
        menu_handler_ = new CustomMenuHandler();
        downloader_handler_ = new CustomDownloadHandler();
        drag_handler_ = new CustomDragHandler();
        focus_handler_ = new CustomFocusHandler();
        keyboard_handler_ = new CustomKeyboardHandler();
        load_handler_ = new CustomLoadHandler();
        lifespan_handler_ = new CustomLifeSpanHandler(message_router_,function_map_,this);
        request_handler_ = new CustomRequestHandler(message_router_);
    }


    CCJGW_Cef3ClientHandler::~CCJGW_Cef3ClientHandler(void)
    {
    }

    bool CCJGW_Cef3ClientHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
    {
        CEF_REQUIRE_UI_THREAD();
        std::wstring strMessageName = message->GetName();
        //! 同步或者异步回调
        if (1 ==  message->GetArgumentList()->GetSize() && message->GetName() == L"Renderer.reflect" || message->GetName() == L"Renderer.async_reflect")
        {
            CefRefPtr<CefListValue> args = message->GetArgumentList();
            CefString v8Value = args->GetString(0);
            //CefString strParam = args->GetString(1);

            CefString funcNm;
            CefRefPtr<CefValue> jsonObject = CefParseJSON(v8Value, JSON_PARSER_ALLOW_TRAILING_COMMAS);
            if (jsonObject->IsValid())
            {
                CefRefPtr<CefDictionaryValue> dict = jsonObject->GetDictionary();
                funcNm = dict->GetString("func");
            }
            else
            {
                LOG(ERROR) << "Renderer.Reflect Invalid Json Param" << v8Value;
                return false;
            }

            // TDO 函数名称回调
            //TODO: doSomething() in native way
            CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("Browser.Return");

            // Retrieve the argument list object.
            CefRefPtr<CefListValue> ret_args = msg->GetArgumentList();
            ret_args->SetSize(2);
            ret_args->SetString(0, funcNm);

            //! CALL BACK CPP FUNCTION <=> {"result" : "true","error":""}
            CefString strReturnMessage(L"{\"error_code\" : -1,\"msg\" : \"\",\"error\":\"Unknown function name\"}");
            {
                base::AutoLock autoLock(mLock);
                std::map<std::wstring,pCppFuncCallback>::iterator it = mmapFuncNameFunc.find(funcNm);
                if (it != mmapFuncNameFunc.end())
                {
                    std::wstring strMsg = it->second(v8Value.c_str());
#if 0
                    JGW_ReplaceStringW(strMsg,L"\r\n", L"");
                    JGW_ReplaceStringW(strMsg,L"\n\t", L"");
                    JGW_ReplaceStringW(strMsg,L" ", L"");
#endif
                    strReturnMessage = JGW_GetFormatWString(L"{\"error_code\" : 0,\"error\":\"\",\"msg\":\"%s\"}",strMsg.c_str());
                }
                else
                {
                    strReturnMessage = JGW_GetFormatWString(L"{\"error_code\" : -1,\"error\":\"Unknown function name\",\"msg\":\"\"}");
                }
            }
            //! 传入复合字符包换换行符，无法传递参数
            //std::wstring s2(strContent);
            //CString strReplace = s2.c_str();
            //strReplace.Replace(L"\r\n", L"");
            //strReplace.Replace(L"\n\t", L"");
            //strReplace.Replace(L" ", L"");
            //std::wstring s3(strReplace);

            // Populate the argument values.
            ret_args->SetString(1, strReturnMessage);
            browser->GetMainFrame()->SendProcessMessage(PID_RENDERER, msg);
        }
        else if (message_router_->OnProcessMessageReceived(browser, frame, source_process, message)) 
        {
            return true;
        }
        // Check for messages from the client renderer.
        std::string message_name = message->GetName();
        if (message_name == "FocusedNodeChanged") 
        {
#if 0
            if (message_name == kFocusedNodeChangedMessage) 
            {
                // A message is sent from ClientRenderDelegate to tell us whether the
                // currently focused DOM node is editable. Use of |focus_on_editable_field_|
                // is redundant with CefKeyEvent.focus_on_editable_field in OnPreKeyEvent
                // but is useful for demonstration purposes.
                focus_on_editable_field_ = message->GetArgumentList()->GetBool(0);
                return true;
            }
#endif
            CefRefPtr<CefListValue> args = message->GetArgumentList();
            std::string tagName = args->GetString(0);
            std::string elementId = args->GetString(1);
            // 处理焦点变化逻辑（如触发输入法）
            return true;
        }
        else if (message_name == "JSException")
        {
            CefRefPtr<CefListValue> args = message->GetArgumentList();
            std::string errorMsg = args->GetString(0);
        }
        else if (message_name == kFocusedNodeChangedMessage) 
        {
            // A message is sent from ClientRenderDelegate to tell us whether the
            // currently focused DOM node is editable. Use of |focus_on_editable_field_|
            // is redundant with CefKeyEvent.focus_on_editable_field in OnPreKeyEvent
            // but is useful for demonstration purposes.
            //! focus_on_editable_field_ = message->GetArgumentList()->GetBool(0);
            return true;
        }
        return false;
    }

    void CCJGW_Cef3ClientHandler::CloseAllBrowsers(bool force_close)
    {
        if (lifespan_handler_.get())
        {
            lifespan_handler_->CloseAllBrowsers(force_close);
        }
    }

    bool CCJGW_Cef3ClientHandler::IsClosing() const
    {
        if (lifespan_handler_.get())
        {
            return lifespan_handler_->IsClosing();
        }
        return true;
    }

    void CCJGW_Cef3ClientHandler::RegisterCppFuncToJs(const wchar_t* strFuntionName,pCppFuncCallback pCppFunc)
    {
        base::AutoLock autoLock(mLock);
        std::map<std::wstring,pCppFuncCallback>::iterator it = mmapFuncNameFunc.find(strFuntionName);
        if (it == mmapFuncNameFunc.end())
        {
            mmapFuncNameFunc.insert(std::make_pair<std::wstring,pCppFuncCallback>(strFuntionName,pCppFunc));
        }
    }

    bool CCJGW_Cef3ClientHandler::ExecuteJaveScript(int nBrowserId,const std::wstring& strJsCode)
    {
        if (lifespan_handler_.get())
        {
            lifespan_handler_->ExecuteJaveScript(nBrowserId,strJsCode);
            return true;
        }
        return false;
    }

    int CCJGW_Cef3ClientHandler::GetBrowserIdToUrl(const std::wstring& strUrl)
    {
        if (lifespan_handler_.get())
        {
            return lifespan_handler_->GetBrowserIdToUrl(strUrl);
        }
        return -1;
    }

    void CCJGW_Cef3ClientHandler::AddBrowserEventDelegate(CCJGW_Cef3BrowserEventDelegate* pCef3BrowserEventDelegate)
    {
        base::AutoLock autoLock(mBrowserEventDelegateLock);
        std::vector<CCJGW_Cef3BrowserEventDelegate*>::iterator it = std::find_if(mvCef3BrowserEventDelegate.begin(), mvCef3BrowserEventDelegate.end(),std::bind2nd(std::equal_to<CCJGW_Cef3BrowserEventDelegate*>(), pCef3BrowserEventDelegate));
        if (it == mvCef3BrowserEventDelegate.end())
        {
            mvCef3BrowserEventDelegate.push_back(pCef3BrowserEventDelegate);
        }
    }

    void CCJGW_Cef3ClientHandler::RemoveBrowserEventDelegate(CCJGW_Cef3BrowserEventDelegate* pCef3BrowserEventDelegate)
    {
        base::AutoLock autoLock(mBrowserEventDelegateLock);
        std::vector<CCJGW_Cef3BrowserEventDelegate*>::iterator it = std::find_if(mvCef3BrowserEventDelegate.begin(), mvCef3BrowserEventDelegate.end(),std::bind2nd(std::equal_to<CCJGW_Cef3BrowserEventDelegate*>(), pCef3BrowserEventDelegate));
        if (it != mvCef3BrowserEventDelegate.end())
        {
            mvCef3BrowserEventDelegate.erase(it);
        }
    }
}

