#ifndef UIlib_h__
#define UIlib_h__

//! 开启使用ZIP资源开关
#ifndef  _DEBUG
 	#define USE_ZIP_RESOURCE_ENABLE
#endif
//! 开启视同ZIP文件开关
//! #define USE_ZIP_FILE_ENABLE

#if defined(USE_ZIP_RESOURCE_ENABLE) && defined(USE_ZIP_FILE_ENABLE)
#error zip资源和zip文件不能同时定义只能使用一个
#endif

#ifndef IDR_ZIPRES1
#define IDR_ZIPRES1                     129
#endif

#if defined(UILIB_EXPORTS)
#if defined(_MSC_VER)
#define UILIB_API __declspec(dllexport)
#else
#define UILIB_API 
#endif
#else
#if defined(_MSC_VER)
#define UILIB_API __declspec(dllimport)
#else
#define UILIB_API 
#endif
#endif
/*
#ifndef UILIB_EXPORTS
#	define UILIB_API __declspec(dllimport)
#else
#	ifdef UILIB_EXPORTS
#		if _MSC_VER >= 1500
#			define UILIB_API __declspec(dllexport)
#		else
#			define UILIB_API 
#		endif
#	else
#		if _MSC_VER >= 1500
#			define UILIB_API __declspec(dllimport)
#		else
#			define UILIB_API 
#		endif
#	endif
#endif
*/
#define UILIB_COMDAT __declspec(selectany) 
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
#define     MapWindowRect(hwndFrom, hwndTo, lprc) \
	MapWindowPoints((hwndFrom), (hwndTo), (POINT *)(lprc), 2)
#define     SetWindowFont(hwnd, hfont, fRedraw) FORWARD_WM_SETFONT((hwnd), (hfont), (fRedraw), SNDMSG)
#define Edit_GetText(hwndCtl, lpch, cchMax)     GetWindowText((hwndCtl), (lpch), (cchMax))
#define Edit_GetTextLength(hwndCtl)             GetWindowTextLength(hwndCtl)
#define Edit_SetText(hwndCtl, lpsz)             SetWindowText((hwndCtl), (lpsz))
#define Edit_SetModify(hwndCtl, fModified)      ((void)SNDMSG((hwndCtl), EM_SETMODIFY, (WPARAM)(UINT)(fModified), 0L))
#define Edit_LimitText(hwndCtl, cchMax)         ((void)SNDMSG((hwndCtl), EM_LIMITTEXT, (WPARAM)(cchMax), 0L))
#define Edit_SetPasswordChar(hwndCtl, ch)       ((void)SNDMSG((hwndCtl), EM_SETPASSWORDCHAR, (WPARAM)(UINT)(ch), 0L))
#define GET_WM_COMMAND_CMD(wp, lp)              HIWORD(wp)
#define     SubclassWindow(hwnd, lpfn)       \
	((WNDPROC)SetWindowLongPtr((hwnd), GWLP_WNDPROC, (LPARAM)(WNDPROC)(lpfn)))
#define     GetWindowOwner(hwnd)    GetWindow(hwnd, GW_OWNER)
#define     GetWindowStyle(hwnd)    ((DWORD)GetWindowLong(hwnd, GWL_STYLE))
#define     GetWindowExStyle(hwnd)  ((DWORD)GetWindowLong(hwnd, GWL_EXSTYLE))
#define     GetWindowOwner(hwnd)    GetWindow(hwnd, GW_OWNER)
#define FORWARD_WM_SETFONT(hwnd, hfont, fRedraw, fn) \
	(void)(fn)((hwnd), WM_SETFONT, (WPARAM)(HFONT)(hfont), (LPARAM)(BOOL)(fRedraw))
#define Edit_Enable(hwndCtl, fEnable)           EnableWindow((hwndCtl), (fEnable))
#define Edit_SetReadOnly(hwndCtl, fReadOnly)    ((BOOL)(DWORD)SNDMSG((hwndCtl), EM_SETREADONLY, (WPARAM)(BOOL)(fReadOnly), 0L))
#define Edit_SetSel(hwndCtl, ichStart, ichEnd)  ((void)SNDMSG((hwndCtl), EM_SETSEL, (ichStart), (ichEnd)))
#define Edit_ReplaceSel(hwndCtl, lpszReplace)   ((void)SNDMSG((hwndCtl), EM_REPLACESEL, 0L, (LPARAM)(LPCTSTR)(lpszReplace)))



#include <windows.h>
/*#include <windowsx.h>*/
#include <commctrl.h>
#include <stddef.h>
#include <richedit.h>
#include <tchar.h>
#include <assert.h>
#include <crtdbg.h>
#include <malloc.h>
#include <atlbase.h>
#include <atlstr.h>
#include <xstring>
#include <comdef.h>
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#include <gdiplus.h>

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

#pragma comment(lib,"oledlg.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"gdiplus.lib")

#include "Utils/DuiPointer.h"
#include "Utils/Utils.h"
#include "Utils/UIShadow.h"
#include "Utils/Internal.h"
#include "Utils/UIDelegate.h"
#include "Utils/DuiAutoComplete.h"
#include "Utils/DuiTrayIcon.h"

#include "Core/UITimer.h"
#include "Core/UIDefine.h"
#include "Core/UIBase.h"
#include "Core/UIManager.h"
#include "Core/UIDxAnimation.h"

#include "Core/UIControl.h"
#include "Core/UIContainer.h"
#include "Core/UIMarkup.h"
#include "Core/UIDlgBuilder.h"
#include "Core/UIRender.h"

#include "Layout/UIVerticalLayout.h"
#include "Layout/UIHorizontalLayout.h"
#include "Layout/UITileLayout.h"
#include "Layout/UITabLayout.h"
#include "Layout/UIChildLayout.h"

#include "Control/ListTestCtrl.h"
#include "Control/UIList.h"
#include "Control/UICombo.h"
#include "Control/UIScrollBar.h"
#include "Control/UITreeView.h"
#include "Control/UIChartView.h"

#include "Control/UILabel.h"
#include "Control/UIText.h"
#include "Control/UIEdit.h"
#include "Control/UIGifAnim.h"
#include "Control/UIBarcode.h"

#include <vector>
#include <algorithm>
#include "Control/UIAnimation.h"
#include "Layout/UIAnimationTabLayout.h"
#include "Control/UIFadeButton.h"
#include "Control/UIButton.h"
#include "Control/UIOption.h"
#include "Control/UICheckBox.h"

#include "Control/UIProgress.h"
#include "Control/UISlider.h"

#include "Control/UIComboBox.h"
#include "Control/UIRichEdit.h"
#include "Control/UIDateTime.h"

#include "Control/UIActiveX.h"
#include "Control/UIWebBrowser.h"
#include "Control/UIFlash.h"

#include "Control/UIVerticalLayoutUIProjectBtn.h"

#include "Control/UIMenu.h"

#include "Utils/WinImplBase.h"
#include "Utils/IWindowBase.h"
//#include "HDuilibWnd.h"
namespace DuiLib = UiLib;
//namespace JGW = UiLib;

#define CLOSEBTN	 _T("closebtn")
#define MAXBTN		 _T("maxbtn")
#define MINBTN		 _T("minbtn")
#define RESTORBTN	 _T("restorebtn")

#ifndef UILIB_EXPORTS
#   if _DEBUG
#		pragma comment(lib,"UiLib_d.lib")
#   else
#		pragma comment(lib,"UiLib.lib")
#   endif
#endif



#endif // UIlib_h__
