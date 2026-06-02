// Cef3Win32Demo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Cef3Win32Demo.h"
#include <include/cef_app.h>

#include <UiLibCef/UiLibCefExportManager.h>

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
HWND ghMainHwnd = NULL;;
// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

JGW::UiLibCefHelpInterface* gpUiLibCefHelpInterface;
//
//void SimpleHandler::PlatformTitleChange(CefRefPtr<CefBrowser> browser,
//    const CefString& title) {
//        CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
//        SetWindowText(hwnd, std::wstring(title).c_str());
//}

namespace JGW
{
	class SimpleUiLibCefMsgHandler : public UiLibCefMsgHandler
	{
	public:
		int mnBrowserID;
        bool mbIsClosing;
        SimpleUiLibCefMsgHandler() : mbIsClosing(false),mnBrowserID(-1){}

		//! 在创建浏览器时调用。
		virtual void OnBrowserCreated(int nBrowserID) override
		{
			OutputDebugStringA("OnBrowserCreated\n");
			mnBrowserID = nBrowserID;
		}

		//! Called when the browser is closing.
		//! 浏览器正在关闭时调用。
		virtual void OnBrowserClosing(int nBrowserID)  override
		{
			OutputDebugStringA("OnBrowserClosing\n");
            mbIsClosing = true;
		}

		// Called when the browser has been closed.
		//! 当浏览器已经关闭时调用
		virtual void OnBrowserClosed(int nBrowserID) override
		{
			OutputDebugStringA("OnBrowserClosed\n");
		}
		// Set the window URL address.
		//! 设置窗口URL地址。
		virtual void OnSetAddress(const std::string& url) override
		{
			OutputDebugStringA("OnSetAddress\n");
		}
		// Set the window title.
		//! 设置窗口标题。
		virtual void OnSetTitle(const std::string& title) override
		{
			OutputDebugStringA("OnSetTitle\n");
			SetWindowTextA(ghMainHwnd,title.c_str());
		}
		// Set fullscreen mode.
		//! 设置全屏模式。
		virtual void OnSetFullscreen(bool fullscreen)  override
		{
			OutputDebugStringA("OnSetFullscreen\n");
		}

		// Auto-resize contents.
		//! 自动调整大小的内容。
		virtual void OnAutoResize(int width_val, int height_val)  override
		{
			OutputDebugStringA("OnAutoResize\n");
		}

		// Set the loading state.
		//! 设置加载状态。
		virtual void OnSetLoadingState(bool isLoading,
			bool canGoBack,
			bool canGoForward)  override
		{
			OutputDebugStringA("OnSetLoadingState\n");
		}
		// Set focus to the next/previous control.
		//! 将焦点设置为下一个/上一个控件。
		virtual void OnTakeFocus(bool next)  override
		{
			OutputDebugStringA("OnTakeFocus\n");
		}
	};
}

JGW::SimpleUiLibCefMsgHandler gSimpleUiLibCefMsgHandler;


namespace JGW
{
    int CustomeJsCallCppFunc(const std::wstring& strUrl,const std::wstring& strRequest,std::wstring& strResponse)
    {
        if (0 == strRequest.compare(L"DialogTest.FileOpen"))
        {
            strResponse = L"1111111111111";
            return 1;
        } else if (0 == strRequest.compare(L"DialogTest.FileOpenMultiple"))
        {
            strResponse = L"Error DialogTest.FileOpenMultiple";
            return -1;
        }
        return 0;
    }
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CEF3WIN32DEMO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

    gpUiLibCefHelpInterface = JGW::GetUiLibCefHelp();
    gpUiLibCefHelpInterface->RegisterCppFunc(JGW::CustomeJsCallCppFunc);
	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CEF3WIN32DEMO));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
    Sleep(10 * 1000);
    //gpUiLibCefHelpInterface->UnInitialize();
    gpUiLibCefHelpInterface->UnRegisterCppFunc(JGW::CustomeJsCallCppFunc);
    gpUiLibCefHelpInterface->UnInitialize();
    DestroyWiFiSetting(gpUiLibCefHelpInterface);

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CEF3WIN32DEMO));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CEF3WIN32DEMO);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}
#if 0
//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
#else
//CefRefPtr<SimpleHandler> g_handler;

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    //初始化
#if 0
    void *sandbox_info = NULL;
    CefMainArgs main_args(hInstance);
    int exit_code = CefExecuteProcess(main_args, NULL, sandbox_info);
    if (exit_code >= 0)
    {
        return exit_code;
    }

    //关闭沙箱，浏览器初始化
    CefSettings settings;
    settings.no_sandbox = true;
    settings.multi_threaded_message_loop = true;
    CefRefPtr<SimpleApp> app(new SimpleApp);
    CefInitialize(main_args, settings, app.get(), sandbox_info);
#else
    if (!gpUiLibCefHelpInterface->Initialize())
    {
        return FALSE;
    }
#endif

    hInst = hInstance; // 将实例句柄存储在全局变量中

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	ghMainHwnd = hWnd;
    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
#if 0
    CefRefPtr<SimpleHandler> _handler(new SimpleHandler(false));
    g_handler = _handler;
    CefWindowInfo window_info;
    RECT rect;
    GetWindowRect(hWnd, &rect);
    int w = rect.right - rect.left;
    int h = rect.bottom - rect.top;
    w = std::abs(w);
    h = std::abs(h);
    rect.left = 0;
    rect.right = w;
    rect.top = 0;
    rect.bottom = h;
    window_info.SetAsChild(hWnd, rect);
    //window_info.SetAsPopup(NULL, "hello");
    CefBrowserSettings browser_settings;
    CefBrowserHost::CreateBrowser(window_info, g_handler.get(), "http://www.baidu.com", browser_settings, NULL, NULL);
#else
    RECT rect;
    //GetWindowRect(hWnd, &rect);
    GetClientRect(hWnd, &rect);
    int w = rect.right - rect.left;
    int h = rect.bottom - rect.top;
    w = std::abs(w);
    h = std::abs(h);
    rect.left = 0;
    rect.right = w;
    rect.top = 0;
    rect.bottom = h;

    //gpUiLibCefHelpInterface->LoadCef(hWnd,&gSimpleUiLibCefMsgHandler, L"file:///G:/JGW_PrsVendorRoamingToolV1.0/html/prs_connect_status_d.html",rect);
    gpUiLibCefHelpInterface->LoadCef(hWnd,&gSimpleUiLibCefMsgHandler, L"file:///G:/CEF/cef_binary_83.5.0+gbf03589+chromium-83.0.4103.106_windows32/tests/cefclient/resources/dialogs.html",rect);
    
    //gpUiLibCefHelpInterface->LoadCef(hWnd,L"http://www.baidu.com",rect);
#endif


    return TRUE;
}

#endif

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
    static bool bClose = false;

	switch (message)
	{
    case WM_CREATE:
        {
            RECT rect;
            GetClientRect(hWnd, &rect);
            gpUiLibCefHelpInterface->SizeMsgHandler(rect);
        }
        break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			//gpUiLibCefHelpInterface->CloseBrowserToId(gSimpleUiLibCefMsgHandler.mnBrowserID,true);
			gpUiLibCefHelpInterface->CloseAllBrowser();
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			//gpUiLibCefHelpInterface->QuitCefQuitMessageLoop();
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
    case WM_SIZE:
        {
            RECT rect;
            GetClientRect(hWnd, &rect);
            gpUiLibCefHelpInterface->SizeMsgHandler(rect);
        }
        break;
    case WM_CLOSE:
#if 0
        if (bClose) {
		    //gpUiLibCefHelpInterface->CloseBrowserToId(gSimpleUiLibCefMsgHandler.mnBrowserID,true);
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        gpUiLibCefHelpInterface->CloseAllBrowser();
        bClose = true;
#else
        if (!gSimpleUiLibCefMsgHandler.mbIsClosing)
        {
            gpUiLibCefHelpInterface->CloseAllBrowser();
        }
        else
        {
            DestroyWindow(hWnd);
        }  
#endif
        
		//ptrBrowser->GetHost()->TryCloseBrowser()
        //CefShutdown();
        //CefDoMessageLoopWork();
		//gpUiLibCefHelpInterface->CloseBrowserToId(gSimpleUiLibCefMsgHandler.mnBrowserID);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		//CefShutdown();
        //gpUiLibCefHelpInterface->UnInitialize();
		PostQuitMessage(0);
		break;
	default:
        
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
