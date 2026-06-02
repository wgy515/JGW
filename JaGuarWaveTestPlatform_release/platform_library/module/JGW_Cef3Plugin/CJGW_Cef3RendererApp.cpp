#include "StdAfx.h"
#include <JGW_Cef3Plugin/CJGW_Cef3RendererApp.h>
#include "CJGW_Cef3V8ExtensionHandler.h"
#include "CJGW_ReflectCef3V8Handler.h"
namespace JGW
{
    CCJGW_Cef3RendererApp::CCJGW_Cef3RendererApp(void)
    {
    }


    CCJGW_Cef3RendererApp::~CCJGW_Cef3RendererApp(void)
    {
    }

    CefRefPtr<CefRenderProcessHandler> CCJGW_Cef3RendererApp::GetRenderProcessHandler()
    {
        return this;
    }

    void CCJGW_Cef3RendererApp::OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info)
    {
#if 0
        //// 读取Browser进程传递的参数
        //if (extra_info && extra_info->GetSize() > 0) {
        //    debug_mode_ = extra_info->GetBool(0);
        //}

        // 初始化V8隔离环境
        v8::Isolate::CreateParams params;
        params.array_buffer_allocator = 
            v8::ArrayBuffer::Allocator::NewDefaultAllocator();
        isolate_ = v8::Isolate::New(params);

        // 传递启动参数（如调试模式）
        if (extra_info && extra_info->GetSize() > 0) {
            debug_port_ = extra_info->GetInt(0);
        }
#endif
    }

    void CCJGW_Cef3RendererApp::OnWebKitInitialized()
    {
#if 0
        //! 使用CefV8StackTrace::GetCurrent时可通过参数指定最大帧数，例如
        //! ‌注意事项‌
        //! 必须在TID_RENDERER线程操作5
        //! 堆栈深度过大会影响性能，建议值在10-30之间2
        //! 需配合OnUncaughtException的stackTrace参数获取实际堆栈3
        CefV8Context::GetCurrentContext()->GetTaskRunner()->PostTask(
            CefCreateClosureTask(base::BindOnce(‌[]{
                CefV8StackTrace::SetStackLimit(20); // 设置堆栈深度上限
        }))
            );
#endif
#if 0
        //! Extension方式和窗口绑定方式类似，但是Extension方式是为每一个frame加载到上下文context，一旦加载变不能在修改，没有加载之前，DOM是不存在的，尝试范围这个值的DOM会出现崩溃。Extension方式是在CefRenderProcessHandler::OnWebKitInitialized()函数中用CefRegisterExtension() 函数注册的，是在初始化函数中实现的，所以对于每一个frame都是一样的。
        // Define the extension contents.
        std::string extensionCode =
            "var test;"
            "if (!test)"
            "  test = {};"
            "(function() {"
            "  test.myval = 'My Value!';"
            "})();";

        // Register the extension.
        CefRegisterExtension("v8/test", extensionCode, NULL);
        //! alert(test.myval); // Shows an alert box with "My Value!"
#endif
#if 0
        std::string app_code =
            //-----------------------------------
            //声明JavaScript里要调用的Cpp方法
            "var app;"
            "if (!app)"
            "  app = {};"
            "(function() {"
            //  jsInvokeCPlusPlus 实例函数
            "  app.jsInvokeCPlusPlus = function(v1, v2) {"
            "    native function jsInvokeCPlusPlus();"
            "    return jsInvokeCPlusPlus(v1, v2);"
            "  };"
            //函数
            "  app.jsTransform = function(v1) {"
            "    native function jsTransform();"
            "    return jsTransform(v1);"
            "  };"
            "})();";
        // Register app extension module
        // JavaScript里调用app.jsInvokeCPlusPlus时，就会去通过CefRegisterExtension注册的CefV8Handler列表里查找
        // 找到"v8/app"对应的CCEFV8HandlerEx，就调用它的Execute方法
        // 假设v8Handler_是CCefClientApp的一个成员变量
        //v8Handler_ = new CCEFV8HandlerEx();
        //CefRegisterExtension("v8/app", app_code, v8Handler_);
#endif
#if 1
        std::string app_code =
            "var application;"
            "if (!application)"
            "    application = {};"
            "(function() {"
            "    application.GetId = function() {"
            "        native function GetId();"
            "        return GetId();"
            "    };"
            //"})();";

            // Registered Javascript Function, which will be called by Cpp
            "  application.reflect = function(name,callback) {"
            "    native function reflect();"
            "    return reflect(name,callback);"
            "  };"
            // Registered aysnc Javascript Function, which will be called by Cpp
            "  application.async_reflect = function(name,callback) {"
            "    native function async_reflect();"
            "    return async_reflect(name,callback);"
            "  };"
            "})();";

        LOG(INFO) << "OnWebKitInitialized";
        if (nullptr == mptrReflectCef3V8Handler)
        {
            mptrReflectCef3V8Handler = new CCJGW_ReflectCef3V8Handler();
            LOG(INFO) << "V8 handler is nullptr.";
        }
        CefRegisterExtension("v8/application", app_code, mptrReflectCef3V8Handler);//第一个参数不能为空，否则报错，这个名字可以自定义
#endif
        // Create the renderer-side router for query handling.
        //! 为查询处理创建呈现端路由器。
        CefMessageRouterConfig config;
        message_router_ = CefMessageRouterRendererSide::Create(config);
    }

    void CCJGW_Cef3RendererApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
    {
        CEF_REQUIRE_RENDERER_THREAD();
#if 0
        //! 在CEF程序中，创建一个CefV8Value对象，获取上下文的窗口对象，注入窗口对象一个变量值，网页中就可以使用JavaScript获取这个变量值。窗口绑定在CefRenderProcessHandler::OnContextCreated()函数中。是上下文创建响应函数，窗口绑定方式在每次frame重新加载（context创建）时都会加载一遍，CEF程序可以在OnContextCreated()给每一个frame设置不同的变量值。
        // Retrieve the context's window object.
        CefRefPtr<CefV8Value> object = context->GetGlobal();
        // Create a new V8 string value. See the "Basic JS Types" section below.
        CefRefPtr<CefV8Value> str = CefV8Value::CreateString("My Value!");
        // Add the string to the window object as "window.myval". See the "JS Objects" section below.
        object->SetValue("myval", str, V8_PROPERTY_ATTRIBUTE_NONE);
        //! alert(window.myval); // Shows an alert box with "My Value!"
#endif
#if 0
        //! 自定义类实现CefV8Handler类，实现Execute接口，JavaScript执行函数后，会将函数名称、参数和返回值引用传递给Execute函数，Execute函数根据函数名去调用函数，函数的具体实现在Execute中，然后执行返回返回值。
        class MyV8Handler : public CefV8Handler {
        public:
            MyV8Handler() {}

            virtual bool Execute(const CefString& name,
                CefRefPtr<CefV8Value> object,
                const CefV8ValueList& arguments,
                CefRefPtr<CefV8Value>& retval,
                CefString& exception) OVERRIDE {
                    if (name == "myfunc") {
                        // Return my string value.
                        retval = CefV8Value::CreateString("My Value!");
                        return true;
                    }

                    // Function does not exist.
                    return false;
            }

            // Provide the reference counting implementation for this class.
            IMPLEMENT_REFCOUNTING(MyV8Handler);
        };

        //! Functions and Window Binding
        // Retrieve the context's window object.
        CefRefPtr<CefV8Value> object = context->GetGlobal();

        // Create an instance of my CefV8Handler object.
        CefRefPtr<CefV8Handler> handler = new MyV8Handler();

        // Create the "myfunc" function.
        CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction("myfunc", handler);

        // Add the "myfunc" function to the "window" object.
        object->SetValue("myfunc", func, V8_PROPERTY_ATTRIBUTE_NONE);
        //! alert(window.myfunc()); // Shows an alert box with "My Value!"
#endif
#if 0
        //! 应用场景是需要在CEF中拦截一个URL请求，并把它重定向到另外一个URL，可以调用pFrame->ExecuteJavaScript来执行一个JavaScript脚本，实现跳转。当然也可以实现其他应用功能。
        CefRefPtr<CefFrame> pFrame = browser->GetMainFrame();

        std::string strurl = pFrame->GetURL().ToString();
        std::string strname = pFrame->GetName().ToString();
        pFrame->GetName().ToString().c_str());
        if (pFrame->GetURL() == "https://10.19.141.75/portal/")
        {
            pFrame->ExecuteJavaScript("var param= { url:'https://10.19.141.75/ishelf-web/personalCenter' }; \
                                      window.goToApp(param);\
                                      var paramEx = { isExtend:true };\
                                      window.extendScreen(paramEx);", pFrame->GetURL(), 0);
        }
#endif
        message_router_->OnContextCreated(browser, frame, context);
    }

    void CCJGW_Cef3RendererApp::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
    {
        int id = browser->GetIdentifier();
        auto iter = browser_v8extension_map_.find(id);
        if (iter != browser_v8extension_map_.end()) 
        {
            iter->second->OnContextCreated(browser, frame, context);
        }

        if (nullptr != mptrReflectCef3V8Handler)
        {
            mptrReflectCef3V8Handler->clear();
        }

        message_router_->OnContextReleased(browser,frame,context);
    }

    void CCJGW_Cef3RendererApp::SetIOnContextCreatedSolt(int browser_id ,CefRefPtr<IOnContextCreatedSolt> context_solt)
    {
        browser_v8extension_map_[browser_id] = context_solt;
    }

    bool CCJGW_Cef3RendererApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
    {
        _ASSERT(source_process == PID_BROWSER);
        LOG(INFO) << "CCJGW_Cef3RendererApp::OnProcessMessageReceived";
        LOG(INFO) << message->GetName();
        if (message->GetName() == L"Browser.Return")
        {
            // extract message  
            CefRefPtr<CefListValue> args = message->GetArgumentList();
            CefString strFunctionName = args->GetString(0);
            CefString strArgs = args->GetString(1);
            std::wstring Content(strArgs);

            if (nullptr == mptrReflectCef3V8Handler)
            {
                LOG(ERROR) << "RenderApp::OnProcessMessageReceived V8 Handler is nullptr.";
                return false;
            }

            CallbackMap::const_iterator it = mptrReflectCef3V8Handler->GetCallbackMap().find(std::make_pair(strFunctionName.ToString(), browser->GetIdentifier()));
            if (it == mptrReflectCef3V8Handler.get()->GetCallbackMap().end()) 
            {
                LOG(ERROR) << "CallbackMap not find item.";
                return false;
            }

            // Keep a local reference to the objects. The callback may remove itself
            // from the callback map.
            CefRefPtr<CefV8Context> context = it->second.first;
            CefRefPtr<CefV8Value> callback = it->second.second;

            // Enter the context.
            context->Enter();

            CefV8ValueList arguments;
            // First argument is the message name.
            //arguments.push_back(CefV8Value::CreateString(message_name));
            arguments.push_back(CefV8Value::CreateString(Content));

            // Execute the callback.
            CefRefPtr<CefV8Value> retval = callback->ExecuteFunction(NULL, arguments);
            if (retval.get())
            {
                if (retval->IsBool())
                    bool bReturn = retval->GetBoolValue();
            }

            // Exit the context.
            context->Exit();
            return true;
        }
        else if (message->GetName() == L"CreateBrowser") //创建浏览器
        {
            CefRefPtr<IOnContextCreatedSolt> context_solt = new CCJGW_Cef3V8ExtensionHandler();
            context_solt->ConnectionNamePipe(message->GetArgumentList()->GetString(0));
            browser_v8extension_map_[browser->GetIdentifier()] = context_solt;
        } 
        else if (message->GetName() == L"SetFunctionName")  //设置函数
        {
            auto iter = browser_v8extension_map_.find(browser->GetIdentifier());
            if (iter != browser_v8extension_map_.end()) 
            {
                auto argu = message->GetArgumentList();
                for (size_t i = 0; i < argu->GetSize(); ++i) 
                {
                    iter->second->SetFunction(argu->GetString(i));
                }
            }  
        } 
        else if (message->GetName() == L"CloseBrowser") //关闭浏览器
        {
            auto iter = browser_v8extension_map_.find(browser->GetIdentifier());
            if (iter != browser_v8extension_map_.end()) 
            {
                iter->second->Release();
            }
        }
        else
        {
            return message_router_->OnProcessMessageReceived(browser, frame,source_process, message);
        }
        return true;



#if 0



        bool RenderApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
        {
            // extract message  
            CefRefPtr<CefListValue> args = message->GetArgumentList();
            CefString strFunctionName = args->GetString(0);
            CefString strArgs = args->GetString(1);
            std::wstring Content(strArgs);

            std::wstring strFunName(message->GetName());
            if (nullptr == m_v8Handler)
            {
                LOG(ERROR) << "RenderApp::OnProcessMessageReceived V8 Handler is nullptr.";
                //LOG(ERROR) << strFunName;
                return false;
            }

            if (m_v8Handler->callback_map_.empty()) {
                LOG(ERROR) << "CallbackMap is empty.";
                //LOG(ERROR) << strFunName;
                return false;
            }

            const CefString& message_name = message->GetName();
            CallbackMap::const_iterator it = m_v8Handler->callback_map_.find(
                std::make_pair(message_name.ToString(),
                browser->GetIdentifier()));
            if (it == m_v8Handler.get()->callback_map_.end()) {
                LOG(ERROR) << "CallbackMap not find item.";
                //LOG(ERROR) << strFunName;
                return false;
            }

            // Keep a local reference to the objects. The callback may remove itself
            // from the callback map.
            CefRefPtr<CefV8Context> context = it->second.first;
            CefRefPtr<CefV8Value> callback = it->second.second;

            // Enter the context.
            context->Enter();

            CefV8ValueList arguments;
            // First argument is the message name.
            //arguments.push_back(CefV8Value::CreateString(message_name));
            arguments.push_back(CefV8Value::CreateString(Content));

            // Execute the callback.
            CefRefPtr<CefV8Value> retval = callback->ExecuteFunction(NULL, arguments);
            if (retval.get()) {
                if (retval->IsBool())
                    bool handled = retval->GetBoolValue();
            }

            // Exit the context.
            context->Exit();
            return true;
        }

#endif


        
    }

    void CCJGW_Cef3RendererApp::OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context, CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace)
    {
        // 获取异常详细信息
        CefString errorMsg = exception->GetMessage();
        CefString scriptUrl = exception->GetScriptResourceName();
        int lineNumber = exception->GetLineNumber();

        // 可通过IPC将错误信息发送给Browser进程
        CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("JSException");
        msg->GetArgumentList()->SetString(0, errorMsg);
        frame->SendProcessMessage(PID_BROWSER, msg);  
#if 0
        CefRefPtr<CefV8StackTrace> trace = 
            CefV8StackTrace::GetCurrent(15); // 动态获取15层堆栈
        for(int i=0; i<trace->GetFrameCount(); ++i) {
            CefRefPtr<CefV8StackFrame> frame = trace->GetFrame(i);
            LOG(INFO) << frame->GetFunctionName() << ":" 
                << frame->GetLineNumber();
        }
#endif
    }

    void CCJGW_Cef3RendererApp::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node)
    {
        if (node) 
        {
            CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("FocusedNodeChanged");
            CefRefPtr<CefListValue> args = msg->GetArgumentList();
            args->SetString(0, node->GetElementTagName());  // 标签类型
            args->SetString(1, node->GetElementAttribute("id"));   // 元素ID
            frame->SendProcessMessage(PID_BROWSER, msg);  // 发送至Browser进程
        }
    }
}
