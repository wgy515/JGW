#include "StdAfx.h"
#include "CustomKeyboardHandler.h"

namespace JGW
{
    CustomKeyboardHandler::CustomKeyboardHandler(void)
    {
    }


    CustomKeyboardHandler::~CustomKeyboardHandler(void)
    {
    }

    bool CustomKeyboardHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event, bool* is_keyboard_shortcut)
    {
        // 示例：拦截F5刷新
        if (event.windows_key_code == VK_F5) {
            return true; // 阻止默认刷新行为
        }
        return false;
    }

    bool CustomKeyboardHandler::OnKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event)
    {
        return false;
    }
}
