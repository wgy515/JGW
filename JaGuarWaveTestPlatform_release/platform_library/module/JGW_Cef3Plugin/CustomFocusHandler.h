#pragma once
#include <include/cef_focus_handler.h>
namespace JGW
{
    //! 需在Browser进程UI线程中调用，离屏渲染需额外处理
    /*
    ‌5. 注意事项‌
    ‌线程安全‌：必须通过CefPostTask在UI线程操作
    ‌与OnGotFocus区别‌：OnSetFocus是请求阶段，OnGotFocus是实际获得焦点后
    ‌多进程模式‌：需确保Browser进程与Render进程同步
    */
    class CustomFocusHandler : public CefFocusHandler
    {
    public:
        CustomFocusHandler(void);
        ~CustomFocusHandler(void);
    protected:
        //! 浏览器即将失去焦点时	关闭悬浮弹窗/保存表单状态
        void OnTakeFocus(CefRefPtr<CefBrowser> browser, bool next) override;
        //! 浏览器请求获取焦点时	允许/拒绝焦点切换请求
        //! CEF3的OnSetFocus是CefFocusHandler接口中的核心方法，其功能和使用要点如下：
        /*
        ‌参数‌
            browser：触发事件的浏览器实例
            source：焦点来源（如FOCUS_SOURCE_NAVIGATION导航触发或FOCUS_SOURCE_SYSTEM系统触发）
        ‌返回值
        true：阻止默认焦点处理
        false：允许CEF继续默认行为
        ‌2. 触发时机‌
        当浏览器内元素（如输入框）尝试获取焦点时
        通过JS调用element.focus()或用户点击页面元素时

        当宿主应用与CEF控件存在焦点竞争时，通过返回值协调焦点分配
        阻止恶意页面通过自动获取焦点进行UI欺骗
       */
        bool OnSetFocus(CefRefPtr<CefBrowser> browser, FocusSource source) override;
        //! 浏览器实际获得焦点后	高亮激活状态的UI元素
        void OnGotFocus(CefRefPtr<CefBrowser> browser) override;
    public:
        IMPLEMENT_REFCOUNTING(CustomFocusHandler);
    };

}

