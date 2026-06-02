#include "StdAfx.h"
#include "CefHandlerDelegate.h"

namespace JGW
{
	CefHandlerDelegate::CefHandlerDelegate(UiLibCefMsgHandler* pUiLibCefMsgHandler) : mpUiLibCefMsgHandler(pUiLibCefMsgHandler)
	{

	}
    // Called when the browser is created.
    void CefHandlerDelegate::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
    {
        if (NULL != mpUiLibCefMsgHandler)
		{
			mpUiLibCefMsgHandler->OnBrowserCreated(browser->GetIdentifier());
		}
    }

    // Called when the browser is closing.
    void CefHandlerDelegate::OnBrowserClosing(CefRefPtr<CefBrowser> browser)
    {
		if (NULL != mpUiLibCefMsgHandler)
		{
			mpUiLibCefMsgHandler->OnBrowserClosing(browser->GetIdentifier());
		}
    }

    // Called when the browser has been closed.
    void CefHandlerDelegate::OnBrowserClosed(CefRefPtr<CefBrowser> browser)
    {
		if (NULL != mpUiLibCefMsgHandler)
		{
			mpUiLibCefMsgHandler->OnBrowserClosed(browser->GetIdentifier());
		}
    }

    // Set the window URL address.
    void CefHandlerDelegate::OnSetAddress(const std::string& url)
    {
		if (NULL != mpUiLibCefMsgHandler)
		{
			mpUiLibCefMsgHandler->OnSetAddress(url);
		}
    }

    // Set the window title.
    void CefHandlerDelegate::OnSetTitle(const std::string& title)
    {
		if (NULL != mpUiLibCefMsgHandler)
		{
			mpUiLibCefMsgHandler->OnSetTitle(title);
		}
    }

    // Set the Favicon image.
    void CefHandlerDelegate::OnSetFavicon(CefRefPtr<CefImage> image)
    {

    }

    // Set fullscreen mode.
    void CefHandlerDelegate::OnSetFullscreen(bool fullscreen)
    {
		if (NULL != mpUiLibCefMsgHandler)
		{
			mpUiLibCefMsgHandler->OnSetFullscreen(fullscreen);
		}
    }

    // Auto-resize contents.
    void CefHandlerDelegate::OnAutoResize(const CefSize& new_size)
    {
		if (NULL != mpUiLibCefMsgHandler)
		{
			mpUiLibCefMsgHandler->OnAutoResize(new_size.width,new_size.height);
		}
    }

    // Set the loading state.
    void CefHandlerDelegate::OnSetLoadingState(bool isLoading,
        bool canGoBack,
        bool canGoForward)
    {
		if (NULL != mpUiLibCefMsgHandler)
		{
			mpUiLibCefMsgHandler->OnSetLoadingState(isLoading,canGoBack,canGoForward);
		}
    }

    // Set the draggable regions.
    void CefHandlerDelegate::OnSetDraggableRegions(
        const std::vector<CefDraggableRegion>& regions)
    {

    }

    // Set focus to the next/previous control.
    void CefHandlerDelegate::OnTakeFocus(bool next)
    {
		if (NULL != mpUiLibCefMsgHandler)
		{
			mpUiLibCefMsgHandler->OnTakeFocus(next);
		}
    }

    // Called on the UI thread before a context menu is displayed.
    void CefHandlerDelegate::OnBeforeContextMenu(CefRefPtr<CefMenuModel> model)
    {

    }
}
