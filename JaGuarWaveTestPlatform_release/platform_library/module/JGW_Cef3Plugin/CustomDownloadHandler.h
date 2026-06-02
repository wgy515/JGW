#pragma once
#include <include/cef_download_handler.h>
namespace JGW
{
    class CustomDownloadHandler : public CefDownloadHandler
    {
    public:
        CustomDownloadHandler(void);
        ~CustomDownloadHandler(void);
    protected:
        //! 下载即将开始时	设置保存路径/取消下载
        void OnBeforeDownload(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefDownloadItem> download_item,
            const CefString& suggested_name,
            CefRefPtr<CefBeforeDownloadCallback> callback) override;
        //! OnDownloadUpdated	下载进度变化时	更新进度条/检测完成状态
        void OnDownloadUpdated(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefDownloadItem> download_item,
            CefRefPtr<CefDownloadItemCallback> callback) override;
    public:
        IMPLEMENT_REFCOUNTING(CustomDownloadHandler);
    };
}
