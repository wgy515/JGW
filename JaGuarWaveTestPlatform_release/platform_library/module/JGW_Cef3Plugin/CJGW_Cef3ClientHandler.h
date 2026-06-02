#pragma once
#include <include/cef_client.h>
#include "CustomMenuHandler.h"
#include "CustomDisplayHandler.h"
#include "CustomDownloadHandler.h"
#include "CustomDragHandler.h"
#include "CustomFocusHandler.h"
#include "CustomKeyboardHandler.h"
#include "CustomLoadHandler.h"
#include "CustomLifeSpanHandler.h"
#include "CustomRequestHandler.h"
#include <include/wrapper/cef_message_router.h>
#include <functional>
#include <JGW_Cef3Plugin/JGW_Cef3PluginExport.h>
namespace JGW
{
    //! CefClient提供了获取各种handler的接口，例如上下文菜单handler、对话框handler、显示状态handler，下载事件handler、拖动事件handler、查找事件handler、键盘handler、生命周期事件handler、加载页面事件handler、离屏render进程handler、请求事件handler等。但是只是返回事件的handle。每个handler的具体的回调函数不在CefClient类中，需要继承各个handler类，才可以实现回调。CefClient类只有一个回调函数OnProcessMessageReceived用来处理进程间的通讯消息。CefClient的定义如下：
    //! TID_UI 线程是浏览器的主线程。如果应用程序在调用调用CefInitialize()时，传递CefSettings.multi_threaded_message_loop=false，这个线程也是应用程序的主线程。
    //! TID_IO 线程主要负责处理IPC消息以及网络通信。
    //! TID_FILE 线程负责与文件系统交互。
    class CCJGW_Cef3ClientHandler : public CefClient
#if 0
        public CefContextMenuHandler,
        public CefDisplayHandler,
        public CefDownloadHandler,
        public CefDragHandler,
        public CefFocusHandler,
        public CefKeyboardHandler,
        public CefLifeSpanHandler,
        public CefLoadHandler,
        public CefRequestHandler,
        public CefResourceRequestHandler
#endif
    {
    public:
        CCJGW_Cef3ClientHandler(void);
        ~CCJGW_Cef3ClientHandler(void);
    protected:
        /************************** CefClient ********************************/
        //! CEF3中GetAudioHandler是用于处理音频相关事件的核心接口
        //! 属于CefClient接口方法，用于处理浏览器中的音频播放状态变化和元数据获取。当页面包含音频元素（如HTML5 <audio>标签）时会触发相关回调
        //! 仅在‌浏览器进程‌中生效，需通过继承CefClient实现
        //! 渲染进程通过IPC与浏览器进程交互音频事件
        CefRefPtr<CefAudioHandler> GetAudioHandler() override { return nullptr; }
        //! CEF3中GetContextMenuHandler是处理浏览器上下文菜单的核心接口
        //! 属于CefClient接口方法，用于自定义右键菜单的显示和行为。通过实现该接口可拦截、修改或禁用Chromium默认的上下文菜单
        //! 在‌浏览器进程‌中生效，通过CefClient::GetContextMenuHandler()返回处理器实例
        //! 渲染进程通过IPC传递菜单事件到浏览器进程处理
        //! 禁用默认右键菜单（如安全限制场景
        //! 添加自定义菜单项（如开发者工具入口）
        //! 根据页面元素动态生成菜单
        //! 需继承CefContextMenuHandler并实现三个核心方法
        //! 菜单操作需在UI线程执行（回调自动保证线程安全）
        //! 通过CefMenuModel管理菜单项层级和状态
        CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override{ return menu_handler_;}
        //! GetDisplayHandler是CEF3中用于处理浏览器显示相关事件的核心接口，其功能和使用方法如下：
        //! ‌1. 功能定位‌
        //! 显示状态管理‌：处理页面标题变化、地址栏URL更新、全屏切换等显示相关事件
        //! 控制台交互‌：捕获JavaScript控制台输出和错误信息
        //! 全屏模式控制‌：响应页面进入/退出全屏状态的请求
        //! 所有回调方法均在浏览器进程的UI线程执行
        CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return display_handler_;}
        /*
        GetDownloadHandler是CEF3中管理文件下载的核心接口，其功能实现和典型用法如下：

        ‌1. 核心功能‌
        ‌下载控制‌：拦截下载请求并指定保存路径
        ‌进度监控‌：实时获取下载进度和状态（开始/完成/取消
        ‌交互处理‌：支持弹出保存对话框或静默下载

        OnBeforeDownload	下载即将开始时	设置保存路径/取消下载
        OnDownloadUpdated	下载进度变化时	更新进度条/检测完成状态

        ‌4. 典型应用场景‌
        ‌路径自定义‌：通过OnBeforeDownload重定向下载目录
        ‌批量下载管理‌：利用OnDownloadUpdated构建下载队列监控系统
        ‌安全拦截‌：在下载前验证文件类型或来源
        ‌5. 注意事项‌
        ‌线程安全‌：所有回调均在浏览器进程的UI线程执行
        ‌生命周期‌：需通过IMPLEMENT_REFCOUNTING实现引用计数
        ‌跨平台差异‌：Windows/Linux路径处理需注意转义字符
        */
        CefRefPtr<CefDownloadHandler> GetDownloadHandler() override { return downloader_handler_;}
        /*
        CEF3的CefDragHandler是处理浏览器拖拽事件的核心接口，其功能实现和典型用法如下：

        ‌1. 核心功能‌
        ‌拖拽事件拦截‌：处理外部内容拖入浏览器窗口的事件
        ‌操作类型控制‌：通过DragOperationMask指定允许的拖拽操作类型（如复制/移动/链接
        ‌跨进程通信‌：在Browser进程的UI线程中处理拖拽数据
        ‌2. 关键方法‌
        方法名	触发时机	返回值说明
        OnDragEnter	外部拖拽进入浏览器窗口时	true取消事件，false继续默认处理
        OnDraggableRegionsChanged	页面定义可拖拽区域时更新宿主窗口	无返回值
        ‌4. 典型应用场景‌
        ‌文件拖拽上传‌：通过OnDragEnter拦截外部文件拖入事件
        ‌安全限制‌：禁止特定类型内容拖入浏览器（如可执行文件)
        ‌自定义拖拽UI‌：结合OnDraggableRegionsChanged实现不规则拖拽区域
        ‌5. 注意事项‌
        ‌线程限制‌：所有方法必须在UI线程调用
        ‌OSR模式差异‌：离屏渲染需额外实现渲染层交互逻辑
        ‌数据访问‌：CefDragData包含拖拽内容的MIME类型和实际数据
        */
        CefRefPtr<CefDragHandler> GetDragHandler() override { return drag_handler_;}
        /*
        CEF3的CefFocusHandler是处理浏览器焦点事件的核心接口，其功能实现和典型用法如下：

        ‌1. 核心功能‌
        ‌焦点状态管理‌：处理浏览器窗口/页面元素的焦点获取与丢失事件
        ‌跨进程同步‌：在Browser进程的UI线程中处理焦点变化7
        ‌宿主应用联动‌：通知宿主程序焦点状态变化以实现UI同步2
        ‌2. 关键方法‌
        方法名	触发时机	典型应用场景
        OnTakeFocus	浏览器即将失去焦点时	关闭悬浮弹窗/保存表单状态
        OnSetFocus	浏览器请求获取焦点时	允许/拒绝焦点切换请求
        OnGotFocus	浏览器实际获得焦点后	高亮激活状态的UI元素

        ‌4. 典型应用场景‌
        ‌弹窗管理‌：当焦点切换到浏览器时自动关闭宿主应用的悬浮窗口
        ‌输入同步‌：确保Web表单与原生输入控件的焦点状态一致
        ‌安全拦截‌：阻止恶意页面通过焦点窃取进行UI欺骗
        ‌5. 注意事项‌
        ‌线程限制‌：所有回调必须在Browser进程的UI线程执行
        ‌OSR模式‌：离屏渲染需额外处理渲染层焦点状态
        ‌返回值处理‌：OnSetFocus返回true可阻止默认焦点行为
        */
        CefRefPtr<CefFocusHandler> GetFocusHandler() override { return focus_handler_;}
        /*
        CEF3的CefKeyboardHandler是处理键盘事件的核心接口，其功能实现和典型用法如下：

        ‌1. 核心功能‌
        ‌键盘事件拦截‌：在按键事件传递到渲染进程前进行预处理
        ‌快捷键处理‌：识别并拦截系统/自定义快捷键组合（如F5刷新、F12开发者工具
        ‌线程安全‌：所有方法均在Browser进程的UI线程调用
        ‌2. 关键方法‌
        方法名	触发时机	返回值作用
        OnPreKeyEvent	按键事件到达渲染进程前	true拦截事件，false继续传递
        OnKeyEvent	渲染进程处理按键事件后	通常用于后置处理

        ‌4. 典型应用场景‌
        ‌禁用开发者工具‌：拦截F12或Ctrl+Shift+I组合键
        ‌自定义快捷键‌：实现应用级快捷键（如Ctrl+S保存
        ‌游戏控制‌：捕获方向键事件用于Web游戏
        ‌5. 注意事项‌
        ‌事件对象‌：CefKeyEvent包含windows_key_code、modifiers等关键字段
        ‌OS差异‌：不同平台需处理对应的键码体系（如Windows的VK_*常量）
        ‌离屏渲染‌：需额外处理OS级输入事件传递
        该接口是CEF3实现复杂键盘交互的基础组件，常与其他Handler协同使用
        */
        CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() override { return keyboard_handler_;}
        /*
        CEF3的CefLoadHandler是用于监控浏览器加载状态的核心接口，其功能和使用要点如下：

        ‌1. 核心功能‌
        ‌加载状态跟踪‌：监控页面加载的开始、完成、错误等状态变化
        ‌HTTP状态码获取‌：在加载完成时提供HTTP响应状态码
        ‌错误处理‌：捕获资源加载失败或取消事件
        ‌2. 关键方法‌
        方法名	触发时机	典型应用场景
        OnLoadStart	浏览器开始加载框架时	显示加载动画/初始化表单
        OnLoadEnd	框架加载完成时（含HTTP状态码）	执行页面注入JS/提交表单
        OnLoadError	资源加载失败或取消时	显示错误页面/重试逻辑
        OnLoadingStateChange	整体加载状态变化时	更新UI进度条/按钮状态

        ‌4. 注意事项‌
        ‌线程安全‌：所有方法均在Browser进程的UI线程调用
        ‌执行顺序‌：可能存在OnLoadEnd先于OnLoadStart触发的异常情况（需额外状态判断
        ‌多框架处理‌：通过CefFrame::IsMain()区分主框架和子框架
        */
        CefRefPtr<CefLoadHandler> GetLoadHandler() override { return load_handler_;}
        /*
        CEF3的CefLifeSpanHandler是管理浏览器窗口生命周期的核心接口，其关键功能和实现要点如下：

        ‌1. 核心功能‌
        ‌窗口生命周期控制‌：处理浏览器窗口创建、关闭等关键事件
        ‌模态窗口管理‌：支持自定义模态对话框逻辑
        ‌进程安全清理‌：确保浏览器对象销毁前完成资源释放
        ‌2. 关键方法‌
        方法名	触发时机	典型应用场景
        OnBeforeClose	浏览器对象销毁前立即调用	释放关联资源/终止子进程
        DoClose	收到关闭请求时调用	拦截或延迟窗口关闭
        OnAfterCreated	浏览器窗口创建完成后调用	初始化窗口相关组件

        ‌4. 关闭流程详解‌
        用户触发关闭操作（如点击X按钮
        调用DoClose()决定是否拦截关闭
        执行JavaScript的onbeforeunload和onunload事件
        最终触发OnBeforeClose()进行清理
        ‌5. 注意事项‌
        ‌线程安全‌：所有方法均在Browser进程的UI线程调用
        ‌多进程模型‌：需区分Browser进程和Render进程的处理逻辑
        ‌模态窗口‌：需在OnBeforeClose()中恢复父窗口状态
        */
        CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return lifespan_handler_;}
        /*
        CEF3的CefRequestHandler是处理网络请求的核心接口，其功能和使用要点如下：

        ‌1. 核心功能‌
        ‌请求拦截与修改‌：在资源加载前拦截并修改请求头/URL
        ‌自定义资源处理‌：实现本地加密/ZIP资源的加载逻辑
        ‌响应过滤‌：捕获HTTP响应数据（包括AJAX请求）
        ‌2. 关键方法‌
        方法名	触发时机	典型应用场景
        OnBeforeResourceLoad	资源加载前调用	修改请求头/重定向URL
        GetResourceHandler	需要自定义资源响应时调用	加载本地加密/ZIP文件
        OnResourceLoadComplete	资源加载完成后调用	记录请求日志/分析响应

        ‌4. 注意事项‌
        ‌线程安全‌：OnBeforeResourceLoad在IO线程调用，需避免直接操作UI
        ‌性能影响‌：频繁修改请求可能降低页面加载速度
        ‌跨域限制‌：修改请求需注意CORS策略
        */
        CefRefPtr<CefRequestHandler> GetRequestHandler() override { return request_handler_;}


#if 0
        /*************************** CefRequestHandler ********************************************/
        CefRefPtr<CefResourceRequestHandler> GetResourceRequestHandler(
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request,
            bool is_navigation,
            bool is_download,
            const CefString& request_initiator,
            bool& disable_default_handling) override;
#endif

        //! 接收渲染进程发送过来的消息，跨进程通讯,返回true表示消息已经处理
        bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message);
    public:
        void CloseAllBrowsers(bool force_close);
        bool IsClosing() const;
        void RegisterCppFuncToJs(const wchar_t* strFuntionName,pCppFuncCallback pCppFunc);
        bool ExecuteJaveScript(int nBrowserId,const std::wstring& strJsCode);
        int GetBrowserIdToUrl(const std::wstring& strUrl);
        void AddBrowserEventDelegate(CCJGW_Cef3BrowserEventDelegate* pCef3BrowserEventDelegate);
        void RemoveBrowserEventDelegate(CCJGW_Cef3BrowserEventDelegate* pCef3BrowserEventDelegate);
    public:
        // Include the default reference counting implementation.
        IMPLEMENT_REFCOUNTING(CCJGW_Cef3ClientHandler);
        DISALLOW_COPY_AND_ASSIGN(CCJGW_Cef3ClientHandler);
        friend CustomLifeSpanHandler;
    private:
        CefRefPtr<CustomDisplayHandler> display_handler_;
        CefRefPtr<CustomMenuHandler> menu_handler_;
        CefRefPtr<CustomDownloadHandler> downloader_handler_;
        CefRefPtr<CustomDragHandler> drag_handler_;
        CefRefPtr<CustomFocusHandler> focus_handler_;
        CefRefPtr<CustomKeyboardHandler> keyboard_handler_;
        CefRefPtr<CustomLoadHandler> load_handler_;
        CefRefPtr<CustomLifeSpanHandler> lifespan_handler_;
        CefRefPtr<CustomRequestHandler> request_handler_;
    private:
        base::Lock mLock;
        base::Lock mBrowserEventDelegateLock;
        // Handles the browser side of query routing. The renderer side is handled
        // in client_renderer.cc.
        CefRefPtr<CefMessageRouterBrowserSide> message_router_;
        //函数map
        std::map<CefString, CustomFunction> function_map_;
        //!
        std::map<std::wstring,pCppFuncCallback> mmapFuncNameFunc;
        //!
        std::vector<CCJGW_Cef3BrowserEventDelegate*> mvCef3BrowserEventDelegate;
    };
}


