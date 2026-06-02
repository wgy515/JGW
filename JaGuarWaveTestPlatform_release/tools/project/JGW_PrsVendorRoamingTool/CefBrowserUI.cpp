#include "StdAfx.h"
#include "CefBrowserUI.h"

#include <include/internal/cef_win.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

extern JGW::UiLibCefHelpInterface* gpUiLibCefHelpInterface;

namespace JGW
{
    CCefBrowserUI::CCefBrowserUI(HWND hParentHwnd) : mhParentHwnd(hParentHwnd)
    {
    }

    CCefBrowserUI::~CCefBrowserUI(void)
    {
    }

    LPCTSTR CCefBrowserUI::GetClass() const 
    {
        return CCefBrowserUIClassName;
    }

    LPVOID CCefBrowserUI::GetInterface(LPCTSTR pstrName)
    {
        if( _tcscmp(pstrName, CCefBrowserUIItemInterface) == 0 ) 
            return static_cast<CCefBrowserUI*>(this);  
        return CControlUI::GetInterface(pstrName);  
    }

    void CCefBrowserUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) 
    {
        if ( _tcscmp(pstrName, _T("name")) == 0)
            browser_name_ = pstrValue;
        else if( _tcscmp(pstrName, _T("url")) == 0 ) 
            url_ = pstrValue;  
        CControlUI::SetAttribute(pstrName, pstrValue);  
    }

    void CCefBrowserUI::DoInit()
    {
        if (!url_.empty())
        {
            RECT rect = {0};
            gpUiLibCefHelpInterface->LoadCef(mhParentHwnd,&mcSimpleUiLibCefMsgHandler,url_,rect);
            mbIsCreated = true;
        }
    }

    void CCefBrowserUI::SetPos(RECT rc)
    {
        CControlUI::SetPos(rc);
#if 0
        CefWindowHandle hwnd = gpUiLibCefHelpInterface->GetBrowserWindowHandleToId(gSimpleUiLibCefMsgHandler.mnBrowserID);
        if (hwnd != NULL) {
            ::SetWindowPos(hwnd, NULL, rc.left, rc.top, rc.right - rc.left, 
                rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);   
        }
#else
        gpUiLibCefHelpInterface->SizeMsgHandler(rc);
#endif
    }

    void CCefBrowserUI::SetVisible(bool bVisible) 
    {
        CControlUI::SetVisible(bVisible);
        if (mbIsCreated) 
        {
            CefWindowHandle hwnd = gpUiLibCefHelpInterface->GetBrowserWindowHandleToId(mcSimpleUiLibCefMsgHandler.mnBrowserID);
            if( hwnd != NULL) 
                ::ShowWindow(hwnd, IsVisible() ? SW_SHOW : SW_HIDE);
        }
    }

    void CCefBrowserUI::SetInternVisible(bool bVisible) 
    {
        CControlUI::SetInternVisible(bVisible);
        if (mbIsCreated) 
        {
            CefWindowHandle hwnd = gpUiLibCefHelpInterface->GetBrowserWindowHandleToId(mcSimpleUiLibCefMsgHandler.mnBrowserID);
            if( hwnd != NULL) 
                ::ShowWindow(hwnd, IsVisible() ? SW_SHOW : SW_HIDE);
        }
    }

    void CCefBrowserUI::ChangeLegendData(int nIndex,const std::wstring& strStaMac)
    {
        if (mbIsCreated && mcSimpleUiLibCefMsgHandler.mnBrowserID > 0) 
        {
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"ChangeLegendData(%d,'%s');",nIndex,strStaMac.c_str());
            gpUiLibCefHelpInterface->ExecuteJavaScriptToId(mcSimpleUiLibCefMsgHandler.mnBrowserID,strTemp);
        }   
    }

    void CCefBrowserUI::AddYSeries(const std::wstring& strStaMac)
    {
        if (mbIsCreated && mcSimpleUiLibCefMsgHandler.mnBrowserID > 0) 
        {
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"AddSeriesData('%s');",strStaMac.c_str());
            gpUiLibCefHelpInterface->ExecuteJavaScriptToId(mcSimpleUiLibCefMsgHandler.mnBrowserID,strTemp);
        }   
    }

    void CCefBrowserUI::AddYSeriesData(const std::wstring& strMac,__int64 nTimeStampMsec,int nPrsRoamingConnectStatus,const std::wstring& strDes)
    {
        if (mbIsCreated && mcSimpleUiLibCefMsgHandler.mnBrowserID > 0) 
        {
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"AddLegendDataToLegendName('%s',%I64d,%d,'%s');",strMac.c_str(),nTimeStampMsec,nPrsRoamingConnectStatus,strDes.c_str());
            gpUiLibCefHelpInterface->ExecuteJavaScriptToId(mcSimpleUiLibCefMsgHandler.mnBrowserID,strTemp);
        }   
    }

    void CCefBrowserUI::ReloadLocation()
    {
        if (mbIsCreated && mcSimpleUiLibCefMsgHandler.mnBrowserID > 0) 
        {
            gpUiLibCefHelpInterface->ExecuteJavaScriptToId(mcSimpleUiLibCefMsgHandler.mnBrowserID,L"ReloadLocation();");
        }
    }

    void CCefBrowserUI::SettingMaxListCount(int nMaxListCount)
    {
        if (mbIsCreated && mcSimpleUiLibCefMsgHandler.mnBrowserID > 0) 
        {
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"SettingMaxListCount(%d);",nMaxListCount);
            gpUiLibCefHelpInterface->ExecuteJavaScriptToId(mcSimpleUiLibCefMsgHandler.mnBrowserID,strTemp.c_str());
        }
    }

    void CCefBrowserUI::StartAutoScroll()
    {
        if (mbIsCreated && mcSimpleUiLibCefMsgHandler.mnBrowserID > 0) 
        {
            gpUiLibCefHelpInterface->ExecuteJavaScriptToId(mcSimpleUiLibCefMsgHandler.mnBrowserID,L"StartAutoScroll();");
        }
    }

    void CCefBrowserUI::StopAutoScroll()
    {
        if (mbIsCreated && mcSimpleUiLibCefMsgHandler.mnBrowserID > 0) 
        {
            gpUiLibCefHelpInterface->ExecuteJavaScriptToId(mcSimpleUiLibCefMsgHandler.mnBrowserID,L"StopAutoScroll();");
        } 
    }

    void CCefBrowserUI::LoadFile(const std::wstring& strFile)
    {
        url_ = strFile;
        DoInit();
    }

    void CCefBrowserUI::LoadUrl(const std::wstring& strUrl)
    {
        url_ = strUrl;
        DoInit();
    }
}
