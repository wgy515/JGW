#pragma once
#include "include/cef_client.h"
#include <include/wrapper/cef_message_router.h>
#include <include/wrapper/cef_helpers.h>
#include <set>
#include <include/wrapper/cef_resource_manager.h>
#include <include/base/cef_lock.h>
#include <include\cef_browser_process_handler.h>
#include "CefHandlerDelegate.h"
namespace JGW
{
    typedef std::set<CefMessageRouterBrowserSide::Handler*> MessageHandlerSet;
    typedef std::map<std::string, std::string> StringResourceMap;
    //! CefRenderHandler => OnCursorChange
	class JGW_Cef3HelpHandler :
		public CefClient,// CefClient提供了获取各种handler的接口，例如上下文菜单handler、对话框handler、显示状态handler，下载事件handler、拖动事件handler、查找事件handler、键盘handler、生命周期事件handler、加载页面事件handler、离屏render进程handler、请求事件handler等。但是只是返回事件的handle。每个handler的具体的回调函数不在CefClient类中，需要继承各个handler类，才可以实现回调。CefClient类只有一个回调函数OnProcessMessageReceived用来处理进程间的通讯消息。CefClient的定义如下：
		public CefDisplayHandler,// 网页显示处理类
		public CefContextMenuHandler,// 右键菜单类
		public CefDownloadHandler,// 网页下载处理类
		public CefDragHandler,// 鼠标拖动到网页处理类
		public CefKeyboardHandler,// 键盘事件响应处理类
		public CefLifeSpanHandler,// 生命周期处理类
		public CefLoadHandler,// 网页加载处理类
		public CefRequestHandler, // 网络请求处理类
		public CefResourceRequestHandler,
        public CefFocusHandler
	{
	public:
		///
		JGW_Cef3HelpHandler(CefHandlerDelegate* pCefHandlerDelegate,bool bIsEnableOffsetReader);
		~JGW_Cef3HelpHandler();
    private:
        //! 是否开启离屏渲染
        bool mbIsEnableOffsetReader;
        //! 
        CefHandlerDelegate* mppCefHandlerDelegate;
        //! 如果禁用鼠标光标更改，则为True。
        bool mouse_cursor_change_disabled_;
        // True if the browser is currently offline.
        //! 如果浏览器当前处于脱机状态，则为True。
        bool offline_;
        // Track state information for the text context menu.
        struct TestMenuState {
            TestMenuState() : check_item(true), radio_item(0) {}
            bool check_item;
            int radio_item;
        } test_menu_state_;
        // Console logging state.
        const std::string console_log_file_;
        bool first_console_message_;
        // The currently active/foreground browser. Access is protected by
        // |active_browser_lock_;
        mutable base::Lock active_browser_lock_;
        CefRefPtr<CefBrowser> active_browser_;
        // True for the initial navigation after browser creation.
        //! 对于创建浏览器后的初始导航，为True。
        bool initial_navigation_;
        // The current number of browsers using this handler.
        int browser_count_;
        //!
        std::string startup_url_;
        // True if Favicon images should be downloaded.
        bool download_favicon_images_;
        // Used to manage string resources in combination with StringResourceProvider.
        // Only accessed on the IO thread.
        StringResourceMap string_resource_map_;
        bool is_closing_;
    private:
        // Test if the current site has SSL information available.
        bool HasSSLInformation(CefRefPtr<CefBrowser> browser);
        // Test context menu creation.测试上下文菜单的创建。
        void BuildTestMenu(CefRefPtr<CefMenuModel> model);
        //!
        bool ExecuteTestMenu(int command_id);
        //!
        void SetOfflineState(CefRefPtr<CefBrowser> browser, bool offline);
        //! Show SSL information for the current site.
        void ShowSSLInformation(CefRefPtr<CefBrowser> browser);
        //! Show a JS alert message.
        void Alert(CefRefPtr<CefBrowser> browser, const std::string& message);

        // Set a string resource for loading via StringResourceProvider.
        void SetStringResource(const std::string& page, const std::string& data);

        // Create a new popup window using the specified information. |is_devtools|
        // will be true if the window will be used for DevTools. Return true to
        // proceed with popup browser creation or false to cancel the popup browser.
        // May be called on any thead.
        //! 使用指定的信息创建一个新的弹出窗口。|is_devtools如果窗口将用于DevTools， |将为true。返回true继续创建弹出式浏览器，返回false取消弹出式浏览器。可以叫到任何一个头。
        bool CreatePopupWindow(CefRefPtr<CefBrowser> browser,
            bool is_devtools,
            const CefPopupFeatures& popupFeatures,
            CefWindowInfo& windowInfo,
            CefRefPtr<CefClient>& client,
            CefBrowserSettings& settings);
        //!  Load a data: URI containing the error message.
        void LoadErrorPage(CefRefPtr<CefFrame> frame,
            const std::string& failed_url,
            cef_errorcode_t error_code,
            const std::string& other_info);
    public:
         CefHandlerDelegate* GetCefHandlerDelegate(){return mppCefHandlerDelegate;}

        void NotifyFavicon(CefRefPtr<CefImage> image);
        // Returns the number of browsers currently using this handler. Can only be
        // called on the CEF UI thread.
        int GetBrowserCount() const;
        // Show a new DevTools popup window.
        void ShowDevTools(CefRefPtr<CefBrowser> browser,
            const CefPoint& inspect_element_at);

        // Close the existing DevTools popup window, if any.
        void CloseDevTools(CefRefPtr<CefBrowser> browser);



    public:
        // This object may outlive the Delegate object so it's necessary for the
        // Delegate to detach itself before destruction.
        //! 这个对象可能比Delegate对象更长寿，所以Delegate有必要在销毁之前分离自己。
        void DetachDelegate();
    public:
        // CefFocusHandler methods
        ///
        // Called when the browser component is about to loose focus. For instance, if
        // focus was on the last HTML element and the user pressed the TAB key. |next|
        // will be true if the browser is giving focus to the next component and false
        // if the browser is giving focus to the previous component.
        // 当浏览器组件即将失去焦点时调用。例如，如果焦点在最后一个HTML元素上，并且用户按下TAB键。如果浏览器将焦点放在下一个组件上，|将为true；如果浏览器将焦点放在上一个组件上，|将为false。
        ///
        /*--cef()--*/
        void OnTakeFocus(CefRefPtr<CefBrowser> browser, bool next) OVERRIDE;
        ///
        // Called when the browser component is requesting focus. |source| indicates
        // where the focus request is originating from. Return false to allow the
        // focus to be set or true to cancel setting the focus.
        //! 当浏览器组件请求焦点时调用。|表示焦点请求来自何处。返回false允许设置焦点，返回true取消设置焦点。
        ///
        /*--cef()--*/
        bool OnSetFocus(CefRefPtr<CefBrowser> browser, FocusSource source);
	public:
#if 1
		/*************************  CefClient methods:*************************************************/
		///
		// Return the handler for audio rendering events.
		// 返回音频呈现事件的处理程序
		///
		/*--cef()--*/
		CefRefPtr<CefAudioHandler> GetAudioHandler() OVERRIDE { return nullptr; }

		///
		// Return the handler for context menus. If no handler is provided the default
		// implementation will be used.
		// 上下文菜单处理程序
		///
		/*--cef()--*/
		CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE {
			return this;
		}

		///
		// Return the handler for dialogs. If no handler is provided the default
		// implementation will be used.
		// 返回对话框处理程序
		///
		/*--cef()--*/
		CefRefPtr<CefDialogHandler> GetDialogHandler() OVERRIDE { return nullptr; }

		///
		// Return the handler for browser display state events.
		// 返回浏览器显示状态事件的处理程序
		///
		/*--cef()--*/
		CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE { return this; }

		///
		// Return the handler for download events. If no handler is returned downloads
		// will not be allowed.
		// 返回下载事件的处理程序。
		///
		/*--cef()--*/
		CefRefPtr<CefDownloadHandler> GetDownloadHandler() OVERRIDE { return this; }

		///
		// Return the handler for drag events.
		// 返回拖动事件的处理程序
		///
		/*--cef()--*/
		CefRefPtr<CefDragHandler> GetDragHandler() OVERRIDE { return this; }

		///
		// Return the handler for find result events.
		// 返回查找结果事件的处理程序。
		///
		/*--cef()--*/
		CefRefPtr<CefFindHandler> GetFindHandler() OVERRIDE { return nullptr; }

		///
		// Return the handler for focus events.
		// 返回焦点事件的处理程序。
		///
		/*--cef()--*/
		CefRefPtr<CefFocusHandler> GetFocusHandler() OVERRIDE { return nullptr; }

		///
		// Return the handler for events related to CefFrame lifespan. This method
		// will be called once during CefBrowser creation and the result will be
		// cached for performance reasons.
		// 返回与CefFrame生命周期相关的事件处理程序。
		///
		/*--cef()--*/
		//CefRefPtr<CefFrameHandler> GetFrameHandler() OVERRIDE { return nullptr; }

		///
		// Return the handler for JavaScript dialogs. If no handler is provided the
		// default implementation will be used.
		// 返回JavaScript对话框的处理程序。
		///
		/*--cef()--*/
		CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() OVERRIDE { return nullptr; }

		///
		// Return the handler for keyboard events.
		// 返回键盘事件的处理程序。
		///
		/*--cef()--*/
		CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE { return this; }

		///
		// Return the handler for browser life span events.
		// 返回浏览器生命周期事件的处理程序。
		///
		/*--cef()--*/
		CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE { return this; }

		///
		// Return the handler for browser load status events.
		// 返回浏览器加载状态事件的处理程序。
		///
		/*--cef()--*/
		CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE { return this; }
		///
		// Return the handler for off-screen rendering events.
		// 返回屏幕外呈现事件的处理程序。
		///
		/*--cef()--*/
		CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE { return nullptr; }

		///
		// Return the handler for browser request events.
		// 返回浏览器请求事件的处理程序。
		///
		/*--cef()--*/
		CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE { return this; }

		///
		// Called when a new message is received from a different process. Return true
		// if the message was handled or false otherwise.  It is safe to keep a
		// reference to |message| outside of this callback.
		// 当从其他进程接收到新消息时调用。如果消息被处理，则返回true，否则返回false。在此回调之外保留对|消息|的引用是安全的。
		// CefClient类只有一个回调函数OnProcessMessageReceived用来处理进程间的通讯消息
		///
		/*--cef()--*/
		bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message) OVERRIDE;
#endif //! CefClient


#if 1
		/************************ CefContextMenuHandler ****************************************************************************************/
		// CefContextMenuHandler是网页上的右键菜单事件回调函数类，提供OnBeforeContextMenu回调函数，在右键菜单弹出之前修改或者禁用右键菜单。右键菜单按钮响应回调RunContextMenu、右键菜单命令回调OnContextMenuCommand菜单禁用回调函数OnContextMenuDismissed等。可以拦截右键菜单响应，进行自定义的处理。
		///
		// Called before a context menu is displayed. |params| provides information
		// about the context menu state. |model| initially contains the default
		// context menu. The |model| can be cleared to show no context menu or
		// modified to show a custom menu. Do not keep references to |params| or
		// |model| outside of this callback.
		// 在右键菜单弹出之前修改或者禁用右键菜单
		///
		/*--cef()--*/
		void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefContextMenuParams> params,
			CefRefPtr<CefMenuModel> model) OVERRIDE;

		///
		// Called to allow custom display of the context menu. |params| provides
		// information about the context menu state. |model| contains the context menu
		// model resulting from OnBeforeContextMenu. For custom display return true
		// and execute |callback| either synchronously or asynchronously with the
		// selected command ID. For default display return false. Do not keep
		// references to |params| or |model| outside of this callback.
		// 右键菜单按钮响应回调RunContextMenu
		///
		/*--cef()--*/
		bool RunContextMenu(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefContextMenuParams> params,
			CefRefPtr<CefMenuModel> model,
			CefRefPtr<CefRunContextMenuCallback> callback) OVERRIDE {
			return false;
		}

		///
		// Called to execute a command selected from the context menu. Return true if
		// the command was handled or false for the default implementation. See
		// cef_menu_id_t for the command ids that have default implementations. All
		// user-defined command ids should be between MENU_ID_USER_FIRST and
		// MENU_ID_USER_LAST. |params| will have the same values as what was passed to
		// OnBeforeContextMenu(). Do not keep a reference to |params| outside of this
		// callback.
		//  右键菜单命令回调
		///
		/*--cef()--*/
		bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefContextMenuParams> params,
			int command_id,
			EventFlags event_flags);

		///
		// Called when the context menu is dismissed irregardless of whether the menu
		// was empty or a command was selected.
		// 菜单禁用回调函数
		///
		/*--cef()--*/
		void OnContextMenuDismissed(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame) OVERRIDE {}
#endif //! CefContextMenuHandler


#if 1
		/****************************** CefDisplayHandler ***************************************************************/
		// CefDisplayHandler提供了一些页面显示回调函数，例如网址发送变化OnAddressChange，网页标题OnTitleChange发生变化，网页图标发生变化OnFaviconURLChange，全屏变化OnFullscreenModeChange，显示提示消息OnTooltip，状态栏消息显示OnStatusMessage，控制台消息回调OnConsoleMessage，设置了自动调整大小回调OnAutoResize，加载进程变化回调OnLoadingProgressChange

		///
		// Called when a frame's address has changed.
		// 网址发生变化
		///
		/*--cef()--*/
		void OnAddressChange(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const CefString& url) OVERRIDE;

		///
		// Called when the page title changes.
		// 网页标题OnTitleChange发生变化
		///
		/*--cef(optional_param=title)--*/
		void OnTitleChange(CefRefPtr<CefBrowser> browser,
			const CefString& title) OVERRIDE;

		///
		// Called when the page icon changes.
		// 网页图标发生变化OnFaviconURLChange
		///
		/*--cef(optional_param=icon_urls)--*/
		void OnFaviconURLChange(CefRefPtr<CefBrowser> browser,
			const std::vector<CefString>& icon_urls) OVERRIDE;

		///
		// Called when web content in the page has toggled fullscreen mode. If
		// |fullscreen| is true the content will automatically be sized to fill the
		// browser content area. If |fullscreen| is false the content will
		// automatically return to its original size and position. The client is
		// responsible for resizing the browser if desired.
		// 全屏变化OnFullscreenModeChange
		///
		/*--cef()--*/
		void OnFullscreenModeChange(CefRefPtr<CefBrowser> browser,
			bool fullscreen) OVERRIDE;

		///
		// Called when the browser is about to display a tooltip. |text| contains the
		// text that will be displayed in the tooltip. To handle the display of the
		// tooltip yourself return true. Otherwise, you can optionally modify |text|
		// and then return false to allow the browser to display the tooltip.
		// When window rendering is disabled the application is responsible for
		// drawing tooltips and the return value is ignored.
		// 显示提示消息OnTooltip
		///
		/*--cef(optional_param=text)--*/
		bool OnTooltip(CefRefPtr<CefBrowser> browser, CefString& text) OVERRIDE {
			return false;
		}

		///
		// Called when the browser receives a status message. |value| contains the
		// text that will be displayed in the status message.
		// 状态栏消息显示OnStatusMessage
		///
		/*--cef(optional_param=value)--*/
		void OnStatusMessage(CefRefPtr<CefBrowser> browser,
			const CefString& value) OVERRIDE {}

		///
		// Called to display a console message. Return true to stop the message from
		// being output to the console.
		// 控制台消息回调OnConsoleMessage
		///
		/*--cef(optional_param=message,optional_param=source)--*/
		bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
			cef_log_severity_t level,
			const CefString& message,
			const CefString& source,
			int line) OVERRIDE;

		///
		// Called when auto-resize is enabled via CefBrowserHost::SetAutoResizeEnabled
		// and the contents have auto-resized. |new_size| will be the desired size in
		// view coordinates. Return true if the resize was handled or false for
		// default handling.
		// 设置了自动调整大小回调OnAutoResize
		///
		/*--cef()--*/
		bool OnAutoResize(CefRefPtr<CefBrowser> browser,
			const CefSize& new_size) OVERRIDE;
		///
		// Called when the overall page loading progress has changed. |progress|
		// ranges from 0.0 to 1.0.
		// 加载进程变化回调OnLoadingProgressChange
		///
		/*--cef()--*/
		void OnLoadingProgressChange(CefRefPtr<CefBrowser> browser,
			double progress) OVERRIDE {}
#if 0
		///
		// Called when the browser's cursor has changed. If |type| is CT_CUSTOM then
		// |custom_cursor_info| will be populated with the custom cursor information.
		// Return true if the cursor change was handled or false for default handling.
		// 当浏览器的光标改变时调用。如果|type|是CT_CUSTOM，那么|custom_cursor_info|将使用自定义游标信息填充。如果已处理游标更改，则返回true；如果为默认处理，则返回false。
		///
		/*--cef()--*/
		bool OnCursorChange(CefRefPtr<CefBrowser> browser,
			CefCursorHandle cursor,
			cef_cursor_type_t type,
			const CefCursorInfo& custom_cursor_info) OVERRIDE {
			return false;
		}
#endif
#endif //! CefDisplayHandler



#if 1
		/******************************** CefDownloadHandler ******************************************************************/
		// CefDownloadHandler网页上下载文件类，提供开始从网页下载文件回调函数OnBeforeDownload，下载文件进度更新回调函数OnDownloadUpdated。

		///
		// Called before a download begins. |suggested_name| is the suggested name for
		// the download file. By default the download will be canceled. Execute
		// |callback| either asynchronously or in this method to continue the download
		// if desired. Do not keep a reference to |download_item| outside of this
		// method.
		// 提供开始从网页下载文件回调函数OnBeforeDownload
		///
		/*--cef()--*/
		void OnBeforeDownload(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefDownloadItem> download_item,
			const CefString& suggested_name,
			CefRefPtr<CefBeforeDownloadCallback> callback) OVERRIDE {

		}
		///
		// Called when a download's status or progress information has been updated.
		// This may be called multiple times before and after OnBeforeDownload().
		// Execute |callback| either asynchronously or in this method to cancel the
		// download if desired. Do not keep a reference to |download_item| outside of
		// this method.
		// 下载文件进度更新回调函数OnDownloadUpdated。
		///
		/*--cef()--*/
		void OnDownloadUpdated(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefDownloadItem> download_item,
			CefRefPtr<CefDownloadItemCallback> callback) OVERRIDE;

#endif //! CefDownloadHandler




#if 1
		/***************************** CefDragHandler ***********************************************/
		// CefDragHandler处理鼠标拖动事件，提供鼠标拖动进入网页回调函数OnDragEnter，网页中可以拖动放入的区域发生变化回调函数OnDraggableRegionsChanged。

		///
		// Called when an external drag event enters the browser window. |dragData|
		// contains the drag event data and |mask| represents the type of drag
		// operation. Return false for default drag handling behavior or true to
		// cancel the drag event.
		// 提供鼠标拖动进入网页回调函数OnDragEnter
		///
		/*--cef()--*/
		bool OnDragEnter(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefDragData> dragData,
			DragOperationsMask mask) OVERRIDE;

		///
		// Called whenever draggable regions for the browser window change. These can
		// be specified using the '-webkit-app-region: drag/no-drag' CSS-property. If
		// draggable regions are never defined in a document this method will also
		// never be called. If the last draggable region is removed from a document
		// this method will be called with an empty vector.
		// 网页中可以拖动放入的区域发生变化回调函数OnDraggableRegionsChanged。
		///
		/*--cef()--*/
		void OnDraggableRegionsChanged(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const std::vector<CefDraggableRegion>& regions) OVERRIDE;

#endif //! CefDragHandler


#if 1
		/**************************** CefKeyboardHandler **********************************************************************************/
		// CefKeyboardHandler处理键盘响应事件，提供键盘按键响应回调函数。拦截键盘消息。

		///
		// Called before a keyboard event is sent to the renderer. |event| contains
		// information about the keyboard event. |os_event| is the operating system
		// event message, if any. Return true if the event was handled or false
		// otherwise. If the event will be handled in OnKeyEvent() as a keyboard
		// shortcut set |is_keyboard_shortcut| to true and return false.
		// 在键盘事件发送到渲染器之前调用。|event|包含键盘事件信息。|os_event如果有操作系统事件消息，则|为操作系统事件消息。如果事件已处理，则返回true，否则返回false。如果事件将在OnKeyEvent（）中作为键盘快捷键处理，则将|is_keyboard_shortcut|设置为true并返回false。
		///
		/*--cef()--*/
		bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
			const CefKeyEvent& event,
			CefEventHandle os_event,
			bool* is_keyboard_shortcut) OVERRIDE;

		///
		// Called after the renderer and JavaScript in the page has had a chance to
		// handle the event. |event| contains information about the keyboard event.
		// |os_event| is the operating system event message, if any. Return true if
		// the keyboard event was handled or false otherwise.
		// 在渲染器和页面中的JavaScript有机会处理事件之后调用。|event|包含键盘事件信息。|os_event如果有操作系统事件消息，则|为操作系统事件消息。如果键盘事件被处理，则返回true，否则返回false。
		///
		/*--cef()--*/
		bool OnKeyEvent(CefRefPtr<CefBrowser> browser,
			const CefKeyEvent& event,
			CefEventHandle os_event) OVERRIDE {
			return false;
		}
#endif //! CefKeyboardHandler



#if 1
		/****************************** CefLifeSpanHandler ***********************************************************/
		// CefLifeSpanHandler是生命周期处理类，新打开一个网页或者关闭一个网页时，会触发回调函数。OnBeforePopup这个只能在创建一个新的弹出式网页时，才会触发，如果是在一个网页中打开一个子网页，回调函数是拦截不到消息的。OnAfterCreated网页创建完成后的回调函数。browser销毁之前会触发回调函数OnBeforeClose。还有一个关闭回调函数DoClose有点复杂，当调用CefBrowserHost::*CloseBrowser()函数关闭browser，或者browser是CEF创建的顶层窗口的子窗口，当顶层窗口关闭时，也会触发关闭DoClose回调函数。点击网页的关闭按钮后，网页不会立刻关闭，而是会调用两次CloseBrowser()或TryCloseBrowser()，提供了一个让CEF处理JS的onbeforeunload事件和选择性取消关闭网页的机会。CefLifeSpanHandler类定义如下：

		///
		// Called on the UI thread before a new popup browser is created. The
		// |browser| and |frame| values represent the source of the popup request. The
		// |target_url| and |target_frame_name| values indicate where the popup
		// browser should navigate and may be empty if not specified with the request.
		// The |target_disposition| value indicates where the user intended to open
		// the popup (e.g. current tab, new tab, etc). The |user_gesture| value will
		// be true if the popup was opened via explicit user gesture (e.g. clicking a
		// link) or false if the popup opened automatically (e.g. via the
		// DomContentLoaded event). The |popupFeatures| structure contains additional
		// information about the requested popup window. To allow creation of the
		// popup browser optionally modify |windowInfo|, |client|, |settings| and
		// |no_javascript_access| and return false. To cancel creation of the popup
		// browser return true. The |client| and |settings| values will default to the
		// source browser's values. If the |no_javascript_access| value is set to
		// false the new browser will not be scriptable and may not be hosted in the
		// same renderer process as the source browser. Any modifications to
		// |windowInfo| will be ignored if the parent browser is wrapped in a
		// CefBrowserView. Popup browser creation will be canceled if the parent
		// browser is destroyed before the popup browser creation completes (indicated
		// by a call to OnAfterCreated for the popup browser). The |extra_info|
		// parameter provides an opportunity to specify extra information specific
		// to the created popup browser that will be passed to
		// CefRenderProcessHandler::OnBrowserCreated() in the render process.
		// 这个只能在创建一个新的弹出式网页时，才会触发，如果是在一个网页中打开一个子网页，回调函数是拦截不到消息的
		///
		/*--cef(optional_param=target_url,optional_param=target_frame_name)--*/
		bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const CefString& target_url,
			const CefString& target_frame_name,
			CefLifeSpanHandler::WindowOpenDisposition target_disposition,
			bool user_gesture,
			const CefPopupFeatures& popupFeatures,
			CefWindowInfo& windowInfo,
			CefRefPtr<CefClient>& client,
			CefBrowserSettings& settings,
			CefRefPtr<CefDictionaryValue>& extra_info,
			bool* no_javascript_access) OVERRIDE;

		///
		// Called after a new browser is created. It is now safe to begin performing
		// actions with |browser|. CefFrameHandler callbacks related to initial main
		// frame creation will arrive before this callback. See CefFrameHandler
		// documentation for additional usage information.
		// OnAfterCreated网页创建完成后的回调函数
		///
		/*--cef()--*/
		void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;

		///
		// Called when a browser has recieved a request to close. This may result
		// directly from a call to CefBrowserHost::*CloseBrowser() or indirectly if
		// the browser is parented to a top-level window created by CEF and the user
		// attempts to close that window (by clicking the 'X', for example). The
		// DoClose() method will be called after the JavaScript 'onunload' event has
		// been fired.
		//
		// An application should handle top-level owner window close notifications by
		// calling CefBrowserHost::TryCloseBrowser() or
		// CefBrowserHost::CloseBrowser(false) instead of allowing the window to close
		// immediately (see the examples below). This gives CEF an opportunity to
		// process the 'onbeforeunload' event and optionally cancel the close before
		// DoClose() is called.
		//
		// When windowed rendering is enabled CEF will internally create a window or
		// view to host the browser. In that case returning false from DoClose() will
		// send the standard close notification to the browser's top-level owner
		// window (e.g. WM_CLOSE on Windows, performClose: on OS X, "delete_event" on
		// Linux or CefWindowDelegate::CanClose() callback from Views). If the
		// browser's host window/view has already been destroyed (via view hierarchy
		// tear-down, for example) then DoClose() will not be called for that browser
		// since is no longer possible to cancel the close.
		//
		// When windowed rendering is disabled returning false from DoClose() will
		// cause the browser object to be destroyed immediately.
		//
		// If the browser's top-level owner window requires a non-standard close
		// notification then send that notification from DoClose() and return true.
		//
		// The CefLifeSpanHandler::OnBeforeClose() method will be called after
		// DoClose() (if DoClose() is called) and immediately before the browser
		// object is destroyed. The application should only exit after OnBeforeClose()
		// has been called for all existing browsers.
		//
		// The below examples describe what should happen during window close when the
		// browser is parented to an application-provided top-level window.
		//
		// Example 1: Using CefBrowserHost::TryCloseBrowser(). This is recommended for
		// clients using standard close handling and windows created on the browser
		// process UI thread.
		// 1.  User clicks the window close button which sends a close notification to
		//     the application's top-level window.
		// 2.  Application's top-level window receives the close notification and
		//     calls TryCloseBrowser() (which internally calls CloseBrowser(false)).
		//     TryCloseBrowser() returns false so the client cancels the window close.
		// 3.  JavaScript 'onbeforeunload' handler executes and shows the close
		//     confirmation dialog (which can be overridden via
		//     CefJSDialogHandler::OnBeforeUnloadDialog()).
		// 4.  User approves the close.
		// 5.  JavaScript 'onunload' handler executes.
		// 6.  CEF sends a close notification to the application's top-level window
		//     (because DoClose() returned false by default).
		// 7.  Application's top-level window receives the close notification and
		//     calls TryCloseBrowser(). TryCloseBrowser() returns true so the client
		//     allows the window close.
		// 8.  Application's top-level window is destroyed.
		// 9.  Application's OnBeforeClose() handler is called and the browser object
		//     is destroyed.
		// 10. Application exits by calling CefQuitMessageLoop() if no other browsers
		//     exist.
		//
		// Example 2: Using CefBrowserHost::CloseBrowser(false) and implementing the
		// DoClose() callback. This is recommended for clients using non-standard
		// close handling or windows that were not created on the browser process UI
		// thread.
		// 1.  User clicks the window close button which sends a close notification to
		//     the application's top-level window.
		// 2.  Application's top-level window receives the close notification and:
		//     A. Calls CefBrowserHost::CloseBrowser(false).
		//     B. Cancels the window close.
		// 3.  JavaScript 'onbeforeunload' handler executes and shows the close
		//     confirmation dialog (which can be overridden via
		//     CefJSDialogHandler::OnBeforeUnloadDialog()).
		// 4.  User approves the close.
		// 5.  JavaScript 'onunload' handler executes.
		// 6.  Application's DoClose() handler is called. Application will:
		//     A. Set a flag to indicate that the next close attempt will be allowed.
		//     B. Return false.
		// 7.  CEF sends an close notification to the application's top-level window.
		// 8.  Application's top-level window receives the close notification and
		//     allows the window to close based on the flag from #6B.
		// 9.  Application's top-level window is destroyed.
		// 10. Application's OnBeforeClose() handler is called and the browser object
		//     is destroyed.
		// 11. Application exits by calling CefQuitMessageLoop() if no other browsers
		//     exist.
		// 
		/*
			DoClose标准关闭处理
			当窗口创建是在browser进程的UI线程创建时，采用标准的关闭处理，使用CefBrowserHost::TryCloseBrowser()。不实现DoClose回调，默认返回false。具体步骤：
			（1）   点击窗口的关闭按钮，发送一个关闭通知给顶层窗口。
			（2）   顶层窗口接收到关闭通知，调用TryCloseBrowser()函数，返回false；
			（3）   JS的onbeforeunload处理句柄执行显示关闭确认对话框。
			（4）   用户点击按钮同意关闭；
			（5）   JS的onunload处理句柄执行；
			（6）   CEF发送一个close通知给顶层窗口；
			（7）   顶层窗口接收到关闭通知，调用TryCloseBrowser，返回true，表示允许关闭。
			（8）   顶层窗口销毁
			（9）   程序的OnBeforeClose处理回调函数执行，browser销毁.
			（10）如果不存在其他browser，则调用CefQuitMessageLoop退出程序。
			DoClose非标准关闭处理
			当窗口不是在browser进程的UI线程中创建时，采用非标准的关闭处理，使用函数CefBrowserHost::CloseBrowser(false)，并且实现DoClose函数。
			（1）   用户点击窗口的关闭按钮，发送一个关闭通知给顶层窗口。
			（2）   顶层窗口接收到关闭通知，调用CefBrowserHost::CloseBrowser(false)函数，取消关闭；
			（3）   JS的onbeforeunload处理句柄执行显示关闭确认对话框。
			（4）   用户点击按钮同意关闭；
			（5）   JS的onunload处理句柄执行；
			（6）   程序的DoClose（）回调函数被调用，设置一个flag表明下次关闭尝试会被允许，返回false；
			（7）   CEF发送一个close通知给顶层窗口；
			（8）   顶层窗口接收到关闭通知，根据之前设置的flag判断是否关闭窗口。
			（9）   顶层窗口销毁；
			（10）程序的OnBeforeClose处理回调函数执行，browser销毁.
			（11）如果不存在其他browser，则调用CefQuitMessageLoop退出程序。
		*/
		///
		/*--cef()--*/
		bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

		///
		// Called just before a browser is destroyed. Release all references to the
		// browser object and do not attempt to execute any methods on the browser
		// object (other than IsValid, GetIdentifier or IsSame) after this callback
		// returns. CefFrameHandler callbacks related to final main frame destruction
		// will arrive after this callback and CefBrowser::IsValid will return false
		// at that time. Any in-progress network requests associated with |browser|
		// will be aborted when the browser is destroyed, and
		// CefResourceRequestHandler callbacks related to those requests may still
		// arrive on the IO thread after this callback. See CefFrameHandler and
		// DoClose() documentation for additional usage information.
		// browser销毁之前会触发回调函数OnBeforeClose
		///
		/*--cef()--*/
		void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
#endif //! CefLifeSpanHandler



#if 1
		/*************************************** CefLoadHandler *************************************************************/
		// 在一个网页中加载内容，或者在网页中打开一个子frame，都可以拦截到iframe打开时的消息以及url等信息。可以拦截子网页url
		//（1）   开始加载OnLoadStart，navigation执行网之后，开始加载内容之前，回调此函数，多frame的进程会同时加载。同页面巡航不会调用。
		//（2）   加载结束OnLoadEnd，加载结束时回调，sub - frame在主frame加载结束后， 会继续开始加载或继续进行加载，同页面巡航不会调用。
		//（3）   加载错误OnLoadError，navigation失败或者取消是回调。
		//（4）   加载状态发生变化OnLoadingStateChange，加载初始化和加载结束时各调用一次，在OnLoadStart之前调用一次，OnLoadEnd或OnLoadError之后调用一次。

		///
		// Called when the loading state has changed. This callback will be executed
		// twice -- once when loading is initiated either programmatically or by user
		// action, and once when loading is terminated due to completion, cancellation
		// of failure. It will be called before any calls to OnLoadStart and after all
		// calls to OnLoadError and/or OnLoadEnd.
		// 当加载状态改变时调用。这个回调将被执行两次——一次是在通过编程或用户操作启动加载时，另一次是在加载因完成或取消失败而终止时。它将在调用OnLoadStart之前和在调用OnLoadError和/或OnLoadEnd之后被调用。
		///
		/*--cef()--*/
		void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
			bool isLoading,
			bool canGoBack,
			bool canGoForward) OVERRIDE;

		///
		// Called after a navigation has been committed and before the browser begins
		// loading contents in the frame. The |frame| value will never be empty --
		// call the IsMain() method to check if this frame is the main frame.
		// |transition_type| provides information about the source of the navigation
		// and an accurate value is only available in the browser process. Multiple
		// frames may be loading at the same time. Sub-frames may start or continue
		// loading after the main frame load has ended. This method will not be called
		// for same page navigations (fragments, history state, etc.) or for
		// navigations that fail or are canceled before commit. For notification of
		// overall browser load status use OnLoadingStateChange instead.
		///
		/*--cef()--*/
		void OnLoadStart(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			TransitionType transition_type) OVERRIDE {}

		///
		// Called when the browser is done loading a frame. The |frame| value will
		// never be empty -- call the IsMain() method to check if this frame is the
		// main frame. Multiple frames may be loading at the same time. Sub-frames may
		// start or continue loading after the main frame load has ended. This method
		// will not be called for same page navigations (fragments, history state,
		// etc.) or for navigations that fail or are canceled before commit. For
		// notification of overall browser load status use OnLoadingStateChange
		// instead.
		///
		/*--cef()--*/
		void OnLoadEnd(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			int httpStatusCode) OVERRIDE {}

		///
		// Called when a navigation fails or is canceled. This method may be called
		// by itself if before commit or in combination with OnLoadStart/OnLoadEnd if
		// after commit. |errorCode| is the error code number, |errorText| is the
		// error text and |failedUrl| is the URL that failed to load.
		// See net\base\net_error_list.h for complete descriptions of the error codes.
		///
		/*--cef(optional_param=errorText)--*/
		void OnLoadError(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			ErrorCode errorCode,
			const CefString& errorText,
			const CefString& failedUrl) OVERRIDE;
#endif //! CefLoadHandler


#if 1
		/**********************************  CefRequestHandler *******************************************************************/
		// 当打开一个网页， CefRequestHandler的OnBeforeBrowser可以拦截网络请求，只有在新打开网页的时候，才会触发，如果网页已经打开，在网页内部点击查询按钮，查询内容，虽然也有request请求，但是OnBeforeBrowser拦截不到获取请求的URL，post请求的参数都可以获取到。OnResourceRedirect还可以拦截重定向请求。CefLoadHandler也可以拦截request请求，而且页面加载中调用很多的GET和POST请求都可以拦截到。测试发现CefRequestHandler页面内部的加载变化是获取不到的，只有打开页面的请求能获取到。而另外一个函数OnBeforeResourceLoad则可以拦截所有的请求，在浏览器中F12显示的所有请求，包括图片下载等请求都能一一获取。所以CefLoadHandler拦截的请求更详细一些，点击查询查询，OnLoadStart和OnLoadEnd 拦截不到，但是OnLoadingStateChange 可以拦截的到请求。

		// OnBeforeBrowser 打开新的网页可以拦截，页面内容变化，或者页面内部调用请求拦截不到。
		// OnBeforeResourceLoad 拦截一切请求，最详细。
		// OnResourceResponse 拦截一切请求，最详细。
		// OnLoadStart和OnLoadEnd 新打开页面可以拦截
		// OnLoadingStateChange 打开新页面， 页面内容重新加载，查询，按钮响应可以拦截。像一些图片加载，CSS加载是拦截不到的。第二详细。

		///
		// Called on the UI thread before browser navigation. Return true to cancel
		// the navigation or false to allow the navigation to proceed. The |request|
		// object cannot be modified in this callback.
		// CefLoadHandler::OnLoadingStateChange will be called twice in all cases.
		// If the navigation is allowed CefLoadHandler::OnLoadStart and
		// CefLoadHandler::OnLoadEnd will be called. If the navigation is canceled
		// CefLoadHandler::OnLoadError will be called with an |errorCode| value of
		// ERR_ABORTED. The |user_gesture| value will be true if the browser
		// navigated via explicit user gesture (e.g. clicking a link) or false if it
		// navigated automatically (e.g. via the DomContentLoaded event).
		// 打开新的网页可以拦截，页面内容变化，或者页面内部调用请求拦截不到。
		// 在浏览器导航之前在UI线程上调用。返回true以取消导航，返回false以允许继续导航。在这个回调中不能修改|请求|对象。CefLoadHandler::OnLoadingStateChange将在所有情况下被调用两次。如果允许导航，CefLoadHandler::OnLoadStart和CefLoadHandler::OnLoadEnd将被调用。如果导航被取消，CefLoadHandler::OnLoadError将被调用，|errorCode|值为ERR_ABORTED。|user_gesture如果浏览器通过显式用户手势导航（例如单击链接），|值将为true；如果是自动导航（例如通过DomContentLoaded事件），|值将为false。
		///
		/*--cef()--*/
		bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request,
			bool user_gesture,
			bool is_redirect) OVERRIDE;
		///
		// Called on the UI thread before OnBeforeBrowse in certain limited cases
		// where navigating a new or different browser might be desirable. This
		// includes user-initiated navigation that might open in a special way (e.g.
		// links clicked via middle-click or ctrl + left-click) and certain types of
		// cross-origin navigation initiated from the renderer process (e.g.
		// navigating the top-level frame to/from a file URL). The |browser| and
		// |frame| values represent the source of the navigation. The
		// |target_disposition| value indicates where the user intended to navigate
		// the browser based on standard Chromium behaviors (e.g. current tab,
		// new tab, etc). The |user_gesture| value will be true if the browser
		// navigated via explicit user gesture (e.g. clicking a link) or false if it
		// navigated automatically (e.g. via the DomContentLoaded event). Return true
		// to cancel the navigation or false to allow the navigation to proceed in the
		// source browser's top-level frame.
		// 以特殊的方式打开的网页，例如鼠标中间按钮，快捷键等，一些很少的应用场景。
		///
		/*--cef()--*/
		bool OnOpenURLFromTab(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const CefString& target_url,
			CefRequestHandler::WindowOpenDisposition target_disposition,
			bool user_gesture) OVERRIDE;

		///
		// Called on the browser process IO thread before a resource request is
		// initiated. The |browser| and |frame| values represent the source of the
		// request. |request| represents the request contents and cannot be modified
		// in this callback. |is_navigation| will be true if the resource request is a
		// navigation. |is_download| will be true if the resource request is a
		// download. |request_initiator| is the origin (scheme + domain) of the page
		// that initiated the request. Set |disable_default_handling| to true to
		// disable default handling of the request, in which case it will need to be
		// handled via CefResourceRequestHandler::GetResourceHandler or it will be
		// canceled. To allow the resource load to proceed with default handling
		// return NULL. To specify a handler for the resource return a
		// CefResourceRequestHandler object. If this callback returns NULL the same
		// method will be called on the associated CefRequestContextHandler, if any.
		///
		/*--cef(optional_param=request_initiator)--*/
		CefRefPtr<CefResourceRequestHandler> GetResourceRequestHandler(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request,
			bool is_navigation,
			bool is_download,
			const CefString& request_initiator,
			bool& disable_default_handling) OVERRIDE {
			CEF_REQUIRE_IO_THREAD();
			return this;
		}
        ///
        // Called on the IO thread to optionally filter resource response content. The
        // |browser| and |frame| values represent the source of the request, and may
        // be NULL for requests originating from service workers or CefURLRequest.
        // |request| and |response| represent the request and response respectively
        // and cannot be modified in this callback.
        ///
        /*--cef(optional_param=browser,optional_param=frame)--*/
        CefRefPtr<CefResponseFilter> GetResourceResponseFilter(
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request,
            CefRefPtr<CefResponse> response) OVERRIDE;
        ///
        // Called on the IO thread to handle requests for URLs with an unknown
        // protocol component. The |browser| and |frame| values represent the source
        // of the request, and may be NULL for requests originating from service
        // workers or CefURLRequest. |request| cannot be modified in this callback.
        // Set |allow_os_execution| to true to attempt execution via the registered OS
        // protocol handler, if any.
        // SECURITY WARNING: YOU SHOULD USE THIS METHOD TO ENFORCE RESTRICTIONS BASED
        // ON SCHEME, HOST OR OTHER URL ANALYSIS BEFORE ALLOWING OS EXECUTION.
        ///
        /*--cef(optional_param=browser,optional_param=frame)--*/
        void OnProtocolExecution(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request,
            bool& allow_os_execution) OVERRIDE;


		///
		// Called on the IO thread before a resource request is loaded. The |browser|
		// and |frame| values represent the source of the request, and may be NULL for
		// requests originating from service workers or CefURLRequest. To redirect or
		// change the resource load optionally modify |request|. Modification of the
		// request URL will be treated as a redirect. Return RV_CONTINUE to continue
		// the request immediately. Return RV_CONTINUE_ASYNC and call
		// CefRequestCallback:: Continue() at a later time to continue or cancel the
		// request asynchronously. Return RV_CANCEL to cancel the request immediately.
		// OnBeforeResourceLoad网页开始加载资源时调用，可以拦截所有的请求，最为详细。
		// 在加载资源请求之前在IO线程上调用。|浏览器|和|帧|值表示请求的来源，对于来自service worker或CefURLRequest的请求可能为NULL。要重定向或更改资源负载，可选择修改|请求|。对请求URL的修改将被视为重定向。返回RV_CONTINUE以立即继续请求。返回RV_CONTINUE_ASYNC并在稍后的时间调用CefRequestCallback:: Continue（）以异步方式继续或取消请求。返回RV_CANCEL立即取消请求。
		///
		/*--cef(optional_param=browser,optional_param=frame,default_retval=RV_CONTINUE)--*/
		cef_return_value_t OnBeforeResourceLoad(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request,
			CefRefPtr<CefRequestCallback> callback) OVERRIDE;

		///
		// Called on the IO thread before a resource is loaded. The |browser| and
		// |frame| values represent the source of the request, and may be NULL for
		// requests originating from service workers or CefURLRequest. To allow the
		// resource to load using the default network loader return NULL. To specify a
		// handler for the resource return a CefResourceHandler object. The |request|
		// object cannot not be modified in this callback.
		///
		/*--cef(optional_param=browser,optional_param=frame)--*/
		CefRefPtr<CefResourceHandler> GetResourceHandler(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request) OVERRIDE;


		///
		// Called on the IO thread when a resource load is redirected. The |browser|
		// and |frame| values represent the source of the request, and may be NULL for
		// requests originating from service workers or CefURLRequest. The |request|
		// parameter will contain the old URL and other request-related information.
		// The |response| parameter will contain the response that resulted in the
		// redirect. The |new_url| parameter will contain the new URL and can be
		// changed if desired. The |request| and |response| objects cannot be modified
		// in this callback.
		// OnResourceRedirect重定向请求拦截
		// 当资源负载被重定向时在IO线程上调用。|浏览器|和|帧|值表示请求的来源，对于来自service worker或CefURLRequest的请求可能为NULL。|request|参数将包含旧的URL和其他与请求相关的信息。|响应|参数将包含导致重定向的响应。|new_url|参数将包含新的URL，并且可以根据需要进行更改。在这个回调中，|请求|和|响应|对象不能被修改。
		///
		/*--cef(optional_param=browser,optional_param=frame)--*/
		void OnResourceRedirect(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request,
			CefRefPtr<CefResponse> response,
			CefString& new_url) OVERRIDE {}

		///
		// Called on the IO thread when a resource response is received. The |browser|
		// and |frame| values represent the source of the request, and may be NULL for
		// requests originating from service workers or CefURLRequest. To allow the
		// resource load to proceed without modification return false. To redirect or
		// retry the resource load optionally modify |request| and return true.
		// Modification of the request URL will be treated as a redirect. Requests
		// handled using the default network loader cannot be redirected in this
		// callback. The |response| object cannot be modified in this callback.
		//
		// WARNING: Redirecting using this method is deprecated. Use
		// OnBeforeResourceLoad or GetResourceHandler to perform redirects.
		// 请求响应后的回调函数
		///
		/*--cef(optional_param=browser,optional_param=frame)--*/
		bool OnResourceResponse(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request,
			CefRefPtr<CefResponse> response) OVERRIDE {
			return false;
		}

		///
		// Called on the IO thread when a resource load has completed. The |browser|
		// and |frame| values represent the source of the request, and may be NULL for
		// requests originating from service workers or CefURLRequest. |request| and
		// |response| represent the request and response respectively and cannot be
		// modified in this callback. |status| indicates the load completion status.
		// |received_content_length| is the number of response bytes actually read.
		// This method will be called for all requests, including requests that are
		// aborted due to CEF shutdown or destruction of the associated browser. In
		// cases where the associated browser is destroyed this callback may arrive
		// after the CefLifeSpanHandler::OnBeforeClose callback for that browser. The
		// CefFrame::IsValid method can be used to test for this situation, and care
		// should be taken not to call |browser| or |frame| methods that modify state
		// (like LoadURL, SendProcessMessage, etc.) if the frame is invalid.
		// OnResourceLoadComplete资源加载结束时的回调
		///
		/*--cef(optional_param=browser,optional_param=frame)--*/
		void OnResourceLoadComplete(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request,
			CefRefPtr<CefResponse> response,
			URLRequestStatus status,
			int64 received_content_length) OVERRIDE {}

		///
		// Called on the IO thread when the browser needs credentials from the user.
		// |origin_url| is the origin making this authentication request. |isProxy|
		// indicates whether the host is a proxy server. |host| contains the hostname
		// and |port| contains the port number. |realm| is the realm of the challenge
		// and may be empty. |scheme| is the authentication scheme used, such as
		// "basic" or "digest", and will be empty if the source of the request is an
		// FTP server. Return true to continue the request and call
		// CefAuthCallback::Continue() either in this method or at a later time when
		// the authentication information is available. Return false to cancel the
		// request immediately.
		///
		/*--cef(optional_param=realm,optional_param=scheme)--*/
		bool GetAuthCredentials(CefRefPtr<CefBrowser> browser,
			const CefString& origin_url,
			bool isProxy,
			const CefString& host,
			int port,
			const CefString& realm,
			const CefString& scheme,
			CefRefPtr<CefAuthCallback> callback) OVERRIDE;

		///
		// Called on the IO thread when JavaScript requests a specific storage quota
		// size via the webkitStorageInfo.requestQuota function. |origin_url| is the
		// origin of the page making the request. |new_size| is the requested quota
		// size in bytes. Return true to continue the request and call
		// CefRequestCallback::Continue() either in this method or at a later time to
		// grant or deny the request. Return false to cancel the request immediately.
        //! 如果配额大小合理，则批准配额请求。
        // 当JavaScript通过webkitStorageInfo请求特定的存储配额大小时，在IO线程上调用。requestQuota函数。|是发出请求的页面的源。|new_size|为请求的配额大小，单位为字节。返回true以继续请求，并在此方法中或稍后调用CefRequestCallback:: continue（）以授予或拒绝请求。返回false以立即取消请求。
		/// 
		/*--cef()--*/
		bool OnQuotaRequest(CefRefPtr<CefBrowser> browser,
			const CefString& origin_url,
			int64 new_size,
			CefRefPtr<CefRequestCallback> callback) OVERRIDE;

		///
		// Called on the UI thread to handle requests for URLs with an invalid
		// SSL certificate. Return true and call CefRequestCallback::Continue() either
		// in this method or at a later time to continue or cancel the request. Return
		// false to cancel the request immediately. If
		// CefSettings.ignore_certificate_errors is set all invalid certificates will
		// be accepted without calling this method.
        // 在UI线程上调用，以处理对带有无效SSL证书的url的请求。返回true并在此方法中调用CefRequestCallback::Continue（）或在稍后的时间继续或取消请求。返回false以立即取消请求。如果CefSettings。Ignore_certificate_errors设置所有无效证书将
        //在不调用此方法的情况下被接受。
		///
		/*--cef()--*/
		bool OnCertificateError(CefRefPtr<CefBrowser> browser,
			cef_errorcode_t cert_error,
			const CefString& request_url,
			CefRefPtr<CefSSLInfo> ssl_info,
			CefRefPtr<CefRequestCallback> callback) OVERRIDE;

		///
		// Called on the UI thread when a client certificate is being requested for
		// authentication. Return false to use the default behavior and automatically
		// select the first certificate available. Return true and call
		// CefSelectClientCertificateCallback::Select either in this method or at a
		// later time to select a certificate. Do not call Select or call it with NULL
		// to continue without using any certificate. |isProxy| indicates whether the
		// host is an HTTPS proxy or the origin server. |host| and |port| contains the
		// hostname and port of the SSL server. |certificates| is the list of
		// certificates to choose from; this list has already been pruned by Chromium
		// so that it only contains certificates from issuers that the server trusts.
		///
		/*--cef()--*/
		bool OnSelectClientCertificate(
			CefRefPtr<CefBrowser> browser,
			bool isProxy,
			const CefString& host,
			int port,
			const X509CertificateList& certificates,
			CefRefPtr<CefSelectClientCertificateCallback> callback) OVERRIDE;

		///
		// Called on the browser process UI thread when a plugin has crashed.
		// |plugin_path| is the path of the plugin that crashed.
		///
		/*--cef()--*/
		void OnPluginCrashed(CefRefPtr<CefBrowser> browser,
			const CefString& plugin_path) OVERRIDE {}

		///
		// Called on the browser process UI thread when the render view associated
		// with |browser| is ready to receive/handle IPC messages in the render
		// process.
		///
		/*--cef()--*/
		void OnRenderViewReady(CefRefPtr<CefBrowser> browser) OVERRIDE {}

		///
		// Called on the browser process UI thread when the render process
		// terminates unexpectedly. |status| indicates how the process
		// terminated.
		///
		/*--cef()--*/
		void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
			TerminationStatus status) OVERRIDE;

		///
		// Called on the browser process UI thread when the window.document object of
		// the main frame has been created.
		///
		/*--cef()--*/
		void OnDocumentAvailableInMainFrame(CefRefPtr<CefBrowser> browser) OVERRIDE;
#endif //! CefRequestHandler

	public:
		// Include the default reference counting implementation.
		IMPLEMENT_REFCOUNTING(JGW_Cef3HelpHandler);
		DISALLOW_COPY_AND_ASSIGN(JGW_Cef3HelpHandler);
    public:
		void CloseAllBrowser(const bool &force_close = false);
		void CloseBrowser(CefRefPtr<CefBrowser> ptrBrowser,const bool &force_close = false);
		void CloseBrowserToBrowserId(int id,const bool &force_close = false);
		CefRefPtr<CefBrowser> GetBrowserByID(const INT32 &id);
		void QuitCefQuitMessageLoop();
        void SizeMsgHandler(RECT rect);
        HWND GetBrowserWindowHandleToId(int id);
        void ExecuteJavaScriptToId(int id,const std::wstring& strJsCode);
	private:
		/*
		通用消息路由器
		译者注语：这里给出了一种前端（JavaScript）调用客户端（C++）接口的方法，并且通用消息路由器是一种异步的方式。
		*/
		//! JS 代码调用C++ 代码消息路由
		CefRefPtr<CefMessageRouterBrowserSide> message_router_;
		// Set of Handlers registered with the message router.
		MessageHandlerSet message_handler_set_;

		/*
		通用资源管理器
		CEF 提供了一种通用实现，用于管理来自一个或多个数据源的资源请求。此用户为不同的数据源注册处理程序，例如磁盘上的目录、zip 存档或自定义实现，并且管理器处理请求。应用程序通过从标准 CEF C++ 回调（OnBeforeResourceLoad、GetResourceHandler）传递数据来与路由器交互。有关演示 CefResourceManager 用法的独立示例应用程序，请参阅resource_manager 示例。有关完整的使用文档，请参阅include/wrapper/cef_resource_manager.h。
		*/
		//
		CefRefPtr<CefResourceManager> resource_manager_;
		//!
		std::vector<CefRefPtr<CefBrowser>> mvCefBrowserList;
	private:
		bool m_is_closing;
		//! 是否打开开发者工具
		bool m_bOpenDevTools;
		//! 是否显示浏览器菜单栏
		bool m_bUseBrowserMenu;
		//! 
		bool m_is_focus_oneditable_field;
	};
}
