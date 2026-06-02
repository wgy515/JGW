#pragma once
#include <include/cef_app.h>
namespace JGW
{
    class IOnContextCreatedSolt : public CefBaseRefCounted 
    {
    public:
        //V8环境创建回调
        virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context) = 0;
        //链接命名管道
        virtual void ConnectionNamePipe(const CefString& pipe_name) = 0;
        //设置函数名
        virtual void SetFunction(const CefString& name) = 0;
    private:
        IMPLEMENT_REFCOUNTING(IOnContextCreatedSolt);
    };

    class CCJGW_Cef3V8ExtensionHandler : public CefV8Handler,public IOnContextCreatedSolt
    {
    public:
        CCJGW_Cef3V8ExtensionHandler(void);
        ~CCJGW_Cef3V8ExtensionHandler(void);

        //js回调函数
        virtual bool Execute(const CefString& name,
            CefRefPtr<CefV8Value> object,
            const CefV8ValueList& arguments,
            CefRefPtr<CefV8Value>& retval,
            CefString& exception) OVERRIDE;

    public:
        void OnContextCreated(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context);
        //设置函数
        void SetFunction(const CefString& name);
        //链接命名管道
        void ConnectionNamePipe(const CefString& pipe_name);

    private:
        HANDLE					handle_name_pipe_;	//命名管道句柄
        CefRefPtr<CefBrowser>   browser_;			//浏览器对象
        std::vector<CefString>	function_name_;		//函数名称列表
        IMPLEMENT_REFCOUNTING(CCJGW_Cef3V8ExtensionHandler);
    };
}


