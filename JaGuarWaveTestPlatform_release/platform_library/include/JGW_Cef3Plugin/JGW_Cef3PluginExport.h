#pragma once
#ifndef JGW_CEF3PLUGIN_H__
#define JGW_CEF3PLUGIN_H__
#include <string>
#if (defined JGW_CEF3PLUGIN_EXPORTS) || (defined JGW_CEF3PLUGIN_IMPORTS)
#else
#define JGW_CEF3PLUGIN_IMPORTS
#endif


#ifdef JGW_CEF3PLUGIN_EXPORTS
#	define JGW_EXTERN_
#	if _MSC_VER >= 1500
#		define JGW_CEF3PLUGIN_API __declspec(dllexport)
#		define JGW_CEF3PLUGIN_CLASS __declspec(dllexport)
#	else
#		define JGW_CEF3PLUGIN_API 
#		define JGW_CEF3PLUGIN_CLASS 
#	endif
#endif

#ifdef JGW_CEF3PLUGIN_IMPORTS
#		define JGW_EXTERN_
#		if _MSC_VER >= 1500
#			define JGW_CEF3PLUGIN_API __declspec(dllimport)
#			define JGW_CEF3PLUGIN_CLASS __declspec(dllimport)
#		else
#			define JGW_CEF3PLUGIN_API 
#			define JGW_CEF3PLUGIN_CLASS 
#		endif
#			ifdef _DEBUG
#				pragma comment(lib,"JGW_Cef3Plugin_d.lib")
#			else
#				pragma comment(lib,"JGW_Cef3Plugin.lib")
#			endif
#endif

#include <Windows.h>

typedef const wchar_t* (*pCppFuncCallback)(const wchar_t*);

namespace JGW
{
    class CCJGW_Cef3BrowserEventDelegate
    {
    public:
        virtual void OnBrowserCreate(int nBrowserID,const std::wstring& strUrl) = 0;
    };
}

#ifdef __cplusplus
extern "C" 
{
#endif
    /*
    CCJGW_Cef3ClientHandler 只有一个实例
    创建browser的时候，发送浏览器ID
    怎么向浏览器注册window object.(将字符串以“window.myval”的形式添加到window对象。请参阅下面的“JS对象”一节)
    URL,Param Name,Param Value 从浏览器进程发送到渲染进程

    C++ 执行JS函数
    CefRefPtr<CefBrowser> browser = ...;
    CefRefPtr<CefFrame> frame = browser->GetMainFrame();
    frame->ExecuteJavaScript("alert('ExecuteJavaScript works!');",
    frame->GetURL(), 0);

    注册全局变量所有frame有效 <=> Param Name,Param Value 从浏览器进程发送到渲染进程
    
    注册C++函数  浏览器进程将函数名称注册到渲染进程，渲染进程然后注册



    注册C++函数到JS，然后JS调用C++注册函数，渲染进程开始执行调用注册函数，然后将回调函数名称发送给浏览器进程，浏览器进程执行ExecuteJavaScript函数实现回调功能
    注册C++函数到JS，然后JS调用C++注册函数，渲染进程开始执行调用注册函数，然后将回调函数名称发送给浏览器进程，浏览器进程将调用的函数的结果转发给渲染进程，然后渲染进程执行回调函数

    */
    //! 配置主子进程，可以分离浏览器和渲染进程，渲染子进程
    JGW_CEF3PLUGIN_API int ExecuteSubProcesses(HINSTANCE hInstance);
    //! 初始化CEF3进程
    //! hInstance : 程序模块句柄
    //! bMultiThreadedMessageLoop设置为 true 让浏览器处理消息循环在单独的线程中运行 设置 CefSettings.multi_threaded_message_loop = true（仅限 Windows 和 Linux）。这将导致 CEF 在与主应用程序线程不同的线程上运行浏览器 UI 线程。使用这种方法，既不需要调用 CefDoMessageLoopWork() 也不需要调用 CefRunMessageLoop()。CefInitialize() 和 CefShutdown() 仍应在主应用程序线程上调用。您将需要提供自己的机制来与主应用程序线程通信（例如，请参阅 cefclient_win.cpp 中的消息窗口用法）。您可以在 Windows 或 Linux 上的 cefclient 中通过使用“--multi-threaded-message-loop”命令行标志运行来测试此模式。
    //! strSubProcessesPath 子进程绝对路径
    JGW_CEF3PLUGIN_API bool InitializeCef3Processes(HINSTANCE hInstance,bool bMultiThreadedMessageLoop,const wchar_t* strSubProcessesPath,const std::wstring& cef_temp_path = L"cef_temp");
    //! 创建浏览器实例
    JGW_CEF3PLUGIN_API void CreateBrowserHelp(HWND hWnd,RECT rect,const wchar_t* strUrl,int nMainIconResId = 107);
    //! 设置HTML资源为ZIP资源类型
    JGW_CEF3PLUGIN_API bool SetZipResResource(HINSTANCE hModuleHandle, DWORD dwResourceNameID, const wchar_t* strResourceType,const wchar_t* szPassword = NULL);
    //! 设置HTML资源为ZIP文件类型
    JGW_CEF3PLUGIN_API bool SetZipFilePath(const wchar_t* strZipFilePath,const wchar_t* szZipPassWord = NULL);
    //! 关闭所有浏览器
    JGW_CEF3PLUGIN_API void CloseAllBrowsers(bool force_close);
    //! 是否关闭CEF3浏览器
    JGW_CEF3PLUGIN_API bool IsCloseCef3Browser();
    //! 这个函数应该在主应用程序线程上调用，以便在应用程序退出之前关闭CEF浏览器进程。
    JGW_CEF3PLUGIN_API void UnInitializeCef3Processes();
    //! 注册C++函数到JS中
    JGW_CEF3PLUGIN_API void RegisterCppFuncToJs(const wchar_t* strFuntionName,pCppFuncCallback pCppFunc);
    //! 
    JGW_CEF3PLUGIN_API int GetBrowserIdToUrl(const wchar_t* strUrl);
    //! 返回值 true : 执行 false : nBrowserId不存在
    JGW_CEF3PLUGIN_API bool ExecuteJaveScript(int nBrowserId,const std::wstring& strJsCode);
    //! 添加注册browser事件委托
    JGW_CEF3PLUGIN_API void AddBrowserEventDelegate(JGW::CCJGW_Cef3BrowserEventDelegate* pCef3BrowserEventDelegate);
    //! 移除已注册browser事件委托
    JGW_CEF3PLUGIN_API void RemoveBrowserEventDelegate(JGW::CCJGW_Cef3BrowserEventDelegate* pCef3BrowserEventDelegate);
    //! C++调用JS函数
    //! OnAfterCreated 创建浏览器，可以向外发送浏览器ID
    //! DoClose => OnBeforeClose 调用顺序
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

#ifdef __cplusplus
};
#endif

#endif