#include "StdAfx.h"
#include <UiLibCef/Cef3BrowserUI.h>
#include <include/internal/cef_win.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCef3BrowserUI::CCef3BrowserUI(void) : mnBrowserId(-1),mbIsCreated(false),mhParentHwnd(NULL)
    {
    }


    CCef3BrowserUI::~CCef3BrowserUI(void)
    {
    }

    LPCTSTR CCef3BrowserUI::GetClass() const
    {
        return CEF3_BROWSER_UI_CLASS_NAME;
    }

    LPVOID CCef3BrowserUI::GetInterface(LPCTSTR pstrName)
    {
        if( _tcscmp(pstrName, CEF3_BROWSER_UI_INTERFACE) == 0 ) 
            return static_cast<CCef3BrowserUI*>(this);  
        return CControlUI::GetInterface(pstrName);  
    }

    void CCef3BrowserUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) 
    {
        if ( _tcscmp(pstrName, _T("name")) == 0)
            browser_name_ = pstrValue;
        else if( _tcscmp(pstrName, _T("url")) == 0 ) 
            url_ = pstrValue;  
        CControlUI::SetAttribute(pstrName, pstrValue);  
    }

    void CCef3BrowserUI::DoInit()
    {
        if (!url_.empty())
        {
            RECT rect = {0};
            mpUiLibCefHelpInterface->LoadCef(mhParentHwnd,this,url_,rect);
            mbIsCreated = true;
        }
    }

    void CCef3BrowserUI::SetPos(RECT rc)
    {
        CControlUI::SetPos(rc);
#if 0
        CefWindowHandle hwnd = mpUiLibCefHelpInterface->GetBrowserWindowHandleToId(gSimpleUiLibCefMsgHandler.mnBrowserID);
        if (hwnd != NULL) {
            ::SetWindowPos(hwnd, NULL, rc.left, rc.top, rc.right - rc.left, 
                rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);   
        }
#else
        mpUiLibCefHelpInterface->SizeMsgHandler(rc);
#endif
    }

    void CCef3BrowserUI::SetVisible(bool bVisible) 
    {
        CControlUI::SetVisible(bVisible);
        if (mbIsCreated) 
        {
            CefWindowHandle hwnd = mpUiLibCefHelpInterface->GetBrowserWindowHandleToId(mnBrowserId);
            if( hwnd != NULL) 
                ::ShowWindow(hwnd, IsVisible() ? SW_SHOW : SW_HIDE);
        }
    }

    void CCef3BrowserUI::SetInternVisible(bool bVisible) 
    {
        CControlUI::SetInternVisible(bVisible);
        if (mbIsCreated) 
        {
            CefWindowHandle hwnd = mpUiLibCefHelpInterface->GetBrowserWindowHandleToId(mnBrowserId);
            if( hwnd != NULL) 
                ::ShowWindow(hwnd, IsVisible() ? SW_SHOW : SW_HIDE);
        }
    }

    void CCef3BrowserUI::OnBrowserCreated(int nBrowserID)
    {
        mnBrowserId = nBrowserID;
    }

    void CCef3BrowserUI::OnBrowserClosing(int nBrowserID)
    {

    }
}

