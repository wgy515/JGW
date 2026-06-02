#include "StdAfx.h"
#include "CefRendererDelegate.h"
#include "JGW_Cef3HelpApp.h"
namespace JGW
{
    CefRendererDelegate::CefRendererDelegate()
    {

    }

    void CefRendererDelegate::OnRenderThreadCreated(CefRefPtr<JGW_Cef3HelpApp> app,
        CefRefPtr<CefListValue> extra_info) {}

    void CefRendererDelegate::OnWebKitInitialized(CefRefPtr<JGW_Cef3HelpApp> app) 
    {

    }

    void CefRendererDelegate::OnBrowserCreated(CefRefPtr<JGW_Cef3HelpApp> app,
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefDictionaryValue> extra_info) 
    {
       
    }

    void CefRendererDelegate::OnBrowserDestroyed(CefRefPtr<JGW_Cef3HelpApp> app,
        CefRefPtr<CefBrowser> browser) 
    {

    }

    CefRefPtr<CefLoadHandler> CefRendererDelegate::GetLoadHandler(
        CefRefPtr<JGW_Cef3HelpApp> app) 
    {
            return nullptr;
    }

    void CefRendererDelegate::OnContextCreated(CefRefPtr<JGW_Cef3HelpApp> app,
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefV8Context> context) 
    {
        //! CEF3的OnContextCreated回调执行于‌渲染进程(Render Process)‌中，具体机制如下：
        /*
        void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) {
        CefRefPtr<CefV8Value> global = context->GetGlobal();
        global->SetValue("myVar", CefV8Value::CreateString("Hello from C++"), V8_PROPERTY_ATTRIBUTE_NONE);
        }
        */
    }

    void CefRendererDelegate::OnContextReleased(CefRefPtr<JGW_Cef3HelpApp> app,
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefV8Context> context) {}

    void CefRendererDelegate::OnUncaughtException(CefRefPtr<JGW_Cef3HelpApp> app,
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefV8Context> context,
        CefRefPtr<CefV8Exception> exception,
        CefRefPtr<CefV8StackTrace> stackTrace)
    {

    }

    void CefRendererDelegate::OnFocusedNodeChanged(CefRefPtr<JGW_Cef3HelpApp> app,
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefDOMNode> node) 
    {

    }

    // Called when a process message is received. Return true if the message was
    // handled and should not be passed on to other handlers. Delegates
    // should check for unique message names to avoid interfering with each
    // other.
    bool CefRendererDelegate::OnProcessMessageReceived(
        CefRefPtr<JGW_Cef3HelpApp> app,
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefProcessId source_process,
        CefRefPtr<CefProcessMessage> message) 
    {
            return false;
    }
}

