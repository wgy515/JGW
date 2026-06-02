#include "StdAfx.h"
#include "CustomDisplayHandler.h"
#include <include/wrapper/cef_helpers.h>
namespace JGW
{
    CustomDisplayHandler::CustomDisplayHandler(void)
    {
    }


    CustomDisplayHandler::~CustomDisplayHandler(void)
    {
    }

    void CustomDisplayHandler::OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url)
    {
        CEF_REQUIRE_UI_THREAD();
    }

    void CustomDisplayHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
    {
        CEF_REQUIRE_UI_THREAD();
        CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
        SetWindowText(hwnd, std::wstring(title).c_str());
        //browser.FrameLoadEnd += (sender, args) => {
        //    if (args.Frame.IsMain) {
        //        string title = args.Frame.Title;
        //        // ¡ä|¨¤¨ª¡À¨º¨¬a
        //    }
        //};
    }

    bool CustomDisplayHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser, cef_log_severity_t level, const CefString& message, const CefString& source, int line)
    {
        CEF_REQUIRE_UI_THREAD();
        return false;
    }

    void CustomDisplayHandler::OnFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen)
    {
        CEF_REQUIRE_UI_THREAD();
    }
}

