#pragma once
#include "include/cef_client.h"
#include <include/wrapper/cef_message_router.h>
#include <include/wrapper/cef_helpers.h>
#include <include/wrapper/cef_resource_manager.h>
#include <include/base/cef_lock.h>
#include <include\cef_browser_process_handler.h>
#include <UiLibCef\UiLibCefExportManager.h>

namespace JGW
{
    class CefHandlerDelegate
    {
    public:
		CefHandlerDelegate(UiLibCefMsgHandler* pUiLibCefMsgHandler);
        // Called when the browser is created.
        virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser);

        // Called when the browser is closing.
        virtual void OnBrowserClosing(CefRefPtr<CefBrowser> browser);

        // Called when the browser has been closed.
        virtual void OnBrowserClosed(CefRefPtr<CefBrowser> browser);

        // Set the window URL address.
        virtual void OnSetAddress(const std::string& url);

        // Set the window title.
        virtual void OnSetTitle(const std::string& title);

        // Set the Favicon image.
        virtual void OnSetFavicon(CefRefPtr<CefImage> image);

        // Set fullscreen mode.
        virtual void OnSetFullscreen(bool fullscreen);

        // Auto-resize contents.
        virtual void OnAutoResize(const CefSize& new_size);

        // Set the loading state.
        virtual void OnSetLoadingState(bool isLoading,
            bool canGoBack,
            bool canGoForward);

        // Set the draggable regions.
        virtual void OnSetDraggableRegions(
            const std::vector<CefDraggableRegion>& regions);

        // Set focus to the next/previous control.
        virtual void OnTakeFocus(bool next);

        // Called on the UI thread before a context menu is displayed.
        virtual void OnBeforeContextMenu(CefRefPtr<CefMenuModel> model);
    private:
        UiLibCefMsgHandler* mpUiLibCefMsgHandler;
    };
}


