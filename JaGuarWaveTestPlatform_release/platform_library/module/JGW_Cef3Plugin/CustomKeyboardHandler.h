#pragma once
#include <include/cef_keyboard_handler.h>

namespace JGW
{
    class CustomKeyboardHandler : public CefKeyboardHandler
    {
    public:
        CustomKeyboardHandler(void);
        ~CustomKeyboardHandler(void);
    protected:
        //! 按键事件到达渲染进程前	true拦截事件，false继续传递
        /*
        browser：触发事件的浏览器实例
        event：包含键码、修饰符等信息的键盘事件对象
        is_keyboard_shortcut：标识是否为系统级快捷键

        windows_key_code	Windows虚拟键码（如VK_F12）
        modifiers	修饰键状态（ALT/CTRL/SHIFT）
        type	事件类型（KEYEVENT_RAWKEYDOWN等）
        */
        bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
            const CefKeyEvent& event,
            CefEventHandle os_event,
            bool* is_keyboard_shortcut) override;
        //! 渲染进程处理按键事件后	通常用于后置处理
        bool OnKeyEvent(CefRefPtr<CefBrowser> browser,
            const CefKeyEvent& event,
            CefEventHandle os_event) override;
    public:
        IMPLEMENT_REFCOUNTING(CustomKeyboardHandler);
    };


}
