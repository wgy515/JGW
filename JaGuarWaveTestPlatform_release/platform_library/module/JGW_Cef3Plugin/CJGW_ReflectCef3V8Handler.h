#pragma once
#include <include/cef_app.h>

namespace JGW
{
    typedef std::map<std::pair<std::string, int>, std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value> > >CallbackMap;

    class CCJGW_ReflectCef3V8Handler : public CefV8Handler
    {
    public:
        CCJGW_ReflectCef3V8Handler(void);
        ~CCJGW_ReflectCef3V8Handler(void);
    public:
        /*
        CEF3中CefV8Handler::Execute是处理JavaScript调用C++功能的核心接口，其关键机制如下：

        ‌1. 基本作用‌
        ‌JS-C++桥接‌：当JavaScript调用通过CefV8Handler注册的函数时，Execute方法会被触发执行对应的C++逻辑
        ‌参数传递‌：通过const CefV8ValueList& args接收JS调用参数，通过CefV8Value& retval返回结果
        ‌2. 实现要点‌
        ‌返回值处理‌：
        同步调用：直接修改retval返回结果（需返回true表示成功）
        异步调用：需结合CefPostTask或进程间通信（如SendProcessMessage）延迟返回结果
        ‌线程限制‌：必须在渲染进程的TID_RENDERER线程执行（V8上下文线程）
        */
        bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) override;

        void clear();
    public:
        CallbackMap& GetCallbackMap();
    public:
        IMPLEMENT_REFCOUNTING(CCJGW_ReflectCef3V8Handler);
    private:
        CallbackMap callback_map_;
    };
}
