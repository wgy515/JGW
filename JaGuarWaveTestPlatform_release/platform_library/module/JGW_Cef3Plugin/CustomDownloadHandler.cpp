#include "StdAfx.h"
#include "CustomDownloadHandler.h"

namespace JGW
{
    CustomDownloadHandler::CustomDownloadHandler(void)
    {
    }


    CustomDownloadHandler::~CustomDownloadHandler(void)
    {
    }

    void CustomDownloadHandler::OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback)
    {
        // 示例：强制弹出保存对话框
        callback->Continue("", true); 
    }

    void CustomDownloadHandler::OnDownloadUpdated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, CefRefPtr<CefDownloadItemCallback> callback)
    {

    }
}
