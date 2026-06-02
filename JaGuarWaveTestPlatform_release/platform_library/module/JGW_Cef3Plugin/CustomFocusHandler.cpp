#include "StdAfx.h"
#include "CustomFocusHandler.h"


namespace JGW
{
    CustomFocusHandler::CustomFocusHandler(void)
    {
    }


    CustomFocusHandler::~CustomFocusHandler(void)
    {
    }


    void CustomFocusHandler::OnTakeFocus(CefRefPtr<CefBrowser> browser, bool next)
    {
        // 示例：通知宿主应用更新焦点状态
        //::PostMessage(hWnd, WM_CEF_FOCUS_CHANGE, 1, 0);
    }

    bool CustomFocusHandler::OnSetFocus(CefRefPtr<CefBrowser> browser, FocusSource source)
    {
#if 0
        // 强制将焦点保留在宿主窗口
        return true; 
#endif
        // 仅允许用户点击触发的焦点变更
        return (source == FOCUS_SOURCE_SYSTEM) ? false : true;
    }

    void CustomFocusHandler::OnGotFocus(CefRefPtr<CefBrowser> browser)
    {

    }
}
