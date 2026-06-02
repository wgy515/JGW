#include "stdafx.h"
#include "JGW_Cef3HelpApp.h"
#include "client_switches.h"
namespace JGW
{
	extern const char* kFocusedNodeChangedMessage;

	JGW_Cef3HelpApp::JGW_Cef3HelpApp()
	{
        CreateCefRendererDelegates(delegates_);
        CreateCefAppDelegates(mCefAppDelegateSet);
	}

	JGW_Cef3HelpApp::~JGW_Cef3HelpApp()
	{

	}

	void JGW_Cef3HelpApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
	{
		// Pass additional command-line flags to the browser process.
        //! 向浏览器进程传递额外的命令行标志。
        //! 对于浏览器进程，|process_type|值将为空
		if (process_type.empty())
		{
#ifdef _DEBUG
			//debug下启用单进程，方便调试，但是千万不要在release发布版本中使用，官方已经不推荐使用单进程模式
			//注意，使用单进程模式下vs调试，点击关闭软件后vs调试模式不会关闭，需要手动关闭（除了需要在reander进程上设置断点调试外的情况，不要使用单进程模式）
			command_line->AppendSwitch("single-process");
#endif

            // Pass additional command-line flags when off-screen rendering is enabled.
            if (command_line->HasSwitch(switches::kOffScreenRenderingEnabled) &&
                !command_line->HasSwitch(switches::kSharedTextureEnabled)) {
                    // Use software rendering and compositing (disable GPU) for increased FPS
                    // and decreased CPU usage. This will also disable WebGL so remove these
                    // switches if you need that capability.
                    // See https://bitbucket.org/chromiumembedded/cef/issues/1257 for details.
                    if (!command_line->HasSwitch(switches::kEnableGPU)) {
                        command_line->AppendSwitch("disable-gpu");
                        command_line->AppendSwitch("disable-gpu-compositing");
                    }
            }

            if (command_line->HasSwitch(switches::kUseViews) &&
                !command_line->HasSwitch("top-chrome-md")) {
                    // Use non-material mode on all platforms by default. Among other things
                    // this causes menu buttons to show hover state. See usage of
                    // MaterialDesignController::IsModeMaterial() in Chromium code.
                    command_line->AppendSwitchWithValue("top-chrome-md", "non-material");
            }

            if (!command_line->HasSwitch(switches::kCachePath) &&
                !command_line->HasSwitch("disable-gpu-shader-disk-cache")) {
                    // Don't create a "GPUCache" directory when cache-path is unspecified.
                    command_line->AppendSwitch("disable-gpu-shader-disk-cache");
            }

            CefAppDelegateSet::iterator it = mCefAppDelegateSet.begin();
            for (; it != mCefAppDelegateSet.end(); ++it)
                (*it)->OnBeforeCommandLineProcessing(this, command_line);
#if 0
            // Append Chromium command line parameters if touch events are enabled
            //! 如果启用触摸事件，则附加Chromium命令行参数
            // command_line->AppendSwitchWithValue("touch-events", "enabled");
            //! 配置PPAPI Flash插件版本号
			command_line->AppendSwitchWithValue("ppapi-flash-version", "20.0.0.228");
            //! 必须与ppapi-flash-path参数配合使用 
			command_line->AppendSwitchWithValue("ppapi-flash-path", "PepperFlash\\pepflashplayer.dll");

            //! process-per-site模式会使同一域名下的所有页面共享同一个渲染进程，而非默认的每个页面独立进程
			command_line->AppendSwitch("process-per-site");
            //! 在CEF(Chromium Embedded Framework)中禁用GPU加速
            //! 禁用GPU后WebGL功能将不可用，需移除disable-gpu才能启用WebGL 2
            //! 在MFC多文档结构中需配合disable-gpu-compositing解决黑屏问题 3
            //! 可能影响CSS媒体查询响应(如@media screen)的准确性 
			command_line->AppendSwitch("disable-gpu");
			command_line->AppendSwitch("disable-gpu-compositing");
#endif
			//command_line->AppendSwitchWithValue("proxy-server", "SOCKS5://127.0.0.1:1080");	

			//// 开启离屏渲染
			//if (CefManager::GetInstance()->IsEnableOffsetRender())
			//{
			//	command_line->AppendSwitch("disable-surfaces");
			//	command_line->AppendSwitch("enable-begin-frame-scheduling");
			//}
		}

        
	}

	void JGW_Cef3HelpApp::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
	{
#if 0
        // Register "client" as a standard scheme.
        //! 高版本CEF使用
        int options = CEF_SCHEME_OPTION_STANDARD;
        registrar->AddCustomScheme("client", options);
#endif
#if 0
        // Default schemes that support cookies.
        //! 支持cookie的默认方案。
        m_cookieable_schemes.push_back("http");
        m_cookieable_schemes.push_back("https");
        // Default schemes that support cookies.
        //m_cookieable_schemes.push_back("http");
        //m_cookieable_schemes.push_back("https");
#else
        RegisterCustomSchemes(registrar, cookieable_schemes_);
#endif
	}

	void JGW_Cef3HelpApp::OnContextInitialized()
	{
#if 0
        // Register cookieable schemes with the global cookie manager.
        //! 使用全局cookie管理器注册可cookie方案。
        CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager(NULL);
        //ASSERT(manager.get());
        manager->SetSupportedSchemes(m_cookieable_schemes,true, NULL);
        // 这里可以删除了保存的Cooies信息
        // manager->DeleteCookies(L"", L"", nullptr);
#else
        if (!cookieable_schemes_.empty()) {
            // Register cookieable schemes with the global cookie manager.
            CefRefPtr<CefCookieManager> manager =
                CefCookieManager::GetGlobalManager(nullptr);
            DCHECK(manager.get());
            manager->SetSupportedSchemes(cookieable_schemes_, true, nullptr);
        }

        //print_handler_ = CreatePrintHandler();

        CefAppDelegateSet::iterator it = mCefAppDelegateSet.begin();
        for (; it != mCefAppDelegateSet.end(); ++it)
            (*it)->OnContextInitialized(this);

#endif
	}

	void JGW_Cef3HelpApp::OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line)
	{
        CefAppDelegateSet::iterator it = mCefAppDelegateSet.begin();
        for (; it != mCefAppDelegateSet.end(); ++it)
            (*it)->OnBeforeChildProcessLaunch(this, command_line);
	}

    void JGW_Cef3HelpApp::OnRenderProcessThreadCreated(
        CefRefPtr<CefListValue> extra_info) 
    {
        CefAppDelegateSet::iterator it = mCefAppDelegateSet.begin();
        for (; it != mCefAppDelegateSet.end(); ++it)
            (*it)->OnRenderProcessThreadCreated(this, extra_info);
    }

    void JGW_Cef3HelpApp::OnScheduleMessagePumpWork(int64 delay_ms)
    {
#if 0
        // Only used when `--external-message-pump` is passed via the command-line.
        //! 仅在通过命令行传递‘——external-message-pump ’时使用。
        MainMessageLoopExternalPump* message_pump =
            MainMessageLoopExternalPump::Get();
        if (message_pump)
            message_pump->OnScheduleMessagePumpWork(delay);
#endif
    }



    void JGW_Cef3HelpApp::OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) 
    {
        DelegateSet::iterator it = delegates_.begin();
        for (; it != delegates_.end(); ++it)
            (*it)->OnRenderThreadCreated(this, extra_info);
    }

	void JGW_Cef3HelpApp::OnWebKitInitialized()
	{
        DelegateSet::iterator it = delegates_.begin();
        for (; it != delegates_.end(); ++it)
            (*it)->OnWebKitInitialized(this);
#if 1
		// Create the renderer-side router for query handling.
        //! 为查询处理创建呈现端路由器。
		CefMessageRouterConfig config;
		message_router_ = CefMessageRouterRendererSide::Create(config);
#endif
	}

	void JGW_Cef3HelpApp::OnBrowserCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDictionaryValue> extra_info)
	{
        DelegateSet::iterator it = delegates_.begin();
        for (; it != delegates_.end(); ++it)
            (*it)->OnBrowserCreated(this, browser, extra_info);
	}

	void JGW_Cef3HelpApp::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser)
	{
        DelegateSet::iterator it = delegates_.begin();
        for (; it != delegates_.end(); ++it)
            (*it)->OnBrowserDestroyed(this, browser);
	}

	CefRefPtr<CefLoadHandler> JGW_Cef3HelpApp::GetLoadHandler()
	{
        CefRefPtr<CefLoadHandler> load_handler;
        DelegateSet::iterator it = delegates_.begin();
        for (; it != delegates_.end() && !load_handler.get(); ++it)
            load_handler = (*it)->GetLoadHandler(this);

        return load_handler;
	}

	void JGW_Cef3HelpApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
	{
        DelegateSet::iterator it = delegates_.begin();
        for (; it != delegates_.end(); ++it)
            (*it)->OnContextCreated(this, browser, frame, context);

        message_router_->OnContextCreated(browser, frame, context);
#if 0
		CefRefPtr<CefFrame> pFrame = browser->GetMainFrame();

		std::string strurl = pFrame->GetURL().ToString();
		std::string strname = pFrame->GetName().ToString();
		OutputDebugStringA("--------------------------------------\r\n");
		OutputDebugStringA(strurl.c_str());
		OutputDebugStringA("\r\n");
		OutputDebugStringA(strname.c_str());
		OutputDebugStringA("\r\n");

		strurl = frame->GetURL();
		strname = frame->GetName();
		OutputDebugStringA(strurl.c_str());
		OutputDebugStringA("\r\n");
		OutputDebugStringA(strname.c_str());
		OutputDebugStringA("\r\n");
		OutputDebugStringA("--------------------------------------\r\n");

		//CefRefPtr<CefV8Value> object = context->GetGlobal();
		/*
		--------------------------------------
		file:///F:/project/JaGuarWaveTestPlatform_CEF/html/JaGuarWaveTestPlatform_Html/index.html

		file:///F:/project/JaGuarWaveTestPlatform_CEF/html/JaGuarWaveTestPlatform_Html/index.html

		--------------------------------------
		--------------------------------------
		file:///F:/project/JaGuarWaveTestPlatform_CEF/html/JaGuarWaveTestPlatform_Html/index.html

		file:///F:/project/JaGuarWaveTestPlatform_CEF/html/JaGuarWaveTestPlatform_Html/welcom.html
		right
		--------------------------------------
		*/
		message_router_->OnContextCreated(browser, frame, context);

		CefRefPtr<CefV8Value> object = context->GetGlobal();
		CefRefPtr<CefV8Handler> handler = new JGW_Cef3JsBindCallbackHandler(callback_map_);
		CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction("setMessageCallback", handler);
		object->SetValue("setMessageCallback", func, V8_PROPERTY_ATTRIBUTE_NONE);
#if 0
		// Retrieve the context's window object.
		CefRefPtr<CefV8Value> object = context->GetGlobal();

		// Create a new V8 string value. See the "Basic JS Types" section below.
		CefRefPtr<CefV8Value> str = CefV8Value::CreateString("My Value!");

		// Add the string to the window object as "window.myval". See the "JS Objects" section below.
		object->SetValue("myval", str, V8_PROPERTY_ATTRIBUTE_NONE);
#else
#if 0
		std::map<std::wstring, std::wstring> mapWindowBindingObject;
		mcCefWindowBindingObject.GetUrlWindowBindingObject(frame->GetURL().ToWString(), mapWindowBindingObject);

		// Retrieve the context's window object.
		CefRefPtr<CefV8Value> object = context->GetGlobal();
		for (std::map<std::wstring, std::wstring>::iterator it = mapWindowBindingObject.begin(); it != mapWindowBindingObject.end(); ++it)
		{
			// Create a new V8 string value. See the "Basic JS Types" section below.
			CefRefPtr<CefV8Value> str = CefV8Value::CreateString(it->second);
			// Add the string to the window object as "window.myval". See the "JS Objects" section below.
			object->SetValue(it->second, str, V8_PROPERTY_ATTRIBUTE_NONE);
		}
#endif
#endif
#endif
	}

	void JGW_Cef3HelpApp::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
	{
        DelegateSet::iterator it = delegates_.begin();
        for (; it != delegates_.end(); ++it)
            (*it)->OnContextReleased(this, browser, frame, context);

        message_router_->OnContextReleased(browser, frame, context);
#if 0
		message_router_->OnContextReleased(browser, frame, context);
		// 删除已释放上下文的任何已注册的 JavaScript 回调。
		if (!callback_map_.empty()) {
			CallbackMap::iterator it = callback_map_.begin();
			for (; it != callback_map_.end();) {
				if (it->second.first->IsSame(context))
					callback_map_.erase(it++);
				else
					++it;
			}
		}
#endif
	}

    void JGW_Cef3HelpApp::OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context, CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace)
    {
        DelegateSet::iterator it = delegates_.begin();
        for (; it != delegates_.end(); ++it) {
            (*it)->OnUncaughtException(this, browser, frame, context, exception,
                stackTrace);
        }
    }



	void JGW_Cef3HelpApp::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node)
	{
        DelegateSet::iterator it = delegates_.begin();
        for (; it != delegates_.end(); ++it)
            (*it)->OnFocusedNodeChanged(this, browser, frame, node);
#if 0
		bool is_editable = (node.get() && node->IsEditable());
		if (is_editable != m_last_node_is_editable)
		{
			// Notify the browser of the change in focused element type.
			m_last_node_is_editable = is_editable;
			CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(kFocusedNodeChangedMessage);

			message->GetArgumentList()->SetBool(0, is_editable);
			frame->SendProcessMessage(PID_BROWSER, message);
		}
#endif
	}

	void CefListValue2V8Array(CefRefPtr<CefListValue> source, CefRefPtr<CefV8Value> target) {
		assert(target->IsArray());

		int arg_length = static_cast<int>(source->GetSize());
		if (arg_length == 0)
			return;

		for (int i = 0; i < arg_length; ++i)
		{
			CefRefPtr<CefV8Value> new_value;

			CefValueType type = source->GetType(i);
			switch (type)
			{
			case VTYPE_BOOL:
				new_value = CefV8Value::CreateBool(source->GetBool(i));
				break;
			case VTYPE_DOUBLE:
				new_value = CefV8Value::CreateDouble(source->GetDouble(i));
				break;
			case VTYPE_INT:
				new_value = CefV8Value::CreateInt(source->GetInt(i));
				break;
			case VTYPE_STRING:
				new_value = CefV8Value::CreateString(source->GetString(i));
				break;
			case VTYPE_NULL:
				new_value = CefV8Value::CreateNull();
				break;
			case VTYPE_LIST:
			{
				CefRefPtr<CefListValue> list = source->GetList(i);
				new_value = CefV8Value::CreateArray(static_cast<int>(list->GetSize()));
				CefListValue2V8Array(list, new_value);
			}
			break;
			case VTYPE_DICTIONARY:
			{
				//CefRefPtr<CefDictionaryValue> dictionary = source->GetDictionary(i);
				//new_value = CefV8Value::CreateObject(NULL);
				//Cef3DictionaryValue2V8JsonObject(dictionary, new_value);
			}
			break;
			default:
				break;
			}

			if (new_value.get())
			{
				target->SetValue(i, new_value);
			}
			else
			{
				target->SetValue(i, CefV8Value::CreateNull());
			}
		}
	}

	bool JGW_Cef3HelpApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
	{
        //! 猜测大概率是浏览器进程发送的消息，被渲染进程收到
		_ASSERT(source_process == PID_BROWSER);
		// 收到 browser 的消息回复
		//ASSERT(source_process == PID_BROWSER);
		// 收到 browser 的消息回复

        bool handled = false;

        DelegateSet::iterator it = delegates_.begin();
        for (; it != delegates_.end() && !handled; ++it) {
            handled = (*it)->OnProcessMessageReceived(this, browser, frame,
                source_process, message);
        }

        return message_router_->OnProcessMessageReceived(browser, frame,
            source_process, message);

        //return handled;
#if 0
		bool ret = false;
#if 0
		const CefString& message_name = message->GetName();
		//LOG_INFORMATION("[ClientApp][OnProcessMessageReceived]message_name = %s", message_name);
		if (message_name == kExecuteJsCallbackMessage)
		{
			int			callback_id = message->GetArgumentList()->GetInt(0);
			bool		has_error = message->GetArgumentList()->GetBool(1);
			CefString	json_string = message->GetArgumentList()->GetString(2);

			if (m_render_js_bridge.get())
			{
				// 将收到的参数通过管理器传递给调用时传递的回调函数
				ret = m_render_js_bridge->ExecuteJSCallbackFunc(callback_id, has_error, json_string);
			}
		}
		else if (message_name == kCallJsFunctionMessage)
		{
			CefString function_name = message->GetArgumentList()->GetString(0);
			CefString json_string = message->GetArgumentList()->GetString(1);
			int64 frame_id = message->GetArgumentList()->GetInt(2);

			if (m_render_js_bridge.get())
			{
				// 通过 C++ 执行一个已经注册过的 JS 方法
				ret = m_render_js_bridge->ExecuteJSFunc(function_name, json_string, browser, frame);
			}
		}
#endif
		//const CefString& message_name = message->GetName();
		//if (message_name == kBrowserMessageCallbackMessage)
		//{
		//	CefString function_name = message->GetArgumentList()->GetString(0);
		//	CefString json_string = message->GetArgumentList()->GetString(1);
		//	int64 frame_id = message->GetArgumentList()->GetInt(2);
			// 执行注册的 JavaScript 回调（如果有）。
		if (!callback_map_.empty()) {
			const CefString& message_name = message->GetName();
			CallbackMap::const_iterator it = callback_map_.find(
				std::make_pair(message_name.ToString(),
					browser->GetIdentifier()));
			if (it != callback_map_.end()) {
				// 保留对对象的本地引用。回调可能会从回调映射中移除自身。
				CefRefPtr<CefV8Context> context = it->second.first;
				CefRefPtr<CefV8Value> callback = it->second.second;

				// 进入上下文。
				context->Enter();

				CefV8ValueList arguments;

				// 第一个参数是消息名称。
				arguments.push_back(CefV8Value::CreateString(message_name));

				// 第二个参数是消息参数列表。
				CefRefPtr<CefListValue> list = message->GetArgumentList();
				CefRefPtr<CefV8Value> args = CefV8Value::CreateArray(list->GetSize());
#if 1
				CefListValue2V8Array(list, args);  // Helper function to convert CefListValue to CefV8Value.
#else
				for (size_t index = 0; index < list->GetSize(); index++)
				{
					CefRefPtr<CefValue> val = list->GetValue(index);
					args->SetValue(index, list->GetValue(index));
				}
#endif
				arguments.push_back(args);

				// 执行回调。
				CefRefPtr<CefV8Value> retval = callback->ExecuteFunction(nullptr, arguments);
				if (retval.get()) {
					if (retval->IsBool())
						bool handled = retval->GetBoolValue();
				}

				// 退出上下文。
				context->Exit();
			}
		}
		//}

		return message_router_->OnProcessMessageReceived(browser, frame,
			source_process, message);
#endif
	}

}
