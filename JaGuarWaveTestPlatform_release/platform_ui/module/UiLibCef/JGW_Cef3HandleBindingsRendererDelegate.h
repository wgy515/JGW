#pragma once
#include "CefRendererDelegate.h"

namespace JGW
{
    /*
    注册全局JS函数，供HTML调用，C++函数
    */
    class JGW_Cef3HandleBindingsRendererDelegate : public CefRendererDelegate
    {
    public:
        JGW_Cef3HandleBindingsRendererDelegate(void);
        ~JGW_Cef3HandleBindingsRendererDelegate(void);
    public:
        virtual void OnContextCreated(CefRefPtr<JGW_Cef3HelpApp> app,
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context) OVERRIDE;
    };

}

