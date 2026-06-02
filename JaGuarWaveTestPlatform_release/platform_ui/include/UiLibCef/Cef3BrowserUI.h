#pragma once
#include "UiLibCefExportManager.h"
#include <UiLib/UIlib.h>

#define CEF3_BROWSER_UI_CLASS_NAME L"Cef3BrowserUI"
#define CEF3_BROWSER_UI_INTERFACE L"Cef3BrowserUI"

namespace JGW
{
    ////类名称
    //const TCHAR CCefBrowserUIClassName[] = L"CefBrowserUI";		
    ////控件名称
    //const TCHAR CCefBrowserUIItemInterface[] = L"CefBrowser";	

    class CCef3BrowserUI : public CControlUI,public UiLibCefMsgHandler
    {
    public:
        CCef3BrowserUI(void);
        ~CCef3BrowserUI(void);
    protected:
        LPCTSTR GetClass() const;
        LPVOID GetInterface(LPCTSTR pstrName);
        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
        void DoInit();
        //设置位置
        virtual void SetPos(RECT rc);
        //父容器调用设置显示与否
        virtual void SetInternVisible(bool bVisible = true);
        //设置是否显示
        virtual void SetVisible(bool bVisible = true);
    private:
        //! 
        void OnBrowserCreated(int nBrowserID) override;
        //! 
        void OnBrowserClosing(int nBrowserID) override;
    private:
        //!
        bool mbIsCreated;
        //! 父窗口句柄
        HWND mhParentHwnd;
        //! 
        int mnBrowserId;
        //! 浏览器名称
        std::wstring browser_name_;
        //! 默认地址
        std::wstring url_;
        //! 
        UiLibCefHelpInterface* mpUiLibCefHelpInterface;
    };

}
