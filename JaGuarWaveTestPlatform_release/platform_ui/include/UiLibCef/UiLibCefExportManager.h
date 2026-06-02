#pragma once
#include "UiLibCefDefine.h"
#include <string>

namespace JGW
{
    class UiLibCefMsgHandler
    {
	public:
        //! 在创建浏览器时调用。
        virtual void OnBrowserCreated(int nBrowserID) = 0;

        //! Called when the browser is closing.
        //! 浏览器正在关闭时调用。
        virtual void OnBrowserClosing(int nBrowserID) = 0;

        // Called when the browser has been closed.
        //! 当浏览器已经关闭时调用
        virtual void OnBrowserClosed(int nBrowserID) {}

        // Set the window URL address.
        //! 设置窗口URL地址。
        virtual void OnSetAddress(const std::string& url) {}

        // Set the window title.
        //! 设置窗口标题。
        virtual void OnSetTitle(const std::string& title) {}
#if 0
        // Set the Favicon image.
        //! Set the Favicon image.
        virtual void OnSetFavicon(CefRefPtr<CefImage> image) {}
#endif
        // Set fullscreen mode.
        //! 设置全屏模式。
        virtual void OnSetFullscreen(bool fullscreen) {}

        // Auto-resize contents.
        //! 自动调整大小的内容。
        virtual void OnAutoResize(int width_val, int height_val) {}

        // Set the loading state.
        //! 设置加载状态。
        virtual void OnSetLoadingState(bool isLoading,
            bool canGoBack,
            bool canGoForward) {}

        // Set the draggable regions.
        //! 设置可拖动区域。
#if 0
        virtual void OnSetDraggableRegions(
            const std::vector<CefDraggableRegion>& regions);
#endif
        // Set focus to the next/previous control.
        //! 将焦点设置为下一个/上一个控件。
        virtual void OnTakeFocus(bool next) {}
#if 0
        // Called on the UI thread before a context menu is displayed.
        //! 在显示上下文菜单之前在UI线程上调用。
        virtual void OnBeforeContextMenu(CefRefPtr<CefMenuModel> model);
#endif
    };
    //! JS 调用C++函数回调
    //! 1 : 成功 
    //! 0 : 当前JS调用C++函数不存在 
    //!-1 : 失败ErrorCode
    typedef int (*JsCallCppFuncBK)(const std::wstring& strUrl,const std::wstring& strRequest,std::wstring& strResponse);

    class UiLibCefHelpInterface
    {
    public:
        /**
        * 初始化cef组件
        * @param[in] cef_temp_path cef临时文件路径
        * @param[in] is_enable_offset_render 是否开启离屏渲染
        * @return bool true 继续运行，false 应该结束程序
        */
        virtual bool Initialize(const std::wstring &cef_temp_path = L"cef_temp", const bool &is_enable_offset_render = false) = 0;
        /*
        * 注册JS函数，供JS调用，C++函数
        *
        */
        virtual void AddCefRendererDelegate(void* pCefRendererDelegate) = 0;
        /**
        * @brief 加载cef
        * @param[in] hwnd 窗口句柄
        * @param[in] windowName 窗口名
        * @param[in] url html路径（网址）
        * @param[in] rect 浏览器所显示的窗口区域
        * @param[in] bUseBrowserMenu 是否启用浏览器右键菜单
        * @return 返回 true 表示加载成功，false 加载失败
        */
        virtual bool LoadCef(const HWND &hwnd,UiLibCefMsgHandler* pUiLibCefMsgHandler, const std::wstring &url, const RECT &rect, const bool &bUseBrowserMenu = false) = 0;

        virtual HWND GetBrowserWindowHandleToId(int id) = 0;
		//!
		virtual void CloseBrowserToId(int id,const bool &bForceClose = false) = 0;
		//! 
		virtual void CloseAllBrowser(const bool &bForceClose = false) = 0;

		virtual void QuitCefQuitMessageLoop() = 0;

        virtual void SizeMsgHandler(RECT rect) = 0;
        //! frame->ExecuteJavaScript("alert('" + msg + "');", frame->GetURL(), 0);
        //! 通过IPC机制将JS代码发送到渲染进程执行，实际执行仍发生在渲染进程的TID_RENDERER线程
        //! TID_RENDERER
        virtual void ExecuteJavaScriptToId(int id,const std::wstring& strJsCode) = 0;
        /**
        * 清理cef组件
        * @return void	无返回值
        */
        virtual void UnInitialize() = 0;
        //! 注册C++回调函数
        virtual void RegisterCppFunc(JsCallCppFuncBK pJsCallCppFuncBK) = 0;
        //! 取消注册C++回调函数
        virtual void UnRegisterCppFunc(JsCallCppFuncBK pJsCallCppFuncBK) = 0;
        //! 添加JS变量 PROPERTY
        //virtual void CreatePropertyString(const std::wstring& strPropertyName,const std::wstring& strPropertyValue) = 0;
        //! 移除JS变量
        //virtual void RemovePropertyString(const std::wstring& strPropertyName) = 0;
    };

#ifdef __cplusplus
    extern "C" 
    {
#endif
        UILIBCEF_API UiLibCefHelpInterface* GetUiLibCefHelp();

        UILIBCEF_API void DestroyWiFiSetting(UiLibCefHelpInterface* pUiLibCefHelpInterface);
#ifdef __cplusplus
    };
#endif
}
